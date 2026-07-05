#ifndef ATTACKCARDS_H
#define ATTACKCARDS_H

#include "card.h"

class AttackCard : public Card
{
protected:
    int m_damage;

public:
    AttackCard(QString name,
               int energyCost,
               QString description,
               int damage,
               bool requiresTarget,
               bool isExhaust,
               bool isRare,
               QGraphicsItem *parent);
    virtual ~AttackCard() = default;
};

class Strike : public AttackCard
{
public:
    explicit Strike(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
};

class Bludgeon : public AttackCard
{
public:
    explicit Bludgeon(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
};

class Reaper : public AttackCard
{
public:
    explicit Reaper(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    bool applyEffect(GamePlay *gameplay) override;
    void upgrade() override;
};

class Feed : public AttackCard
{
public:
    explicit Feed(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;

private:
    int m_increaseMaxHp = 3;
};

class Immolate : public AttackCard
{
public:
    explicit Immolate(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    bool applyEffect(GamePlay *gameplay) override;
    void upgrade() override;
};

class Bash : public AttackCard
{
public:
    explicit Bash(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;

private:
    int m_vulnerableValue = 2;
};

class Clash : public AttackCard
{
public:
    explicit Clash(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
};

class Hemokinesis : public AttackCard
{
public:
    explicit Hemokinesis(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
};

#endif // ATTACKCARDS_H
