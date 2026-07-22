#ifndef ATTACKCARDS_H
#define ATTACKCARDS_H

#include "card.h"

class AttackCard : public Card
{
protected:
    int m_damage;

public:
    AttackCard(CardID ID,
               QString name,
               int energyCost,
               QString description,
               int damage,
               bool requiresTarget,
               bool isExhaust,
               bool isRare,
               QGraphicsItem *parent);
    virtual ~AttackCard() = default;
    QString getDynamicDescription(Player *player, Enemy *target) const override;
    int damage() const;
};

class Strike : public AttackCard
{
public:
    explicit Strike(QGraphicsItem *parent = nullptr); 
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;
};

class Bludgeon : public AttackCard
{
public:
    explicit Bludgeon(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;
};

class Reaper : public AttackCard
{
public:
    explicit Reaper(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    bool applyEffect(GamePlay *gameplay) override;
    bool isAoeEnemyEffect() const override;
    void upgrade() override;
    Card *clone() const override;
};

class Feed : public AttackCard
{
public:
    explicit Feed(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;
    QString getDynamicDescription(Player *player, Enemy *target) const override;

private:
    int m_increaseMaxHp = 3;
};

class Immolate : public AttackCard
{
public:
    explicit Immolate(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    bool applyEffect(GamePlay *gameplay) override;
    bool isAoeEnemyEffect() const override;
    void upgrade() override;
    Card *clone() const override;
};

class Bash : public AttackCard
{
public:
    explicit Bash(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;
    QString getDynamicDescription(Player *player, Enemy *target) const override;

private:
    int m_vulnerableValue = 2;
};

class Clash : public AttackCard
{
public:
    explicit Clash(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;
};

class Hemokinesis : public AttackCard
{
public:
    explicit Hemokinesis(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;
};

class BloodForBlood : public AttackCard
{
public:
    explicit BloodForBlood(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    void upgrade() override;
    Card *clone() const override;
};

class Whirlwind : public AttackCard
{
public:
    explicit Whirlwind(QGraphicsItem *parent = nullptr);
    void applyEffect(Player *player, Enemy *target) override;
    bool applyEffect(GamePlay *gameplay) override;
    bool isAoeEnemyEffect() const override;
    void upgrade() override;
    Card *clone() const override;
};

#endif // ATTACKCARDS_H
