#include "attackcards.h"
#include "enemy.h"
#include "player.h"
#include "gameplay.h"
#include "statuscards.h"

AttackCard::AttackCard(QString name, int energyCost, QString path, QString description, int damage, bool requiresTarget, bool isExhaust, bool isRare, QGraphicsItem *parent)
    : Card(name, CardType::Attack, energyCost, path, description, isRare, requiresTarget, parent), m_damage(damage) {}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Strike::Strike(QString path, QGraphicsItem *parent)
    : AttackCard("Strike", 1, path, "Deal 6 damage", 6, true, false, false, parent) {}

void Strike::applyEffect(Player* player, Enemy* target) {
    Q_UNUSED(player);

    if (target)
        target->takeDamage(this->m_damage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Bludgeon::Bludgeon(QString path, QGraphicsItem *parent)
    : AttackCard("Bludgeon", 3, path, "Deal 32 damage", 32, true, false, true, parent) {}

void Bludgeon::applyEffect(Player* player, Enemy* target) {
    Q_UNUSED(player);

    if (target)
        target->takeDamage(this->m_damage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Reaper::Reaper(QString path, QGraphicsItem *parent)
    : AttackCard("Reaper", 2, path, "Deal 4 damage to all enemies\nHeal HP equal to unblocked damage\nExhaust", 4, false, true, true, parent) {}

void Reaper::applyEffect(Player* player, Enemy* target) {Q_UNUSED(player); Q_UNUSED(target);}
bool Reaper::applyEffect(GamePlay* gameplay)
{
    gameplay->player()->heal(gameplay->takeDamageToAllEnemies(m_damage));
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Feed::Feed(QString path, QGraphicsItem *parent)
    : AttackCard("Feed", 1, path, "Deal 10 damage\nIf fatal, raise max HP by 3\nExhaust", 10, true, true, true, parent) {}

void Feed::applyEffect(Player* player, Enemy* target) {
    if (target && player) {
        target->takeDamage(this->m_damage);

        if (target->getCurrentHP() <= 0) {
            player->addMaxHp(3);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Immolate::Immolate(QString path, QGraphicsItem *parent)
    : AttackCard("Immolate", 2, path, "Deal 21 damage to all enemies\nAdd 2 BURN into discard pile", 21, false, false, true, parent) {}

void Immolate::applyEffect(Player* player, Enemy* target) {Q_UNUSED(player); Q_UNUSED(target);}
bool Immolate::applyEffect(GamePlay* gameplay)
{
    gameplay->takeDamageToAllEnemies(m_damage);
    gameplay->addCardToDiscardPile(new BURN(""));
    gameplay->addCardToDiscardPile(new BURN(""));
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Bash::Bash(QString path, QGraphicsItem *parent)
    : AttackCard("Bash", 2, path, "Deal 8 damage\nApply 2 Vulnerable", 8, true, false, false, parent) {}

void Bash::applyEffect(Player* player, Enemy* target) {
    Q_UNUSED(player);

    if (target) {
        target->takeDamage(this->m_damage);

        //target->applyVulnerable(2);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Clash::Clash(QString path, QGraphicsItem *parent)
    : AttackCard("Clash", 0, path, "Deal 14 damage\nCan only be played if every card in hand is an attack", 14, true, false, false, parent) {}

void Clash::applyEffect(Player* player, Enemy* target) {
    bool isAllHandsCardsAttack = true;

    for(Card* card : player->HandsCards())
    {
        if(card->cardType() != (CardType::Attack))
        {
            isAllHandsCardsAttack = false;
            break;
        }
    }

    if(isAllHandsCardsAttack)
    {
        if (target)
            target->takeDamage(this->m_damage);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Hemokinesis::Hemokinesis(QString path, QGraphicsItem *parent)
    : AttackCard("Hemokinesis", 1, path, "Lose 2 HP\nDeal 15 damage", 15, true, false, false, parent) {}

void Hemokinesis::applyEffect(Player* player, Enemy* target) {
    if (player)
        player->loseHp(2);

    if (target)
        target->takeDamage(this->m_damage);
}