#ifndef ATTACKCARDS_H
#define ATTACKCARDS_H

#include "card.h"

class AttackCard : public Card {
protected:
    int m_damage;

public:
    AttackCard(QString name, int energyCost, QString path, QString description, int damage, bool requiresTarget, bool isExhaust, bool isRare, QGraphicsItem *parent);
    virtual ~AttackCard() = default;
};

class Strike : public AttackCard {
public:
    explicit Strike(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class Bludgeon : public AttackCard {
public:
    explicit Bludgeon(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class Reaper : public AttackCard {
public:
    explicit Reaper(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class Feed : public AttackCard {
public:
    explicit Feed(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class Immolate : public AttackCard {
public:
    explicit Immolate(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class Bash : public AttackCard {
public:
    explicit Bash(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class Clash : public AttackCard {
public:
    explicit Clash(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;

    //bool canPlay(Player* player) const;
};

class Hemokinesis : public AttackCard {
public:
    explicit Hemokinesis(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

#endif // ATTACKCARDS_H
