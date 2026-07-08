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

QString AttackCard::getDynamicDescription(Player *player, Enemy *target) const
{
    int finalDamage = m_damage;

    if (player)
        finalDamage = player->calculateOutgoingDamage(m_damage);

    if (target) {
        int vulnStacks = target->effectStacks(BuffDebuffType::Vulnerable);
        if (vulnStacks > 0)
            finalDamage = static_cast<int>(finalDamage * 1.5);
    }

    return m_description.arg(finalDamage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Strike::Strike(QGraphicsItem *parent)
    : AttackCard("Strike", 1, "Deal %1 damage", 6, true, false, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Attack/strike_ironclad.png";
    loadPixmap();
}

void Strike::applyEffect(Player *player, Enemy *target)
{
    if (target && player)
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));
}

void Strike::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_damage += 3;
    m_name = "Strike+";
}

Card *Strike::clone() const
{
    Strike *copy = new Strike();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Bludgeon::Bludgeon(QGraphicsItem *parent)
    : AttackCard("Bludgeon", 3, "Deal %1 damage", 32, true, false, true, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Attack/bludgeon.png";
    loadPixmap();
}

void Bludgeon::applyEffect(Player *player, Enemy *target)
{
    if (target && player)
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));
}

void Bludgeon::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_damage += 10;
    m_name = "Bludgeon+";
}

Card *Bludgeon::clone() const
{
    Bludgeon *copy = new Bludgeon();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Reaper::Reaper(QGraphicsItem *parent)
    : AttackCard("Reaper",
                 2,
                 "Deal %1 damage to all enemies\nHeal HP equal to unblocked damage\nExhaust",
                 4,
                 false,
                 true,
                 true,
                 parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Attack/Red-Reaper-Art.png";
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

void Reaper::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_damage += 1;
    m_name = "Reaper+";
}

Card *Reaper::clone() const
{
    Reaper *copy = new Reaper();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Feed::Feed(QGraphicsItem *parent)
    : AttackCard("Feed",
                 1,
                 "Deal %1 damage\nIf fatal, raise max HP by %2\nExhaust",
                 10,
                 true,
                 true,
                 true,
                 parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Attack/feed.png";
    loadPixmap();
}

void Feed::applyEffect(Player *player, Enemy *target)
{
    if (target && player) {
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));

        if (target->currentHP() <= 0)
            player->addMaxHp(m_increaseMaxHp);
    }
}

void Feed::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_damage += 2;
    m_increaseMaxHp += 1;
}

Card *Feed::clone() const
{
    Feed *copy = new Feed();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

QString Feed::getDynamicDescription(Player *player, Enemy *target) const
{
    int finalDamage = m_damage;

    if (player)
        finalDamage = player->calculateOutgoingDamage(m_damage);

    if (target) {
        int vulnStacks = target->effectStacks(BuffDebuffType::Vulnerable);
        if (vulnStacks > 0)
            finalDamage = static_cast<int>(finalDamage * 1.5);
    }

    int maxHpFinalIncrease = m_increaseMaxHp;

    return m_description.arg(finalDamage, maxHpFinalIncrease);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Immolate::Immolate(QGraphicsItem *parent)
    : AttackCard("Immolate",
                 2,
                 "Deal %1 damage to all enemies\nAdd 2 BURN into discard pile",
                 21,
                 false,
                 false,
                 true,
                 parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Attack/Red-Immolate-Art.png";
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
    // gameplay->addCardToDiscardPile(new BURN());
    // gameplay->addCardToDiscardPile(new BURN());
    return true;
}

void Immolate::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_damage += 7;
    m_name = "Immolate+";
}

Card *Immolate::clone() const
{
    Immolate *copy = new Immolate();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Bash::Bash(QGraphicsItem *parent)
    : AttackCard("Bash", 2, "Deal %1 damage\nApply %2 Vulnerable", 8, true, false, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Attack/bash.png";
    loadPixmap();
}

void Bash::applyEffect(Player *player, Enemy *target)
{
    if (target && player) {
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));
        target->applyBuffDebuff(BuffDebuffType::Vulnerable, m_vulnerableValue);
    }
}

void Bash::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_damage += 2;
    m_vulnerableValue += 1;

    m_name = "Bash+";
}

Card *Bash::clone() const
{
    Bash *copy = new Bash();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

QString Bash::getDynamicDescription(Player *player, Enemy *target) const
{
    int finalDamage = m_damage;

    if (player)
        finalDamage = player->calculateOutgoingDamage(m_damage);

    if (target) {
        int vulnStacks = target->effectStacks(BuffDebuffType::Vulnerable);
        if (vulnStacks > 0)
            finalDamage = static_cast<int>(finalDamage * 1.5);
    }

    int finalVulnValue = m_vulnerableValue;

    return m_description.arg(finalDamage, finalVulnValue);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Clash::Clash(QGraphicsItem *parent)
    : AttackCard("Clash",
                 0,
                 "Deal %1 damage\nCan only be played if every card in hand is an attack",
                 14,
                 true,
                 false,
                 false,
                 parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Attack/clash.png";
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

void Clash::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_damage += 4;
    m_name = "Clash+";
}

Card *Clash::clone() const
{
    Clash *copy = new Clash();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Hemokinesis::Hemokinesis(QGraphicsItem *parent)
    : AttackCard("Hemokinesis", 1, "Lose 2 HP\nDeal %1 damage", 15, true, false, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Attack/hemokinesis.png";
    loadPixmap();
}

void Hemokinesis::applyEffect(Player *player, Enemy *target)
{
    if (player)
        player->loseHp(2);

    if (target && player)
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));
}

void Hemokinesis::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_damage += 5;
    m_name = "Hemokinesis+";
}

Card *Hemokinesis::clone() const
{
    Hemokinesis *copy = new Hemokinesis();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

BloodForBlood::BloodForBlood(QGraphicsItem *parent)
    : AttackCard("Blood for Blood",
                 4,
                 "Deal %1 damage\nCosts 1 less for every time you take unblocked damage",
                 18,
                 true,
                 false,
                 false,
                 parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Attack/BloodForBlood.png";
    loadPixmap();
}

void BloodForBlood::applyEffect(Player *player, Enemy *target)
{
    if (target && player)
        target->takeDamage(player->calculateOutgoingDamage(this->m_damage));
}

void BloodForBlood::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_energyCost -= 1;
    m_damage += 4;
    m_name = "Blood for Blood+";
}

// بقیش باید با سیگنال اسلات تو کلاس پلیر یا کمبتنت پیاده سازی بشه

Card *BloodForBlood::clone() const
{
    BloodForBlood *copy = new BloodForBlood();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Whirlwind::Whirlwind(QGraphicsItem *parent)
    : AttackCard("Whirlwind",
                 -1, //X
                 "Deal %1 damage to all enemies X times",
                 5,
                 false,
                 false,
                 false,
                 parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Attack/Whirlwind.png";
    loadPixmap();
}

void Whirlwind::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(player);
    Q_UNUSED(target);
}

bool Whirlwind::applyEffect(GamePlay *gameplay)
{
    if (gameplay && gameplay->player()) {
        int xCount = gameplay->player()->energy();

        for (int i = 0; i < xCount; ++i)
            gameplay->takeDamageToAllEnemies(m_damage);

        gameplay->player()->setEnergy(0);
    }

    return true;
}

void Whirlwind::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_damage += 3;
    m_name = "Whrilwind+";
}

Card *Whirlwind::clone() const
{
    Whirlwind *copy = new Whirlwind();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}