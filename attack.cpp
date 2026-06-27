#include "attack.h"

Attack::Attack(QString name, CardType type, int energyCost, QString path, int damage, bool requiresTarget, bool isRare, QGraphicsItem *parent)
    : Card(name, type, energyCost, path, isRare, requiresTarget, parent), m_damage(damage) {
}



void Strike::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(player);

    if (target)
        target->takeDamage(this->m_damage);
}