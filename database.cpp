#include "database.h"
#include "player.h"
#include "card.h"
#include <QDebug>

QSqlDatabase Database::db;

Database::Database()
{}

Database& Database::instance()
{
    static Database database;
    return database;
}

bool Database::open(QString dbPath)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    if(!db.open())
    {
        qDebug() << db.lastError().text();
        return false;
    }
    return true;
}

void Database::close()
{
    db.close();
}

bool Database::creatPlayerTable()
{
    QSqlQuery query(db);
    //player ID
    //player Password;
    if(!query.exec("CREATE TABLE IF NOT EXISTS Player ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "password TEXT,"
               "username TEXT,"
               "level INTEGER,"
               "current_Hp INTEGER,"
               "max_Hp INTEGER,"
               "coin INTEGER,"
               "score INTEGER,"
               "seed INTEGER"
               ")"))
    {
        qDebug() << db.lastError().text();
        return false;
    }

    return true;
}

bool Database::insertPlayerValue(Player* p)
{
    QSqlQuery query(db);
    //player ID;
    //player Password;
    if (!query.prepare("INSERT INTO Player("
                       "username,"
                       "password,"
                       "level,"
                       "current_Hp,"
                       "max_Hp,"
                       "coin,"
                       "score,"
                       "seed"
                       ")"
                       "VALUES(?,?,?,?,?,?,?,?)")) {
        qDebug() << db.lastError().text();
        return false;
    }

    query.addBindValue(p->name());
    // query.addBindValue(password);
    query.addBindValue(0);
    query.addBindValue(p->currentHP());
    query.addBindValue(p->maxHP());
    query.addBindValue(p->coin());
    query.addBindValue(0);
    query.addBindValue(0);

    if(!query.exec())
    {
        qDebug() << db.lastError().text();
        return false;
    }

    // p->setId(query.lastInsertId().toInt());

    return true;
}

bool Database::deletePlayerValue(Player* p)
{
    QSqlQuery query(db);

    if(!query.prepare("DELETE FROM Player WHERE id=?"))
    {
        qDebug() << db.lastError().text();
        return false;
    }
    //add player ID;
    //query.addBindValue(p->ID());

    if(!query.exec())
    {
        qDebug() << db.lastError().text();
        return false;
    }

    return true;
}

bool Database::updatePlayerValue(Player* p)
{
    QSqlQuery query(db);

    if(!query.prepare("UPDATE Player SET "
                  "username=?,"
                  "level=?,"
                  "current_Hp=?,"
                  "max_Hp=?,"
                  "coin=?,"
                  "score=?"
                  "WHERE id=?"))
    {
        qDebug() << db.lastError().text();
        return false;
    }

    query.addBindValue(p->name());
    query.addBindValue(0);
    query.addBindValue(p->currentHP());
    query.addBindValue(p->maxHP());
    query.addBindValue(p->coin());
    query.addBindValue(0);
    //Player ID
    //query.addBindValue(p->id());

    if(!query.exec())
    {
        qDebug() << db.lastError().text();
        return false;
    }

    return true;
}

QVector<QPair<Player* , unsigned int>> Database::selectAllPlayers()
{
    QVector<QPair<Player* , unsigned int>> players;
    QSqlQuery query("SELECT * FROM Player", db);

    while(query.next())
    {
        //
        // Player *player = new Player(query.value(2).toString(), query.value(5).toInt());
        // player->setId(query.value(0).toInt());
        // player->setCurrentHPDirect(query.value(4).toInt());
        // player->setCoin(query.value(6).toInt());
        //
        Player* player = new Player(query.value(1).toString(), query.value(5).toInt());
        //player->setID(query.value(0).toInt());
        //player->setPass(query.value(2).toString());
        //player->setLevel(query.value(3).toInt());
        //player->setHp(query.value(4).toInt());
        //player->setCoin(query.value(6).toInt());
        //player->setScore(query.value(7).toInt());

        players.append({player, query.value(8).toInt()});
    }

    return players;
}
//
Player *Database::loadPlayerById(int playerID)
{
    QSqlQuery query(db);
    query.prepare("SELECT id, username, current_Hp, max_Hp, coin FROM Player WHERE id=?");
    query.addBindValue(playerID);

    if (!query.exec() || !query.next())
        return nullptr;

    Player *player = new Player(query.value(1).toString(), query.value(3).toInt());
    // player->setId(query.value(0).toInt());
    // player->setCurrentHPDirect(query.value(2).toInt());
    player->setCoin(query.value(4).toInt());

    return player;
}

bool Database::validateLogin(const QString &username, const QString &password, int &outPlayerID)
{
    QSqlQuery query(db);
    if (!query.prepare("SELECT id FROM Player WHERE username = ? AND password = ?")) {
        qDebug() << db.lastError().text();
        return false;
    }

    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec() || !query.next())
        return false;

    outPlayerID = query.value(0).toInt();
    return true;
}

