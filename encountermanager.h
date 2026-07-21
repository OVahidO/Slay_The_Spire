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

    static unsigned int deriveCombatSeed(unsigned int mapSeed, int floor, int nodePos);
};

#endif // ENCOUNTERMANAGER_H
