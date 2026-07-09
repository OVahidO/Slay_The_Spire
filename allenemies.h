#ifndef ALLENEMIES_H
#define ALLENEMIES_H

#include <QGraphicsObject>
#include "enemy.h"

class Cultist : public Enemy
{
public:
    explicit Cultist(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;

private:
    bool m_hasIncantation = false;
};

class JawWorm : public Enemy
{
public:
    explicit JawWorm(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;
};

class Louse : public Enemy
{
public:
    explicit Louse(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;
    int takeDamage(int incomingDamage, bool isAttackDamage = true) override;

private:
    bool m_isRed;
    bool m_defensiveReactionUsed = false;

    int randomBiteDamage() const;
};

class Slime : public Enemy
{
public:
    Slime(QString name,
          int minHP,
          int maxHP,
          enemyType type,
          bool isMultiplayer = false,
          QGraphicsItem *parent = nullptr);

protected:
    bool m_hasSplit = false;
    virtual bool shouldSplit() const;
};

class AcidSlimeS : public Slime
{
public:
    explicit AcidSlimeS(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;
};

class AcidSlimeM : public Slime
{
public:
    explicit AcidSlimeM(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;
};

class AcidSlimeL : public Slime
{
public:
    explicit AcidSlimeL(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;
    bool shouldSplit() const override;
};

class Thief : public Enemy
{
public:
    Thief(QString name,
          int minHP,
          int maxHP,
          int mugDamage,
          bool isMultiplayer = false,
          QGraphicsItem *parent = nullptr);

    void calculateNextIntent() override;
    bool hasEscaped() const;
    int stolenGold() const;

protected:
    int m_mugDamage;
    int m_stolenGold = 0;
    bool m_hasEscaped = false;
};

class Looter : public Thief
{
public:
    explicit Looter(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
};

class Mugger : public Thief
{
public:
    explicit Mugger(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
};

class BlueSlaver : public Enemy
{
public:
    explicit BlueSlaver(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;
};

class RedSlaver : public Enemy
{
public:
    explicit RedSlaver(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;

private:
    bool m_usedEntangle = false;
};

class SphericGuardian : public Enemy
{
public:
    explicit SphericGuardian(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;
};

class GremlinKnob : public Enemy
{
public:
    explicit GremlinKnob(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;
    void onPlayerSkillPlayed();

private:
    int m_enrageStacks = 0;
};

class Sentry : public Enemy
{
public:
    explicit Sentry(bool startsWithBeam,
                    bool isMultiplayer = false,
                    QGraphicsItem *parent = nullptr);

    void calculateNextIntent() override;

private:
    bool m_startsWithBeam;
};

class BookOfStabbing : public Enemy
{
public:
    explicit BookOfStabbing(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;

private:
    int m_multiStabUsedCount = 0;
};

class Taskmaster : public Enemy
{
public:
    explicit Taskmaster(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;
};

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

class TheChamp : public Enemy
{
public:
    explicit TheChamp(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    void calculateNextIntent() override;
};

#endif // ALLENEMIES_H
