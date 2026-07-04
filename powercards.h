#ifndef POWERCARDS_H
#define POWERCARDS_H

#include <QObject>
#include <QWidget>
#include "card.h"

class PowerCard : public Card
{
public:
    PowerCard(QString name,
              int energyCost,
              QString description,
              bool requiresTarget,
              bool isExhaust,
              bool isRare,
              QGraphicsItem *parent);
    virtual ~PowerCard() = default;
};

class Inflame : public PowerCard
{
public:
    explicit Inflame(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
};

class Brutality : public PowerCard
{
public:
    explicit Brutality(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
};

class DemonForm : public PowerCard
{
public:
    explicit DemonForm(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
};

class Metallicize : public PowerCard
{
public:
    explicit Metallicize(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
};

class Berserk : public PowerCard
{
public:
    explicit Berserk(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
};

class DarkEmbrace : public PowerCard
{
public:
    explicit DarkEmbrace(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
};

#endif // POWERCARDS_H
