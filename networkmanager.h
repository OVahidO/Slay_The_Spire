#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QAbstractSocket>
#include <QByteArray>
#include <QObject>
#include <QPair>
#include <QVector>

#include "buffdebuff.h"
#include "mapButton.h"

class QTcpServer;
class QTcpSocket;
class Player;
class Enemy;
class Card;

enum class PacketType : quint8 {
    Handshake,
    MapSeed,
    NodeSelection,
    EnemyStateSync,
    PlayerStateSync,
    CardPlayed,
    LeaderChanged,
    PlayerEliminated,
    GameOver,
    TurnEnded,
    EnemySpawned
};

struct NetPlayerState
{
    bool isValid = false;
    bool targetIsReceiverSelf = false;

    int currentHP = 0;
    int maxHP = 0;
    int block = 0;
    int energy = 0;
    QVector<QPair<quint8, int>> buffs; // (BuffDebuffType, stacks)
};

struct NetEnemyState
{
    int entityId = -1;

    quint8 enemyIndex = 0;
    int currentHP = 0;
    int maxHP = 0;
    int block = 0;
    QVector<QPair<quint8, int>> buffs;
};

enum class NetSpawnKind : quint8 { AcidSlimeS = 0, AcidSlimeM = 1, AcidSlimeL = 2 };

struct NetEnemySpawn
{
    bool isValid = false;
    NetSpawnKind kind = NetSpawnKind::AcidSlimeS;
    int hp = 0;
    int entityId = -1;
};

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    bool hostGame(quint16 port);
    bool joinGame(const QString &hostAddress, quint16 port);
    void disconnectFromGame();

    bool isConnected() const;
    bool isHost() const;

    // --- Sender ---
    void sendHandshake(const QString &playerName);
    void sendMapSeed(quint32 seed);
    void sendNodeSelection(int levelIndex, int levelPosIndex, MapButtonType type);
    void sendPlayerStateSync(Player *player, bool targetIsReceiverSelf = false);
    void sendLeaderChanged(bool receiverBecomesLeader);
    void sendPlayerEliminated(bool wasLeader);
    void sendGameOver(bool victory);
    void sendCardPlayed(int cardID, bool isUpgraded, int targetEntityId);
    void sendTurnEnded();
    void sendEnemySpawned(NetSpawnKind kind, int hp, int entityId);

    // --- Enemy Sync ---
    void registerEnemiesForSync(const std::vector<Enemy *> &enemies);
    void clearEnemySync();
    void registerSingleEnemyForSync(Enemy *enemy);

    // --- Static Methods ---
    static quint32 decodeMapSeed(const QByteArray &payload);
    static bool decodeNodeSelection(const QByteArray &payload,
                                    int &levelIndex,
                                    int &levelPosIndex,
                                    MapButtonType &type);
    static NetPlayerState decodePlayerStateSync(const QByteArray &payload);
    static NetEnemyState decodeEnemyStateSync(const QByteArray &payload);
    static bool decodeLeaderChanged(const QByteArray &payload);    // true = Leader
    static bool decodePlayerEliminated(const QByteArray &payload); // true = Leader Eliminated
    static bool decodeGameOver(const QByteArray &payload);         // true = win
    static QString decodeHandshake(const QByteArray &payload);
    static bool decodeCardPlayed(const QByteArray &payload,
                                 int &cardID,
                                 bool &isUpgraded,
                                 int &targetEntityId);
    static NetEnemySpawn decodeEnemySpawned(const QByteArray &payload);

signals:
    void hostStarted(quint16 port);
    void hostFailed(const QString &error);
    void clientConnected();
    void connectedToHost();
    void connectionFailed(const QString &error);
    void disconnected();

    void packetReceived(PacketType type, const QByteArray &payload);

private slots:
    void onNewConnection();
    void onSocketReadyRead();
    void onSocketDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);

private:
    QTcpServer *m_server = nullptr;
    QTcpSocket *m_socket = nullptr;
    bool m_isHost = false;

    QByteArray m_recvBuffer;
    QVector<Enemy *> m_syncedEnemies;

    void setupSocket(QTcpSocket *socket);
    void processBuffer();
    void sendPacket(PacketType type, const QByteArray &payload);
    void sendEnemyStateSync(Enemy *enemy, int entityId);

    void teardownSocket();
    bool m_hadSuccessfulConnection = false;
};

#endif // NETWORKMANAGER_H
