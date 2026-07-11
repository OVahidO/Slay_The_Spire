#ifndef CURSECARDS_H
#define CURSECARDS_H

#include "card.h"

class CurseCard : public Card
{
public:
    explicit CurseCard(CardID ID,
                       QString name,
                       int energyCost,
                       QString description,
                       bool isRare = false,
                       bool isExhaust = false,
                       bool requiresTarget = true,
                       QGraphicsItem *parent = nullptr);
    virtual ~CurseCard() = default;
};

class J_A_X : public CurseCard
{
public:
    explicit J_A_X(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
};

class CurseOfTheBell : public CurseCard
{
public:
    explicit CurseOfTheBell(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
};

class Regret : public CurseCard
{
public:
    explicit Regret(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
};

#endif // CURSECARDS_H
