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

class Louse : public Enemy {
public:
    explicit Louse(bool isMultiplayer = false, QGraphicsItem* parent = nullptr);

    void calculateNextIntent() override;

    int takeDamage(int incomingDamage);

private:
    bool m_isRed;
    bool m_defensiveReactionUsed = false;

    int randomBiteDamage() const;
};

#endif // NORMALENEMIES_H
