#include "attackcards.h"
#include "enemy.h"
#include "gameplay.h"
#include "player.h"
#include "statuscards.h"

AttackCard::AttackCard(QString name,
                       int energyCost,
                       QString description,
                       int damage,
                       bool requiresTarget,
                       bool isExhaust,
                       bool isRare,
                       QGraphicsItem *parent)
    : Card(name, CardType::Attack, energyCost, description, isRare, isExhaust, requiresTarget, parent)
    , m_damage(damage)
{}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Strike::Strike(QGraphicsItem *parent)
    : AttackCard("Strike", 1, "Deal 6 damage", 6, true, false, false, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Attack/Strike_R.png";
    loadPixmap();
}

void Strike::applyEffect(Player *player, Enemy *target)
{
    if (target && player)
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Bludgeon::Bludgeon(QGraphicsItem *parent)
    : AttackCard("Bludgeon", 3, "Deal 32 damage", 32, true, false, true, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Attack/Bludgeon.png";
    loadPixmap();
}

void Bludgeon::applyEffect(Player *player, Enemy *target)
{
    if (target && player)
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Reaper::Reaper(QGraphicsItem *parent)
    : AttackCard("Reaper",
                 2,
                 "Deal 4 damage to all enemies\nHeal HP equal to unblocked damage\nExhaust",
                 4,
                 false,
                 true,
                 true,
                 parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Attack/Reaper.png";
    loadPixmap();
}

void Reaper::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(player);
    Q_UNUSED(target);
}
bool Reaper::applyEffect(GamePlay *gameplay)
{
    gameplay->player()->heal(gameplay->takeDamageToAllEnemies(m_damage));
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Feed::Feed(QGraphicsItem *parent)
    : AttackCard("Feed",
                 1,
                 "Deal 10 damage\nIf fatal, raise max HP by 3\nExhaust",
                 10,
                 true,
                 true,
                 true,
                 parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Attack/Feed.png";
    loadPixmap();
}

void Feed::applyEffect(Player *player, Enemy *target)
{
    if (target && player) {
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));

        if (target->currentHP() <= 0)
            player->addMaxHp(3);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Immolate::Immolate(QGraphicsItem *parent)
    : AttackCard("Immolate",
                 2,
                 "Deal 21 damage to all enemies\nAdd 2 BURN into discard pile",
                 21,
                 false,
                 false,
                 true,
                 parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Attack/Immolate.png";
    loadPixmap();
}

void Immolate::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(player);
    Q_UNUSED(target);
}
bool Immolate::applyEffect(GamePlay *gameplay)
{
    gameplay->takeDamageToAllEnemies(m_damage);
    gameplay->addCardToDiscardPile(new BURN());
    gameplay->addCardToDiscardPile(new BURN());
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Bash::Bash(QGraphicsItem *parent)
    : AttackCard("Bash", 2, "Deal 8 damage\nApply 2 Vulnerable", 8, true, false, false, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Attack/Bash.png";
    loadPixmap();
}

void Bash::applyEffect(Player *player, Enemy *target)
{
    if (target && player) {
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));
        target->applyBuffDebuff(BuffDebuffType::Vulnerable, 2);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Clash::Clash(QGraphicsItem *parent)
    : AttackCard("Clash",
                 0,
                 "Deal 14 damage\nCan only be played if every card in hand is an attack",
                 14,
                 true,
                 false,
                 false,
                 parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Attack/Clash.png";
    loadPixmap();
}

void Clash::applyEffect(Player *player, Enemy *target)
{
    bool isAllHandsCardsAttack = true;

    for (Card *card : player->HandsCards())
        if (card->cardType() != (CardType::Attack)) {
            isAllHandsCardsAttack = false;
            break;
        }

    if (isAllHandsCardsAttack) {
        if (target && player)
            target->takeDamage(player->calculateOutgoingDamage(this->m_damage));
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Hemokinesis::Hemokinesis(QGraphicsItem *parent)
    : AttackCard("Hemokinesis", 1, "Lose 2 HP\nDeal 15 damage", 15, true, false, false, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Attack/Hemokinesis.png";
    loadPixmap();
}

void Hemokinesis::applyEffect(Player *player, Enemy *target)
{
    if (player)
        player->loseHp(2);

    if (target && player)
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));
}