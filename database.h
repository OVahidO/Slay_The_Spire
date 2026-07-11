#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

/////////////
class Player;
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
    static QVector<Player> selectAllPlayers();
    //////////////////////////////////////////
    static bool creatPlayersDeckTable();
    static bool insertPlayersDeckValue(Player* p);
    static bool updatePlayersDeckValue(Player* p);
    static bool deletePlayersDeckValue(Player* p);
    //////////////////////////////////////////
    static void close();

signals:
};

#endif // DATABASE_H
