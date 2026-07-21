#include "encountermanager.h"
#include "allenemies.h"
#include "enemy.h"

#include <QRandomGenerator>
#include <QtGlobal>
#include <random>

QVector<Enemy *> EncounterManager::generateEncounter(int act,
                                                     int floor,
                                                     int nodePos,
                                                     bool isElite,
                                                     bool isBoss,
                                                     bool isMultiplayer,
                                                     unsigned int mapSeed,
                                                     QVector<int> &usedFirstTwoIndices)
{
    unsigned int derivedSeed = deriveCombatSeed(mapSeed, floor, nodePos);
    std::mt19937 localRng(derivedSeed);
    ScopedEnemyRng scopedRng(&localRng);

    QVector<Enemy *> result;

    // ---------------- Boss ----------------
    if (isBoss) {
        if (act == 1) {
            int r = localRng() % 2;
            if (r == 0)
                result.append(new KingSlime(isMultiplayer));
            else
                result.append(new HexaGhost(isMultiplayer));
        } else {
            result.append(new TheChamp(isMultiplayer));
        }
        return result;
    }

    // ---------------- Elite ----------------
    if (isElite) {
        if (act == 1) {
            int r = localRng() % 2;
            if (r == 0) {
                result.append(new GremlinKnob(isMultiplayer));
            } else {
                result.append(new Sentry(false, isMultiplayer));
                result.append(new Sentry(true, isMultiplayer));
                result.append(new Sentry(false, isMultiplayer));
            }
        } else {
            int r = localRng() % 2;
            if (r == 0) {
                result.append(new BookOfStabbing(isMultiplayer));
            } else {
                result.append(new Taskmaster(isMultiplayer));
                result.append(new BlueSlaver(isMultiplayer));
                result.append(new RedSlaver(isMultiplayer));
            }
        }
        return result;
    }

    // ---------------- Normal enemy ----------------
    if (act == 1) {
        if (usedFirstTwoIndices.size() < 2) {
            QVector<int> pool = {0, 1, 2, 3};
            for (int used : usedFirstTwoIndices)
                pool.removeAll(used);

            int idx = pool[localRng() % static_cast<unsigned int>(pool.size())];
            usedFirstTwoIndices.append(idx);

            switch (idx) {
            case 0:
                result.append(new Cultist(isMultiplayer));
                break;
            case 1:
                result.append(new JawWorm(isMultiplayer));
                break;
            case 2:
                result.append(new Louse(isMultiplayer));
                result.append(new Louse(isMultiplayer));
                break;
            case 3:
                result.append(new AcidSlimeS(isMultiplayer));
                result.append(new AcidSlimeS(isMultiplayer));
                break;
            }
        } else {
            int idx = localRng() % 4;
            switch (idx) {
            case 0:
                result.append(new BlueSlaver(isMultiplayer));
                break;
            case 1:
                result.append(new RedSlaver(isMultiplayer));
                break;
            case 2:
                for (int i = 0; i < 3; ++i)
                    result.append(new Louse(isMultiplayer));
                break;
            case 3:
                result.append(new AcidSlimeL(isMultiplayer));
                break;
            }
        }
    } else // act == 2
    {
        if (usedFirstTwoIndices.size() < 2) {
            QVector<int> pool = {0, 1, 2, 3};
            for (int used : usedFirstTwoIndices)
                pool.removeAll(used);

            int idx = pool[localRng() % static_cast<unsigned int>(pool.size())];
            usedFirstTwoIndices.append(idx);

            switch (idx) {
            case 0:
                result.append(new SphericGuardian(isMultiplayer));
                break;
            case 1:
                for (int i = 0; i < 3; ++i)
                    result.append(new Cultist(isMultiplayer));
                break;
            case 2:
                result.append(new Looter(isMultiplayer));
                result.append(new Mugger(isMultiplayer));
                break;
            case 3:
                result.append(new Sentry(true, isMultiplayer));
                result.append(new SphericGuardian(isMultiplayer));
                break;
            }
        } else {
            int idx = localRng() % 2;
            if (idx == 0) {
                for (int i = 0; i < 3; ++i)
                    result.append(new AcidSlimeL(isMultiplayer));
            } else {
                result.append(new GremlinKnob(isMultiplayer));
            }
        }
    }

    return result;
}