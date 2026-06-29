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

class Slime : public Enemy {
public:
    Slime(QString name, int minHP, int maxHP, bool isMultiplayer = false, QGraphicsItem* parent = nullptr);

protected:
    bool m_hasSplit = false;
    virtual bool shouldSplit() const;
};

class AcidSlimeS : public Slime {
public:
    explicit AcidSlimeS(bool isMultiplayer = false, QGraphicsItem* parent = nullptr);

    void calculateNextIntent() override;
};

class AcidSlimeM : public Slime {
public:
    explicit AcidSlimeM(bool isMultiplayer = false, QGraphicsItem* parent = nullptr);

    void calculateNextIntent() override;
};

class AcidSlimeL : public Slime {
public:
    explicit AcidSlimeL(bool isMultiplayer = false, QGraphicsItem* parent = nullptr);

    void calculateNextIntent() override;

    bool shouldSplit() const override;
};


#endif // NORMALENEMIES_H
