#ifndef STATUSCARDS_H
#define STATUSCARDS_H

#include "card.h"

class StatusCard : public Card
{
public:
    explicit StatusCard(QString name, int energyCost, QString path, QString description, bool isRare = false, bool requiresTarget = true, QGraphicsItem *parent = nullptr);
    virtual ~StatusCard();
};

class DAZE : public StatusCard
{
public:
    explicit DAZE(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr) override;
};

class SLIME : public StatusCard
{
public:
    explicit SLIME(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr) override;
};

class WOUND : public StatusCard
{
public:
    explicit WOUND(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr) override;
};

class BURN : public StatusCard
{
public:
    explicit BURN(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr) override;
};

#endif // STATUSCARDS_H
