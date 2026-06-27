#ifndef ATTACK_H
#define ATTACK_H

#include "card.h"

class Enemy;
class Player;

class Attack : public Card {
private:
    int m_damage;

public:
    Attack(QString name, CardType type, int energyCost, QString path, int damage, bool requiresTarget, bool isRare, QGraphicsItem *parent);

    virtual void applyEffect(Enemy* target, Player* player) = 0;
};
#endif // ATTACK_H