bool Database::updatePlayerScore(int playerID, int score)
{
    QSqlQuery query(db);
    if (!query.prepare("UPDATE Player SET score=? WHERE id=?")) {
        qDebug() << db.lastError().text();
        return false;
    }

    query.addBindValue(score);
    query.addBindValue(playerID);

    return query.exec();
}

QVector<QPair<QString, int>> Database::topScores(int limit)
{
    QVector<QPair<QString, int>> result;

    QSqlQuery query(db);
    query.prepare("SELECT username, score FROM Player ORDER BY score DESC LIMIT ?");
    query.addBindValue(limit);

    if (query.exec())
        while (query.next())
            result.append({query.value(0).toString(), query.value(1).toInt()});

    return result;
}

//
bool Database::updateMapSeedValue(int playerID, unsigned int seed)
{
    QSqlQuery query(db);

    if(!query.prepare("UPDATE Player SET "
                       "seed=?"
                       "WHERE id=?"))
    {
        qDebug() << db.lastError().text();
        return false;
    }

    query.addBindValue(seed);
    query.addBindValue(playerID);

    if(!query.exec())
    {
        qDebug() << db.lastError().text();
        return false;
    }

    return true;
}

bool Database::creatPlayersDeckTable()
{
    QSqlQuery query(db);

    if(!query.exec("CREATE TABLE IF NOT EXISTS PlayersDeck ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "playerID INTEGER,"
                    "cardID INTEGER"
                    ")"))
    {
        qDebug() << db.lastError().text();
        return false;
    }

    return true;
}

bool Database::insertPlayersDeckValue(Player* p)
{
    QSqlQuery query(db);

    if(!query.prepare("INSERT INTO PlayersDeck ("
                       "playerID,"
                       "cardID"
                       ")"
                       "VALUE(?,?)"))
    {
        qDebug() << db.lastError().text();
        return false;
    }

    db.transaction();

    for(Card* card : p->Deck())
    {
        //query.addBindValue(p->ID());
        query.addBindValue(card->ID());

        if(!query.exec())
        {
            qDebug() << db.lastError().text();
            db.rollback();
            return false;
        }
    }

    db.commit();

    return true;
}

bool Database::updatePlayersDeckValue(Player* p)
{
    if(!Database::deletePlayersDeckValue(p))
        return false;

    return Database::insertPlayersDeckValue(p);
}

bool Database::deletePlayersDeckValue(Player* p)
{
    QSqlQuery query(db);

    if(!query.prepare("DELETE FROM PlayersDeck WHERE playerID=?"))
    {
        qDebug() << db.lastError().text();
        return false;
    }
    //add player ID;
    //query.addBindValue(p->ID());

    if(!query.exec())
    {
        qDebug() << db.lastError().text();
        return false;
    }

    return true;
}

QVector<Card*> Database::selectPlayersDeck(Player* p)
{
    QVector<Card*> Deck;
    QSqlQuery query(db);

    if(!query.prepare("SELECT * FROM PlayersDeck WHERE playerID=?"))
    {
        qDebug() << db.lastError().text();
        return Deck;
    }

    //query.addBindValue(p->ID());

    if(!query.exec())
    {
        qDebug() << db.lastError().text();
        return Deck;
    }

    while(query.next())
    {
        CardID cardID = static_cast<CardID>(query.value(2).toInt());
        Card *c = Card::Creat(cardID);
        Deck.append(c);
    }

    return Deck;
}

// Run state

bool Database::creatRunStateTable()
{
    QSqlQuery query(db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS RunState ("
                    "playerID INTEGER PRIMARY KEY,"
                    "currentAct INTEGER,"
                    "currentFloor INTEGER,"
                    "currentNode INTEGER,"
                    "energy INTEGER,"
                    "maxEnergy INTEGER,"
                    "mapSeed INTEGER"
                    ")")) {
        qDebug() << db.lastError().text();
        return false;
    }
    return true;
}

bool Database::hasActiveRun(int playerID)
{
    QSqlQuery query(db);
    query.prepare("SELECT playerID FROM RunState WHERE playerID=?");
    query.addBindValue(playerID);

    if (!query.exec())
        return false;

    return query.next();
}

bool Database::saveRunState(int playerID,
                            int currentAct,
                            int currentFloor,
                            int currentNode,
                            int energy,
                            int maxEnergy,
                            unsigned int mapSeed)
{
    QSqlQuery query(db);
    query.prepare("INSERT OR REPLACE INTO RunState "
                  "(playerID, currentAct, currentFloor, currentNode, energy, maxEnergy, mapSeed) "
                  "VALUES (?,?,?,?,?,?,?)");
    query.addBindValue(playerID);
    query.addBindValue(currentAct);
    query.addBindValue(currentFloor);
    query.addBindValue(currentNode);
    query.addBindValue(energy);
    query.addBindValue(maxEnergy);
    query.addBindValue(static_cast<qulonglong>(mapSeed));

    if (!query.exec()) {
        qDebug() << db.lastError().text();
        return false;
    }
    return true;
}

