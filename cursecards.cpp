#include "cursecards.h"

CurseCard::CurseCard(QString name,
                     int energyCost,
                     QString path,
                     QString description,
                     bool isRare,
                     bool isExhaust,
                     bool requiresTarget,
                     QGraphicsItem *parent)
    : Card(name, CardType::Curse, energyCost, path, description, isRare, requiresTarget, parent)
{}

CurseCard::~CurseCard() {}

J_A_X::J_A_X(QString path, QGraphicsItem *parent)
    : CurseCard("J_A_X", 0, path, "Lose 3 HP - Gain 2 Strength", false, false, true, parent)
{}

void J_A_X::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);
    if (player != nullptr) {
        player->loseHp(3);
        player->addStrength(2);
    }
}

CurseOfTheBell::CurseOfTheBell(QString path, QGraphicsItem *parent)
    : CurseCard("CurseOfTheBell",
                0,
                path,
                "You cannot remove this card from your deck",
                false,
                false,
                false,
                parent)
{}

void CurseOfTheBell::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

Regret::Regret(QString path, QGraphicsItem *parent)
    : CurseCard("Regret",
                0,
                path,
                "Take 1 damage for every card in hand after your turn ends",
                false,
                false,
                false,
                parent)
{}

void Regret::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}