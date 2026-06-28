#ifndef CURSECARDS_H
#define CURSECARDS_H

#include "card.h"

class CurseCard : public Card
{
public:
    explicit CurseCard(QString name, int energyCost, QString path, bool isRare = false, QGraphicsItem *parent = nullptr);
    virtual ~CurseCard();
};

class J_A_X : public CurseCard
{
public:
    explicit J_A_X(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

#endif // CURSECARDS_H
