#include "skillcards.h"
#include "player.h"

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
        player->addBlock(5);
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
        player->addStrength(player->strength());
    }
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
        player->drawCards(3);
    }
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

    if (player != nullptr) {
        player->addBlock(30);
    }
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

    if (player != nullptr) {
        player->addBlock(15);
        player->addStatus(Wound());
        player->addStatus(Wound());
    }
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
        player->addEnergy(2);
    }
}