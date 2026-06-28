#ifndef ATTACKCARDS_H
#define ATTACKCARDS_H

#include "card.h"

class AttackCard : public Card {
protected:
    int m_damage;

public:
    AttackCard(QString name, CardType type, int energyCost, QString path, int damage, bool requiresTarget, bool isRare, QGraphicsItem *parent, QString description);
};

class Strike : public AttackCard {
public:
    explicit Strike(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Bludgeon : public AttackCard {
public:
    explicit Bludgeon(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Reaper : public AttackCard {
public:
    explicit Reaper(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Feed : public AttackCard {
public:
    explicit Feed(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Immolate : public AttackCard {
public:
    explicit Immolate(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Bash : public AttackCard {
public:
    explicit Bash(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Clash : public AttackCard {
public:
    explicit Clash(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;

    //bool canPlay(Player* player) const;
};

class Hemokinesis : public AttackCard {
public:
    explicit Hemokinesis(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

#endif // ATTACKCARDS_H
