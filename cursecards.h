#ifndef CURSECARDS_H
#define CURSECARDS_H

#include "card.h"

class CurseCard : public Card
{
public:
    explicit CurseCard(QString name,
                       int energyCost,
                       QString path,
                       QString description,
                       bool isRare = false,
                       bool isExhaust = false,
                       bool requiresTarget = true,
                       QGraphicsItem *parent = nullptr);
    virtual ~CurseCard();
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
