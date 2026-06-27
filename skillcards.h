#ifndef SKILLCARDS_H
#define SKILLCARDS_H

#include "card.h"

class SkillCard : public Card
{
public:
    explicit SkillCard(QString name, int energyCost, QString path, bool isRare = false, QGraphicsItem *parent = nullptr);
    virtual ~SkillCard();
};

class Defend : public SkillCard
{
public:
    explicit Defend(QString path, QGraphicsItem *parent = nullptr);

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

#endif // SKILLCARDS_H
