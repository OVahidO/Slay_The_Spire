#include "skillcards.h"
#include "enemy.h"
#include "player.h"
#include "gameplay.h"
#include "statuscards.h"

SkillCard::SkillCard(QString name, int energyCost, QString path, QString description, bool isRare, bool isExhaust, bool requiresTarget, QGraphicsItem *parent)
    :Card(name, CardType::Skill, energyCost, path, description, isRare, requiresTarget, parent) {}

SkillCard::~SkillCard() {}

Defend::Defend(QString path, QGraphicsItem *parent)
    :SkillCard("Defend", 1, path, "Gain 5 block", false, false, true, parent) {}

void Defend::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
        player->addBlock(5);
}

Exhume::Exhume(QString path, QGraphicsItem *parent)
    :SkillCard("Exhume", 1, path, "Put a card from exhaust pile into hand", true, true, false, parent) {}

void Exhume::applyEffect(Player* player, Enemy* targetEnemy) {Q_UNUSED(targetEnemy); Q_UNUSED(player);}
bool Exhume::applyEffect(GamePlay* gameplay)
{
    gameplay->drawFromExhaustPile();
    return true;
}

Limit_Break::Limit_Break(QString path, QGraphicsItem *parent)
    :SkillCard("Limit_Break", 1, path, "Double your Strength", true, true, true, parent) {}

void Limit_Break::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        //player->addStrength(player->strength());
    }
}

Offering::Offering(QString path, QGraphicsItem *parent)
    :SkillCard("Offering", 0, path, "Lose 6 HP\nGain 2 Energy\nDraw 3 cards", true, true, true, parent) {}

void Offering::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);
    if(player != nullptr)
    {
        player->loseHp(6);
        player->addEnergy(2);
    }
}

bool Offering::applyEffect(GamePlay* gameplay)
{
    for(int i = 0 ; i < 3 ; gameplay->drawFromDrawPile(), i++);
    return true;
}

Impervious::Impervious(QString path, QGraphicsItem *parent)
    :SkillCard("Impervious", 2, path, "Gain 30 block", true, true, true, parent) {}

void Impervious::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        player->addBlock(30);
    }
}

Power_Through::Power_Through(QString path, QGraphicsItem *parent)
    :SkillCard("Power_Through", 1, path, "Gain 15 block\nAdd 2 WOUNDs into hand", false, false, true, parent) {}

void Power_Through::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        player->addBlock(15);
    }
}

bool Power_Through::applyEffect(GamePlay* gameplay)
{
    gameplay->addCardToHand(new WOUND(""));
    gameplay->addCardToHand(new WOUND(""));
    return true;
}

Bloodletting::Bloodletting(QString path, QGraphicsItem *parent)
    :SkillCard("Bloodletting", 0, path, "Lose 3 HP\nGain 2 Energy", false, false, true, parent) {}

void Bloodletting::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if(player != nullptr)
    {
        player->loseHp(3);
        player->addEnergy(2);
    }
}