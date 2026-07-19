#include "networkmanager.h"

#include <QDataStream>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>

#include "buffdebuff.h"
#include "card.h"
#include "combatant.h"
#include "enemy.h"
#include "player.h"

static const auto kStreamVersion = QDataStream::Qt_6_5;

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
{}

NetworkManager::~NetworkManager()
{
    clearEnemySync();
}

// ==================== Connection ====================

bool NetworkManager::hostGame(quint16 port)
{
    if (m_server || m_socket)
        return false;

    m_isHost = true;
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);

    if (!m_server->listen(QHostAddress::AnyIPv4, port)) {
        emit hostFailed(m_server->errorString());
        m_server->deleteLater();
        m_server = nullptr;
        return false;
    }

    emit hostStarted(port);
    return true;
}

void NetworkManager::onNewConnection()
{
    if (!m_server)
        return;

    QTcpSocket *incoming = m_server->nextPendingConnection();

    if (m_socket) {
        incoming->disconnectFromHost();
        incoming->deleteLater();
        return;
    }

    m_socket = incoming;
    setupSocket(m_socket);

    m_server->close();

    emit clientConnected();
}

bool NetworkManager::joinGame(const QString &hostAddress, quint16 port)
{
    if (m_socket)
        return false;

    m_isHost = false;
    m_socket = new QTcpSocket(this);
    setupSocket(m_socket);

    connect(m_socket, &QTcpSocket::connected, this, [this]() { emit connectedToHost(); });

    m_socket->connectToHost(hostAddress, port);
    return true;
}

void NetworkManager::setupSocket(QTcpSocket *socket)
{
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onSocketReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::onSocketDisconnected);
    connect(socket, &QAbstractSocket::errorOccurred, this, &NetworkManager::onSocketError);
}

void NetworkManager::disconnectFromGame()
{
    clearEnemySync();

    if (m_socket) {
        m_socket->disconnectFromHost();
        m_socket->deleteLater();
        m_socket = nullptr;
    }
    if (m_server) {
        m_server->close();
        m_server->deleteLater();
        m_server = nullptr;
    }
}

void NetworkManager::onSocketDisconnected()
{
    emit disconnected();
}

void NetworkManager::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    if (!m_isHost)
        emit connectionFailed(m_socket ? m_socket->errorString() : QString());
}

bool NetworkManager::isConnected() const
{
    return m_socket && m_socket->state() == QAbstractSocket::ConnectedState;
}

bool NetworkManager::isHost() const
{
    return m_isHost;
}

// ==================== Framing (Length-Prefix) ====================
// [quint32 bodySize][quint8 packetType][payload...]

void NetworkManager::sendPacket(PacketType type, const QByteArray &payload)
{
    if (!isConnected())
        return;

    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(kStreamVersion);

    quint32 bodySize = static_cast<quint32>(sizeof(quint8) + payload.size());
    stream << bodySize;
    stream << static_cast<quint8>(type);
    block.append(payload);

    m_socket->write(block);
}

void NetworkManager::onSocketReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket)
        return;

    m_recvBuffer.append(socket->readAll());
    processBuffer();
}

void NetworkManager::processBuffer()
{
    while (true) {
        if (m_recvBuffer.size() < static_cast<int>(sizeof(quint32)))
            return;

        QDataStream peekStream(m_recvBuffer);
        peekStream.setVersion(kStreamVersion);

        quint32 bodySize = 0;
        peekStream >> bodySize;

        int totalNeeded = static_cast<int>(sizeof(quint32) + bodySize);
        if (m_recvBuffer.size() < totalNeeded)
            return;

        QByteArray body = m_recvBuffer.mid(sizeof(quint32), bodySize);
        m_recvBuffer.remove(0, totalNeeded);

        QDataStream bodyStream(body);
        bodyStream.setVersion(kStreamVersion);

        quint8 rawType = 0;
        bodyStream >> rawType;

        QByteArray payload = body.mid(sizeof(quint8));

        emit packetReceived(static_cast<PacketType>(rawType), payload);
    }
}

// ==================== Handshake / MapSeed ====================

void NetworkManager::sendHandshake(const QString &playerName)
{
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setVersion(kStreamVersion);
    stream << playerName;
    sendPacket(PacketType::Handshake, payload);
}

QString NetworkManager::decodeHandshake(const QByteArray &payload)
{
    QString name;
    QDataStream stream(payload);
    stream.setVersion(kStreamVersion);
    stream >> name;
    return name;
}

void NetworkManager::sendMapSeed(quint32 seed)
{
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setVersion(kStreamVersion);
    stream << seed;
    sendPacket(PacketType::MapSeed, payload);
}

quint32 NetworkManager::decodeMapSeed(const QByteArray &payload)
{
    quint32 seed = 0;
    QDataStream stream(payload);
    stream.setVersion(kStreamVersion);
    stream >> seed;
    return seed;
}
