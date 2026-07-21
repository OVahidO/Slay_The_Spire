#ifndef ENCOUNTERMANAGER_H
#define ENCOUNTERMANAGER_H

#include <QVector>

class Enemy;

class EncounterManager
{
public:
public:
    static QVector<Enemy *> generateEncounter(int act,
                                              int floor,
                                              int nodePos,
                                              bool isElite,
                                              bool isBoss,
                                              bool isMultiplayer,
                                              unsigned int mapSeed,
                                              QVector<int> &usedFirstTwoIndices);
};

#endif // ENCOUNTERMANAGER_H
