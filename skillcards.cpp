#include "skillcards.h"

SkillCard::SkillCard(QString name, int energyCost, QString path, QString description, bool isRare, bool requiresTarget, QGraphicsItem *parent)
    :Card(name, CardType::Skill, energyCost, path, description, isRare, requiresTarget, parent) {}

SkillCard::~SkillCard() {}

Defend::Defend(QString path, QGraphicsItem *parent)
    :SkillCard("Defend", 1, path, "Gain 5 block", false, true, parent) {}

void Defend::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
        player->addBlock(5);
}

Exhume::Exhume(QString path, QGraphicsItem *parent)
    :SkillCard("Exhume", 1, path, "Put a card from exhaust pile into hand", true, false, parent) {}

void Exhume::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);
    Q_UNUSED(player);
}

Limit_Break::Limit_Break(QString path, QGraphicsItem *parent)
    :SkillCard("Limit_Break", 1, path, "Double your Strength", true, true, parent) {}

void Limit_Break::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        player->addStrength(player->strength());
    }
}

Offering::Offering(QString path, QGraphicsItem *parent)
    :SkillCard("Offering", 0, path, "Lose 6 HP - Gain 2 Energy - Draw 3 cards", true, true, parent) {}

void Offering::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        player->loseHp(6);
        player->addEnergy(2);
        player->drawCards(3);
    }
}

Impervious::Impervious(QString path, QGraphicsItem *parent)
    :SkillCard("Impervious", 2, path, "Gain 30 block", true, true, parent) {}

void Impervious::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        player->addBlock(30);
    }
}

Power_Through::Power_Through(QString path, QGraphicsItem *parent)
    :SkillCard("Power_Through", 1, path, "Gain 15 block - Add 2 WOUNDs into hand", false, true, parent) {}

void Power_Through::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        player->addBlock(15);
        player->addStatus(Wound());
        player->addStatus(Wound());
    }
}

Bloodletting::Bloodletting(QString path, QGraphicsItem *parent)
    :SkillCard("Bloodletting", 0, path, "Lose 3 HP - Gain 2 Energy", false, true, parent) {}

void Bloodletting::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        player->loseHp(3);
        player->addEnergy(2);
    }
}