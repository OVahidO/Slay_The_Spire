#include "cursecards.h"

CurseCard::CurseCard(QString name,
                     int energyCost,
                     QString description,
                     bool isRare,
                     bool isExhaust,
                     bool requiresTarget,
                     QGraphicsItem *parent)
    : Card(name, CardType::Curse, energyCost, description, isRare, isExhaust, requiresTarget, parent)
{}

J_A_X::J_A_X(QGraphicsItem *parent)
    : CurseCard("J_A_X", 0, "Lose 3 HP - Gain 2 Strength", false, false, true, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Skill/J.A.X.png";
    loadPixmap();
}

void J_A_X::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);
    if (player != nullptr) {
        player->loseHp(3);
        player->addStrength(2);
    }
}

CurseOfTheBell::CurseOfTheBell(QGraphicsItem *parent)
    : CurseCard("CurseOfTheBell",
                0,
                "You cannot remove this card from your deck",
                false,
                false,
                false,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Curse/CurseoftheBell.png";
    loadPixmap();
}

void CurseOfTheBell::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

Regret::Regret(QGraphicsItem *parent)
    : CurseCard("Regret",
                0,
                "Take 1 damage for every card in hand after your turn ends",
                false,
                false,
                false,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Curse/Regret.png";
    loadPixmap();
}

void Regret::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}