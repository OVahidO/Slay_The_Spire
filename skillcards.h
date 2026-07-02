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
};

class Exhume : public SkillCard
{
public:
    explicit Exhume(QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr) override;
    bool applyEffect(GamePlay* gameplay) override;
};

class Limit_Break : public SkillCard
{
public:
    explicit Limit_Break(QGraphicsItem *parent = nullptr);

    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
};

class Offering : public SkillCard
{
public:
    explicit Offering(QGraphicsItem *parent = nullptr);

<<<<<<< HEAD
    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr) override;
    bool applyEffect(GamePlay* gameplay) override;
=======
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
>>>>>>> main
};

class Impervious : public SkillCard
{
public:
    explicit Impervious(QGraphicsItem *parent = nullptr);

    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
};

class Power_Through : public SkillCard
{
public:
    explicit Power_Through(QGraphicsItem *parent = nullptr);

<<<<<<< HEAD
    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr) override;
    bool applyEffect(GamePlay* gameplay) override;
=======
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
>>>>>>> main
};

class Bloodletting : public SkillCard
{
public:
    explicit Bloodletting(QGraphicsItem *parent = nullptr);

    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
};

#endif // SKILLCARDS_H
