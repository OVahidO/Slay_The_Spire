#ifndef SKILLCARDS_H
#define SKILLCARDS_H

#include "card.h"

class SkillCard : public Card
{
public:
    explicit SkillCard(QString name, int energyCost, QString path, QString description, bool isRare = false, bool requiresTarget = true, QGraphicsItem *parent = nullptr);
    virtual ~SkillCard();
};

class Defend : public SkillCard
{
public:
    explicit Defend(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

class Exhume : public SkillCard
{
    explicit Exhume(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

class Limit_Break : public SkillCard
{
public:
    explicit Limit_Break(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

class Offering : public SkillCard
{
public:
    explicit Offering(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

class Impervious : public SkillCard
{
public:
    explicit Impervious(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

class Power_Through : public SkillCard
{
public:
    explicit Power_Through(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

class Bloodletting : public SkillCard
{
public:
    explicit Bloodletting(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

#endif // SKILLCARDS_H