bool Database::loadRunState(int playerID,
                            int &currentAct,
                            int &currentFloor,
                            int &currentNode,
                            int &energy,
                            int &maxEnergy,
                            unsigned int &mapSeed)
{
    QSqlQuery query(db);
    query.prepare("SELECT currentAct, currentFloor, currentNode, energy, maxEnergy, mapSeed "
                  "FROM RunState WHERE playerID=?");
    query.addBindValue(playerID);

    if (!query.exec() || !query.next())
        return false;

    currentAct = query.value(0).toInt();
    currentFloor = query.value(1).toInt();
    currentNode = query.value(2).toInt();
    energy = query.value(3).toInt();
    maxEnergy = query.value(4).toInt();
    mapSeed = static_cast<unsigned int>(query.value(5).toULongLong());

    return true;
}

bool Database::deleteRunState(int playerID)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM RunState WHERE playerID=?");
    query.addBindValue(playerID);
    return query.exec();
}

// Run deck
bool Database::creatRunDeckTable()
{
    QSqlQuery query(db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS RunDeck ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "playerID INTEGER,"
                    "cardID INTEGER,"
                    "isUpgraded INTEGER"
                    ")")) {
        qDebug() << db.lastError().text();
        return false;
    }
    return true;
}

bool Database::saveRunDeck(int playerID, const std::vector<Card *> &deck)
{
    QSqlQuery clearQuery(db);
    clearQuery.prepare("DELETE FROM RunDeck WHERE playerID=?");
    clearQuery.addBindValue(playerID);
    clearQuery.exec();

    for (Card *card : deck) {
        if (!card)
            continue;

        QSqlQuery query(db);
        query.prepare("INSERT INTO RunDeck (playerID, cardID, isUpgraded) VALUES (?,?,?)");
        query.addBindValue(playerID);
        query.addBindValue(card->ID());
        query.addBindValue(card->isUpgraded() ? 1 : 0);
        query.exec();
    }

    return true;
}

std::vector<Card *> Database::loadRunDeck(int playerID)
{
    std::vector<Card *> result;

    QSqlQuery query(db);
    query.prepare("SELECT cardID, isUpgraded FROM RunDeck WHERE playerID=?");
    query.addBindValue(playerID);

    if (!query.exec())
        return result;

    while (query.next()) {
        CardID id = static_cast<CardID>(query.value(0).toInt());
        bool upgraded = query.value(1).toInt() != 0;

        Card *card = Card::Creat(id);
        if (card) {
            if (upgraded)
                card->upgrade();
            result.push_back(card);
        }
    }

    return result;
}

// Run relics
bool Database::creatRunRelicsTable()
{
    QSqlQuery query(db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS RunRelics ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "playerID INTEGER,"
                    "relicName TEXT"
                    ")")) {
        qDebug() << db.lastError().text();
        return false;
    }
    return true;
}

bool Database::saveRunRelics(int playerID, const QStringList &relicNames)
{
    QSqlQuery clearQuery(db);
    clearQuery.prepare("DELETE FROM RunRelics WHERE playerID=?");
    clearQuery.addBindValue(playerID);
    clearQuery.exec();

    for (const QString &name : relicNames) {
        QSqlQuery query(db);
        query.prepare("INSERT INTO RunRelics (playerID, relicName) VALUES (?,?)");
        query.addBindValue(playerID);
        query.addBindValue(name);
        query.exec();
    }

    return true;
}

QStringList Database::loadRunRelics(int playerID)
{
    QStringList result;

    QSqlQuery query(db);
    query.prepare("SELECT relicName FROM RunRelics WHERE playerID=?");
    query.addBindValue(playerID);

    if (query.exec())
        while (query.next())
            result << query.value(0).toString();

    return result;
}

// Run potions
bool Database::creatRunPotionsTable()
{
    QSqlQuery query(db);
    if (!query.exec("CREATE TABLE IF NOT EXISTS RunPotions ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "playerID INTEGER,"
                    "potionTag TEXT"
                    ")")) {
        qDebug() << db.lastError().text();
        return false;
    }
    return true;
}

bool Database::saveRunPotions(int playerID, const QStringList &potionTags)
{
    QSqlQuery clearQuery(db);
    clearQuery.prepare("DELETE FROM RunPotions WHERE playerID=?");
    clearQuery.addBindValue(playerID);
    clearQuery.exec();

    for (const QString &tag : potionTags) {
        QSqlQuery query(db);
        query.prepare("INSERT INTO RunPotions (playerID, potionTag) VALUES (?,?)");
        query.addBindValue(playerID);
        query.addBindValue(tag);
        query.exec();
    }

    return true;
}

QStringList Database::loadRunPotions(int playerID)
{
    QStringList result;

    QSqlQuery query(db);
    query.prepare("SELECT potionTag FROM RunPotions WHERE playerID=?");
    query.addBindValue(playerID);

    if (query.exec())
        while (query.next())
            result << query.value(0).toString();

    return result;
}