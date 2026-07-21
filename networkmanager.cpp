#include "networkmanager.h"

#include <QDataStream>
#include <QDebug>
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
    disconnectFromGame();
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
    m_hadSuccessfulConnection = true;

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

    connect(m_socket, &QTcpSocket::connected, this, [this]() {
        m_hadSuccessfulConnection = true;
        emit connectedToHost();
    });

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
    bool shouldNotify = m_hadSuccessfulConnection && isConnected();

    if (m_socket) {
        m_socket->disconnect(this);
        m_socket->disconnectFromHost();
        m_socket->deleteLater();
        m_socket = nullptr;
    }

    if (m_server) {
        m_server->disconnect(this);
        m_server->close();
        m_server->deleteLater();
        m_server = nullptr;
    }

    clearEnemySync();

    if (shouldNotify)
        emit disconnected();

    m_hadSuccessfulConnection = false;
}

void NetworkManager::onSocketDisconnected()
{
    if (m_hadSuccessfulConnection)
        emit disconnected();

    teardownSocket();
    m_hadSuccessfulConnection = false;
}

void NetworkManager::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);

    if (!m_hadSuccessfulConnection) {
        if (!m_isHost)
            emit connectionFailed(m_socket ? m_socket->errorString() : QString());
        teardownSocket();
        return;
    }

    emit disconnected();
    teardownSocket();
    m_hadSuccessfulConnection = false;
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
    static const quint32 kMaxReasonableBodySize = 1024 * 1024;

    while (true) {
        if (m_recvBuffer.size() < static_cast<int>(sizeof(quint32)))
            return;

        QDataStream peekStream(m_recvBuffer);
        peekStream.setVersion(kStreamVersion);

        quint32 bodySize = 0;
        peekStream >> bodySize;

        if (bodySize == 0 || bodySize > kMaxReasonableBodySize) {
            qWarning("NetworkManager: invalid packet length (%u bytes); dropping connection.",
                     bodySize);
            disconnectFromGame();
            return;
        }

        int totalNeeded = static_cast<int>(sizeof(quint32) + bodySize);
        if (m_recvBuffer.size() < totalNeeded)
            return;

        QByteArray body = m_recvBuffer.mid(sizeof(quint32), bodySize);
        m_recvBuffer.remove(0, totalNeeded);

        QDataStream bodyStream(body);
        bodyStream.setVersion(kStreamVersion);

        quint8 rawType = 0;
        bodyStream >> rawType;

        if (bodyStream.status() != QDataStream::Ok) {
            qWarning(
                "NetworkManager: could not read packet type from a received frame; skipping it.");
            continue;
        }

        if (rawType > static_cast<quint8>(PacketType::TurnEnded)) {
            qWarning("NetworkManager: received an unknown packet type (%u); skipping it.", rawType);
            continue;
        }

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

    if (stream.status() != QDataStream::Ok)
        return 0;

    return seed;
}

// ==================== NodeSelection ====================

void NetworkManager::sendNodeSelection(int levelIndex, int levelPosIndex, MapButtonType type)
{
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setVersion(kStreamVersion);
    stream << static_cast<qint32>(levelIndex) << static_cast<qint32>(levelPosIndex)
           << static_cast<quint8>(type);
    sendPacket(PacketType::NodeSelection, payload);
}

bool NetworkManager::decodeNodeSelection(const QByteArray &payload,
                                         int &levelIndex,
                                         int &levelPosIndex,
                                         MapButtonType &type)
{
    QDataStream stream(payload);
    stream.setVersion(kStreamVersion);

    qint32 level = 0, pos = 0;
    quint8 rawType = 0;
    stream >> level >> pos >> rawType;

    if (stream.status() != QDataStream::Ok)
        return false;

    levelIndex = level;
    levelPosIndex = pos;
    type = static_cast<MapButtonType>(rawType);
    return true;
}

// ==================== Player / Enemy State Sync ====================

void NetworkManager::sendPlayerStateSync(Player *player, bool targetIsReceiverSelf)
{
    if (!player)
        return;

    QVector<QPair<quint8, int>> buffs;
    for (BuffDebuff *b : player->getActiveEffects())
        buffs.append({static_cast<quint8>(b->type()), b->stacks()});

    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setVersion(kStreamVersion);
    stream << static_cast<quint8>(targetIsReceiverSelf ? 1 : 0) << player->currentHP()
           << player->maxHP() << player->block() << player->energy() << buffs;

    sendPacket(PacketType::PlayerStateSync, payload);
}

NetPlayerState NetworkManager::decodePlayerStateSync(const QByteArray &payload)
{
    NetPlayerState state;
    quint8 targetFlag = 0;

    QDataStream stream(payload);
    stream.setVersion(kStreamVersion);
    stream >> targetFlag >> state.currentHP >> state.maxHP >> state.block >> state.energy
        >> state.buffs;

    state.isValid = (stream.status() == QDataStream::Ok);
    state.targetIsReceiverSelf = (targetFlag != 0);
    return state;
}

