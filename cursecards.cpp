#include "cursecards.h"

CurseCard::CurseCard(QString name, int energyCost, QString path, bool isRare, QGraphicsItem *parent)
    :Card(name, CardType::Curse, energyCost, path, isRare, parent) {}

CurseCard::~CurseCard() {}

J_A_X::J_A_X(QString path, QGraphicsItem *parent)
    :CurseCard("J_A_X", 0, path, false, parent) {}

void J_A_X::applyEffect(Player* player, Enemy* targetEnemy)
{
    Q_UNUSED(targetEnemy);
    if(player != nullptr)
    {
        player->loseHp(3);
        player->addStrength(2);
    }
}

CurseOfTheBell::CurseOfTheBell(QString path, QGraphicsItem *parent)
    :CurseCard("CurseOfTheBell", 0, path, false, parent) {}

void CurseOfTheBell::applyEffect(Player* player, Enemy* targetEnemy) {Q_UNUSED(player); Q_UNUSED(targetEnemy);}