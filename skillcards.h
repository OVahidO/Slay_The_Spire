#ifndef SKILLCARDS_H
#define SKILLCARDS_H

#include "card.h"

class SkillCard : public Card
{
public:
    explicit SkillCard(QString name,
                       int energyCost,
                       QString description,
                       bool isRare = false,
                       bool isExhaust = false,
                       bool requiresTarget = true,
                       QGraphicsItem *parent = nullptr);
    virtual ~SkillCard();
};

class Defend : public SkillCard
{
public:
    explicit Defend(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
    void upgrade() override;

private:
    int m_blockAmount = 5;
};

class Exhume : public SkillCard
{
public:
    explicit Exhume(QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr) override;
    bool applyEffect(GamePlay* gameplay) override;
    void upgrade() override;
};

class Limit_Break : public SkillCard
{
public:
    explicit Limit_Break(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
    void upgrade() override;
};

class Offering : public SkillCard
{
public:
    explicit Offering(QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr) override;
    bool applyEffect(GamePlay* gameplay) override;
    void upgrade() override;

private:
    int m_drawCount = 3;
};

class Impervious : public SkillCard
{
public:
    explicit Impervious(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
    void upgrade() override;

private:
    int m_blockAmount = 30;
};

class Power_Through : public SkillCard
{
public:
    explicit Power_Through(QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr) override;
    bool applyEffect(GamePlay* gameplay) override;
    void upgrade() override;

private:
    int m_blockAmount = 15;
};

class Bloodletting : public SkillCard
{
public:
    explicit Bloodletting(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
    void upgrade() override;

private:
    int m_energyGain = 2;
};

#endif // SKILLCARDS_H
