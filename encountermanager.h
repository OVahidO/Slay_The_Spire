#ifndef ENCOUNTERMANAGER_H
#define ENCOUNTERMANAGER_H

#include <QObject>
#include <QVector>

class Enemy;

class EncounterManager : public QObject
{
    Q_OBJECT
public:
    static QVector<Enemy *> generateEncounter(int act,
                                              int floor,
                                              bool isElite,
                                              bool isBoss,
                                              bool isMultiplayer,
                                              QVector<int> &usedFirstTwoIndices);
};

#endif // ENCOUNTERMANAGER_H
