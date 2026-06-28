#ifndef ATTACK_H
#define ATTACK_H

#include "card.h"

class Attack : public Card {
protected:
    int m_damage;

public:
    Attack(QString name, CardType type, int energyCost, QString path, int damage, bool requiresTarget, bool isRare, QGraphicsItem *parent);
};

class Strike : public Attack {
public:
    explicit Strike(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Bludgeon : public Attack {
public:
    explicit Bludgeon(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Reaper : public Attack {
public:
    explicit Reaper(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Feed : public Attack {
public:
    explicit Feed(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Immolate : public Attack {
public:
    explicit Immolate(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Bash : public Attack {
public:
    explicit Bash(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

#endif // ATTACK_H