#include "skillcards.h"

SkillCard::SkillCard(QString name, int energyCost, QString path, bool isRare, QGraphicsItem *parent)
    :Card(name, CardType::Skill, energyCost, path, isRare, parent) {}

SkillCard::~SkillCard() {}

Defend::Defend(QString path, QGraphicsItem *parent)
    :SkillCard("Defend", 1, path, false, parent) {}

void Defend::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
        player->addBlock(5);
}

Limit_Break::Limit_Break(QString path, QGraphicsItem *parent)
    :SkillCard("Limit_Break", 1, path, true, parent) {}

void Limit_Break::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        player->addStrength(player->strength());
    }
}

Offering::Offering(QString path, QGraphicsItem *parent)
    :SkillCard("Offering", 0, path, true, parent) {}

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
    :SkillCard("Impervious", 2, path, true, parent) {}

void Impervious::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        player->addBlock(30);
    }
}

Power_Through::Power_Through(QString path, QGraphicsItem *parent)
    :SkillCard("Power_Through", 1, path, false, parent) {}

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