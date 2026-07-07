#include "skillcards.h"
#include "enemy.h"
#include "gameplay.h"
#include "player.h"
#include "statuscards.h"

SkillCard::SkillCard(QString name,
                     int energyCost,
                     QString description,
                     bool isRare,
                     bool isExhaust,
                     bool requiresTarget,
                     QGraphicsItem *parent)
    : Card(name, CardType::Skill, energyCost, description, isRare, isExhaust, requiresTarget, parent)
{}

SkillCard::~SkillCard() {}

Defend::Defend(QGraphicsItem *parent)
    : SkillCard("Defend", 1, "Gain 5 block", false, false, true, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Skill/Defend_R.png";
    loadPixmap();
}

void Defend::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr)
        player->addBlockFromCard(m_blockAmount);
}

void Defend::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_blockAmount += 3;

    m_sourcePath = "";
    loadPixmap();
}

Card *Defend::clone() const
{
    Defend *copy = new Defend();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

Exhume::Exhume(QGraphicsItem *parent)
    : SkillCard("Exhume", 1, "Put a card from exhaust pile into hand", true, true, false, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Skill/Exhume.png";
    loadPixmap();
}

void Exhume::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);
    Q_UNUSED(player);
}
bool Exhume::applyEffect(GamePlay *gameplay)
{
    gameplay->drawFromExhaustPile();
    return true;
}

void Exhume::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_energyCost -= 1;

    m_sourcePath = "";
    loadPixmap();
}

Card *Exhume::clone() const
{
    Exhume *copy = new Exhume();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

Limit_Break::Limit_Break(QGraphicsItem *parent)
    : SkillCard("Limit_Break", 1, "Double your Strength", true, true, true, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Skill/LimitBreak.png";
    loadPixmap();
}

void Limit_Break::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr) {
        int currentStrength = player->effectStacks(BuffDebuffType::Strength);
        player->applyBuffDebuff(BuffDebuffType::Strength, currentStrength);
    }
}

void Limit_Break::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_isExhaust = false;

    m_sourcePath = "";
    loadPixmap();
}

Card *Limit_Break::clone() const
{
    Limit_Break *copy = new Limit_Break();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

Offering::Offering(QGraphicsItem *parent)
    : SkillCard("Offering", 0, "Lose 6 HP\nGain 2 Energy\nDraw 3 cards", true, true, true, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Skill/Offering.png";
    loadPixmap();
}

void Offering::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr) {
        player->loseHp(6);
        player->addEnergy(2);
    }
}

bool Offering::applyEffect(GamePlay *gameplay)
{
    for (int i = 0; i < m_drawCount; i++)
        gameplay->drawFromDrawPile();

    return true;
}

void Offering::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_drawCount += 2;

    m_sourcePath = "";
    loadPixmap();
}

Card *Offering::clone() const
{
    Offering *copy = new Offering();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

Impervious::Impervious(QGraphicsItem *parent)
    : SkillCard("Impervious", 2, "Gain 30 block", true, true, true, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Skill/Impervious.png";
    loadPixmap();
}

void Impervious::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr)
        player->addBlockFromCard(m_blockAmount);
}

void Impervious::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_blockAmount += 10;

    m_sourcePath = "";
    loadPixmap();
}

Card *Impervious::clone() const
{
    Impervious *copy = new Impervious();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

Power_Through::Power_Through(QGraphicsItem *parent)
    : SkillCard("Power_Through",
                1,
                "Gain 15 block\nAdd 2 WOUNDs into hand",
                false,
                false,
                true,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Skill/PowerThrough.png";
    loadPixmap();
}

void Power_Through::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr)
        player->addBlockFromCard(m_blockAmount);
}

bool Power_Through::applyEffect(GamePlay* gameplay)
{
    gameplay->addCardToHand(new WOUND());
    gameplay->addCardToHand(new WOUND());
    return true;
}

void Power_Through::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_blockAmount += 5;

    m_sourcePath = "";
    loadPixmap();
}

Card *Power_Through::clone() const
{
    Power_Through *copy = new Power_Through();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

Bloodletting::Bloodletting(QGraphicsItem *parent)
    : SkillCard("Bloodletting", 0, "Lose 3 HP\nGain 2 Energy", false, false, true, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Skill/Bloodletting.png";
    loadPixmap();
}

void Bloodletting::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr) {
        player->loseHp(3);
        player->addEnergy(m_energyGain);
    }
}

void Bloodletting::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_energyGain += 1;

    m_sourcePath = "";
    loadPixmap();
}

Card *Bloodletting::clone() const
{
    Bloodletting *copy = new Bloodletting();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

Entrench::Entrench(QGraphicsItem *parent)
    : SkillCard("Entrench", 2, "Double your Block", false, false, false, parent)
{
    m_sourcePath = "";
    loadPixmap();
}

void Entrench::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr) {
        int currentBlock = player->block();
        player->addBlockFromCard(currentBlock);
    }
}

void Entrench::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_energyCost -= 1;
}

Card *Entrench::clone() const
{
    Entrench *copy = new Entrench();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

Dual_Wield::Dual_Wield(QGraphicsItem *parent)
    : SkillCard("Dual Wield",
                1,
                "Choose a card in hand\nAdd 2 copies into hand",
                false,
                false,
                false,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Skill/DualWield.png";
    loadPixmap();
}

void Dual_Wield::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

bool Dual_Wield::applyEffect(GamePlay *gameplay)
{
    if (!gameplay)
        return false;

    Card *selected = gameplay->selectedHandCard();

    if (!selected)
        return false;

    for (int i = 0; i < m_copyCount; ++i) {
        Card *copy = selected->clone();
        copy->setLifetime(CardLifetime::EndOfCombat);
        gameplay->addCardToHand(copy);
    }

    return true;
}

void Dual_Wield::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_copyCount++;
}

Card *Dual_Wield::clone() const
{
    Dual_Wield *copy = new Dual_Wield();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}