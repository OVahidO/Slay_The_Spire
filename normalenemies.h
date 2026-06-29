#ifndef NORMALENEMIES_H
#define NORMALENEMIES_H

#include "enemy.h"

class Cultist : public Enemy {
public:
    explicit Cultist(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);

    void calculateNextIntent() override;

private:
    bool m_hasIncantation = false;
};

class JawWorm : public Enemy {
public:
    explicit JawWorm(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);

    void calculateNextIntent() override;
};

#endif // NORMALENEMIES_H
