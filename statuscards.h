#ifndef STATUSCARDS_H
#define STATUSCARDS_H

#include "card.h"

class StatusCard : public Card
{
public:
    explicit StatusCard(CardID ID,
                        QString name,
                        int energyCost,
                        QString description,
                        bool isRare = false,
                        bool isExhaust = false,
                        bool requiresTarget = true,
                        QGraphicsItem *parent = nullptr);
    virtual ~StatusCard();
};

class DAZE : public StatusCard
{
public:
    explicit DAZE(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
    Card *clone() const override;
};

class SLIME : public StatusCard
{
public:
    explicit SLIME(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
    Card *clone() const override;
};

class WOUND : public StatusCard
{
public:
    explicit WOUND(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
    Card *clone() const override;
};

class BURN : public StatusCard
{
public:
    explicit BURN(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player = nullptr, Enemy *targetEnemy = nullptr) override;
    Card *clone() const override;
    void upgrade() override;
    int burnDamage() const;

private:
    int m_damage = 2;
};

#endif // STATUSCARDS_H