void NetworkManager::sendEnemyStateSync(Enemy *enemy, int entityId)
{
    if (!enemy)
        return;

    QVector<QPair<quint8, int>> buffs;
    for (BuffDebuff *b : enemy->getActiveEffects())
        buffs.append({static_cast<quint8>(b->type()), b->stacks()});

    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setVersion(kStreamVersion);
    stream << static_cast<quint8>(entityId) << enemy->currentHP() << enemy->maxHP()
           << enemy->block() << buffs;

    sendPacket(PacketType::EnemyStateSync, payload);
}

NetEnemyState NetworkManager::decodeEnemyStateSync(const QByteArray &payload)
{
    NetEnemyState state;
    quint8 rawId = 0;

    QDataStream stream(payload);
    stream.setVersion(kStreamVersion);
    stream >> rawId >> state.currentHP >> state.maxHP >> state.block >> state.buffs;

    if (stream.status() != QDataStream::Ok) {
        state.entityId = -1;
        return state;
    }

    state.entityId = rawId;
    return state;
}

void NetworkManager::registerEnemiesForSync(const std::vector<Enemy *> &enemies)
{
    clearEnemySync();

    for (Enemy *enemy : enemies) {
        if (!enemy)
            continue;

        int entityId = enemy->networkEntityId();
        connect(enemy, &Combatant::combatStateChanged, this, [this, enemy, entityId]() {
            sendEnemyStateSync(enemy, entityId);
        });
        m_syncedEnemies.append(enemy);
    }
}

void NetworkManager::clearEnemySync()
{
    for (Enemy *enemy : m_syncedEnemies)
        if (enemy)
            disconnect(enemy, &Combatant::combatStateChanged, this, nullptr);

    m_syncedEnemies.clear();
}

// ==================== Leader / Eliminated / GameOver / TurnEnded ====================

void NetworkManager::sendLeaderChanged(bool receiverBecomesLeader)
{
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setVersion(kStreamVersion);
    stream << static_cast<quint8>(receiverBecomesLeader ? 1 : 0);
    sendPacket(PacketType::LeaderChanged, payload);
}

bool NetworkManager::decodeLeaderChanged(const QByteArray &payload)
{
    quint8 raw = 0;
    QDataStream stream(payload);
    stream.setVersion(kStreamVersion);
    stream >> raw;

    if (stream.status() != QDataStream::Ok)
        return false;

    return raw != 0;
}

void NetworkManager::sendPlayerEliminated(bool wasLeader)
{
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setVersion(kStreamVersion);
    stream << static_cast<quint8>(wasLeader ? 1 : 0);
    sendPacket(PacketType::PlayerEliminated, payload);
}

bool NetworkManager::decodePlayerEliminated(const QByteArray &payload)
{
    quint8 raw = 0;
    QDataStream stream(payload);
    stream.setVersion(kStreamVersion);
    stream >> raw;

    if (stream.status() != QDataStream::Ok)
        return false;

    return raw != 0;
}

void NetworkManager::sendGameOver(bool victory)
{
    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setVersion(kStreamVersion);
    stream << static_cast<quint8>(victory ? 1 : 0);
    sendPacket(PacketType::GameOver, payload);
}

bool NetworkManager::decodeGameOver(const QByteArray &payload)
{
    quint8 raw = 0;
    QDataStream stream(payload);
    stream.setVersion(kStreamVersion);
    stream >> raw;
    return raw != 0;
}

void NetworkManager::sendTurnEnded()
{
    sendPacket(PacketType::TurnEnded, QByteArray());
}

// ==================== CardPlayed (Just showing) ====================

void NetworkManager::sendCardPlayed(Card *card)
{
    if (!card)
        return;

    QByteArray payload;
    QDataStream stream(&payload, QIODevice::WriteOnly);
    stream.setVersion(kStreamVersion);
    stream << static_cast<qint32>(card->ID()) << static_cast<quint8>(card->isUpgraded() ? 1 : 0);

    sendPacket(PacketType::CardPlayed, payload);
}

bool NetworkManager::decodeCardPlayed(const QByteArray &payload, int &cardID, bool &isUpgraded)
{
    qint32 id = 0;
    quint8 upgraded = 0;
    QDataStream stream(payload);
    stream.setVersion(kStreamVersion);
    stream >> id >> upgraded;

    if (stream.status() != QDataStream::Ok)
        return false;

    cardID = id;
    isUpgraded = (upgraded != 0);
    return true;
}

void NetworkManager::teardownSocket()
{
    if (!m_socket)
        return;

    m_socket->disconnect(this);
    m_socket->deleteLater();
    m_socket = nullptr;

    clearEnemySync();
}
