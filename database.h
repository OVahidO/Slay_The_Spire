#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPair>

/////////////
class Player;
class Card;
/////////////

class Database : public QObject
{
    Q_OBJECT

private:
    Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    static QSqlDatabase db;

public:
    static Database& instance();

    static bool open(QString dbPath);
    //////////////////////////////////////////

    static bool creatPlayerTable();
    static bool insertPlayerValue(Player* p);
    static bool updatePlayerValue(Player* p);
    static bool deletePlayerValue(Player* p);
    static QVector<QPair<Player* , unsigned int>> selectAllPlayers();
    static Player *loadPlayerById(int playerID);
    static bool validateLogin(const QString &username, const QString &password, int &outPlayerID);
    static bool updatePlayerScore(int playerID, int score);
    static QVector<QPair<QString, int>> topScores(int limit = 10);

    //////////////////////////////////////////

    static bool creatPlayersDeckTable();
    static bool insertPlayersDeckValue(Player* p);
    static bool updatePlayersDeckValue(Player* p);
    static bool deletePlayersDeckValue(Player* p);
    static QVector<Card*> selectPlayersDeck(Player* p);

    //////////////////////////////////////////

    static bool updateMapSeedValue(int playerID, unsigned int seed);

    //////////////////////////////////////////
    // --- Run state: Current Act/Floor/Node, energy, map seed ---
    static bool creatRunStateTable();
    static bool hasActiveRun(int playerID);
    static bool saveRunState(int playerID,
                             int currentAct,
                             int currentFloor,
                             int currentNode,
                             int energy,
                             int maxEnergy,
                             unsigned int mapSeed);
    static bool loadRunState(int playerID,
                             int &currentAct,
                             int &currentFloor,
                             int &currentNode,
                             int &energy,
                             int &maxEnergy,
                             unsigned int &mapSeed);
    static bool deleteRunState(int playerID);

    //////////////////////////////////////////
    // --- Run deck persistence (GameManager reconstructs Card* via Card::Creat) ---
    static bool creatRunDeckTable();
    static bool saveRunDeck(int playerID, const std::vector<Card *> &deck);
    static std::vector<Card *> loadRunDeck(int playerID);

    //////////////////////////////////////////
    // --- Run relics persistence (names only; GameManager reconstructs the objects) ---
    static bool creatRunRelicsTable();
    static bool saveRunRelics(int playerID, const QStringList &relicNames);
    static QStringList loadRunRelics(int playerID);

    //////////////////////////////////////////
    // --- Run potions persistence (type tags only) ---
    static bool creatRunPotionsTable();
    static bool saveRunPotions(int playerID, const QStringList &potionTags);
    static QStringList loadRunPotions(int playerID);
    //////////////////////////////////////////

    static void close();

signals:
};

#endif // DATABASE_H
