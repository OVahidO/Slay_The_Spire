#ifndef POWERCARDS_H
#define POWERCARDS_H

#include <QObject>
#include <QWidget>
#include "card.h"

class PowerCard : public Card
{
public:
    PowerCard(CardID ID,
              QString name,
              int energyCost,
              QString description,
              bool requiresTarget,
              bool isExhaust,
              bool isRare,
              QGraphicsItem *parent);
    virtual ~PowerCard() = default;
    QString getDynamicDescription(Player *player, Enemy *target) const override;
};

class Inflame : public PowerCard
{
public:
    explicit Inflame(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;

private:
    int m_strengthValue = 2;
};

class Brutality : public PowerCard
{
public:
    explicit Brutality(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;
};

class DemonForm : public PowerCard
{
public:
    explicit DemonForm(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;

private:
    int m_strengthPerTurn = 3;
};

class Metallicize : public PowerCard
{
public:
    explicit Metallicize(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;

private:
    int m_blockPerTurn = 3;
};

class Berserk : public PowerCard
{
public:
    explicit Berserk(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;

private:
    int m_vulnerableAmount = 2;
};

class DarkEmbrace : public PowerCard
{
public:
    explicit DarkEmbrace(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;
};

class Barricade : public PowerCard
{
public:
    explicit Barricade(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;
};

class FeelNoPain : public PowerCard
{
public:
    explicit FeelNoPain(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;

private:
    int m_blockAmount = 3;
};

#endif // POWERCARDS_H