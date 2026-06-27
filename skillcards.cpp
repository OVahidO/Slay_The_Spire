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