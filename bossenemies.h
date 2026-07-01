#ifndef BOSSENEMIES_H
#define BOSSENEMIES_H

#include "normalenemies.h"

class KingSlime : public Slime
{
public:
    explicit KingSlime(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);

    void calculateNextIntent() override;
    bool shouldSplit() const override;
};

class HexaGhost : public Enemy
{
public:
    explicit HexaGhost(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);

    void calculateNextIntent() override;
};

#endif // BOSSENEMIES_H
