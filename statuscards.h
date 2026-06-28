#ifndef STATUSCARDS_H
#define STATUSCARDS_H

#include "card.h"

class StatusCard : public Card
{
public:
    explicit StatusCard(QString name, int energyCost, QString path, bool isRare = false, QGraphicsItem *parent = nullptr);
    virtual ~StatusCard();
};

class DAZE : public StatusCard
{
public:
    explicit DAZE(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

class SLIME : public StatusCard
{
public:
    explicit SLIME(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

class WOUND : public StatusCard
{
public:
    explicit WOUND(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

#endif // STATUSCARDS_H
