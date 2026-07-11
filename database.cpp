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
               "score INTEGER"
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
    if(!query.prepare("INSERT INTO Player("
                  "username,"
                  "password,"
                  "level,"
                  "current_Hp,"
                  "max_Hp,"
                  "coin,"
                  "score"
                  ")"
                  "VALUE(?,?,?,?,?,?)"))
    {return false;}

    query.addBindValue(p->name());
    query.addBindValue(0);
    query.addBindValue(0);
    query.addBindValue(p->currentHP());
    query.addBindValue(p->maxHP());
    query.addBindValue(p->coin());
    query.addBindValue(0);

    if(!query.exec())
    {
        qDebug() << db.lastError().text();
        return false;
    }

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
                  "name=?,"
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

QVector<Player> Database::selectAllPlayers()
{
    QVector<Player> players;
    QSqlQuery query("SELECT * FROM Player");

    while(query.next())
    {
        Player player(query.value(1).toString(), query.value(5).toInt());
        //player.setID(query.value(0).toInt());
        //player.setPass(query.value(2).toString());
        //player.setLevel(query.value(3).toInt());
        //player.setHp(query.value(4).toInt());
        //player.setCoin(query.value(6).toInt());
        //player.setScore(query.value(7).toInt());

        players.append(player);
    }

    return players;
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
