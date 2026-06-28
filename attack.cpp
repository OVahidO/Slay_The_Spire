#include "attack.h"

Attack::Attack(QString name, CardType type, int energyCost, QString path, int damage, bool requiresTarget, bool isRare, QGraphicsItem *parent)
    : Card(name, type, energyCost, path, isRare, requiresTarget, parent), m_damage(damage) {}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Strike::Strike(QGraphicsItem *parent)
    : Attack("Strike",
             CardType::Attack,
             1,
             ":/cards/strike.png",
             6,
             true,
             false,
             parent)
{}

void Strike::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(player);

    if (target)
        target->takeDamage(this->m_damage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Bludgeon::Bludgeon(QGraphicsItem *parent)
    : Attack("Bludgeon",
             CardType::Attack,
             3,
             ":/cards/bludgeon.png",
             32,
             true,
             true,
             parent)
{}

void Bludgeon::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(player);

    if (target)
        target->takeDamage(this->m_damage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Reaper::Reaper(QGraphicsItem *parent)
    : Attack("Reaper",
             CardType::Attack,
             2,
             ":/cards/reaper.png",
             4,
             false,
             true,
             parent)
{}

// void Reaper::applyEffect(Enemy* target, Player* player) {
//     Q_UNUSED(target);

//     int totalDamageDealt = 0;

//     QList<Enemy*> allEnemies = CombatManager::getInstance()->getEnemies();

//     for (Enemy* enemy : allEnemies)
//         if (enemy)
//             totalDamageDealt += enemy->takeDamage(this->m_damage);

//     if (player)
//         player->heal(totalDamageDealt);

//     player->exhaustCard(this);
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Feed::Feed(QGraphicsItem *parent)
    : Attack("Feed",
             CardType::Attack,
             1,
             ":/cards/feed.png",
             10,
             true,
             true,
             parent)
{}

// void Feed::Feed::applyEffect(Enemy* target, Player* player) {
//     if (target && player) {
//         int damageDealt = target->takeDamage(this->m_damage);

//         if (target->getCurrentHP() <= 0) {
//             player->increaseMaxHP(4);
//         }
//     }

//     player->exhaustCard(this);
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Immolate::Immolate(QGraphicsItem *parent)
    : Attack("Immolate",
             CardType::Attack,
             2,
             ":/cards/immolate.png",
             21,
             false,
             true,
             parent)
{}

// void Immolate::applyEffect(Enemy* target, Player* player) {

//     QList<Enemy*> allEnemies = CombatManager::getInstance()->getEnemies();

//     for (Enemy* enemy : allEnemies)
//         if (enemy && enemy->getCurrentHP() > 0)
//             enemy->takeDamage(this->m_damage);

//     if (player) {
//         player->addCardToDiscard(new Burn());
//         player->addCardToDiscard(new Burn());
//     }
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////

