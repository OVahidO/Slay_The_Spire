#include "attackcards.h"

AttackCard::AttackCard(QString name, CardType type, int energyCost, QString path, int damage, bool requiresTarget, bool isRare, QGraphicsItem *parent, QString description)
    : Card(name, type, energyCost, path, isRare, requiresTarget, parent, description), m_damage(damage) {}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Strike::Strike(QGraphicsItem *parent)
    : AttackCard("Strike",
             CardType::Attack,
             1,
             ":/cards/strike.png",
             6,
             true,
             false,
             parent,
             "Deal 6 damage")
{}

void Strike::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(player);

    // if (target)
    //     target->takeDamage(this->m_damage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Bludgeon::Bludgeon(QGraphicsItem *parent)
    : AttackCard("Bludgeon",
             CardType::Attack,
             3,
             ":/cards/bludgeon.png",
             32,
             true,
             true,
             parent,
             "Deal 32 damage")
{}

void Bludgeon::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(player);

    // if (target)
    //     target->takeDamage(this->m_damage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Reaper::Reaper(QGraphicsItem *parent)
    : AttackCard("Reaper",
             CardType::Attack,
             2,
             ":/cards/reaper.png",
             4,
             false,
             true,
             parent,
             "Deal 4 damage to all enemies\nHeal HP equal to unblocked damage\nExhaust")
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
    : AttackCard("Feed",
             CardType::Attack,
             1,
             ":/cards/feed.png",
             10,
             true,
             true,
             parent,
             "Deal 10 damage\nIf fatal, raise max HP by 3\nExhaust")
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
    : AttackCard("Immolate",
             CardType::Attack,
             2,
             ":/cards/immolate.png",
             21,
             false,
             true,
             parent,
             "Deal 21 damage to all enemies\nAdd 2 BURN into discard pile")
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

Bash::Bash(QGraphicsItem *parent)
    : AttackCard("Bash",
             CardType::Attack,
             2,
             ":/cards/bash.png",
             8,
             true,
             false,
             parent,
             "Deal 8 damage\nApply 2 Vulnerable")
{}

void Bash::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(player);

    // if (target) {
    //     target->takeDamage(this->m_damage);

    //     target->applyVulnerable(2);
    // }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Clash::Clash(QGraphicsItem *parent)
    : AttackCard("Clash",
             CardType::Attack,
             0,
             ":/cards/clash.png",
             14,
             true,
             false,
             parent,
             "Deal 14 damage\nCan only be played if every card in hand is an attack")
{}

void Clash::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(player);

    // if (target)
    //     target->takeDamage(this->m_damage);
}

// bool Clash::canPlay(Player* player) const {
//     if (!player) return false;

//     QList<Card*> hand = player->getHand();

//     for (Card* card : hand)
//         if (card && card->getType() != CardType::Attack)
//             return false;

//     return true;
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Hemokinesis::Hemokinesis(QGraphicsItem *parent)
    : AttackCard("Hemokinesis",
             CardType::Attack,
             1,
             ":/cards/hemokinesis.png",
             15,
             true,
             false,
             parent,
             "Lose 2 HP\nDeal 15 damage")
{}

void Hemokinesis::applyEffect(Enemy* target, Player* player) {
    // if (player)
    //     player->loseHP(2);

    // if (target)
    //     target->takeDamage(this->m_damage);
}