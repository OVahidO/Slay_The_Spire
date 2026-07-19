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
