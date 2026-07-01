#include "statuscards.h"

StatusCard::StatusCard(QString name,
                       int energyCost,
                       QString path,
                       QString description,
                       bool isRare,
                       bool isExhaust,
                       bool requiresTarget,
                       QGraphicsItem *parent)
    : Card(name, CardType::Status, energyCost, path, description, isRare, requiresTarget, parent)
{}

StatusCard::~StatusCard() {}

DAZE::DAZE(QString path, QGraphicsItem *parent)
    : StatusCard("DAZE", 0, path, "", false, false, false, parent)
{}

void DAZE::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

SLIME::SLIME(QString path, QGraphicsItem *parent)
    : StatusCard("SLIME", 1, path, "", false, true, false, parent)
{}

void SLIME::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

WOUND::WOUND(QString path, QGraphicsItem *parent)
    : StatusCard("WOUND", 0, path, "", false, false, false, parent)
{}

void WOUND::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

BURN::BURN(QString path, QGraphicsItem *parent)
    : StatusCard("BURN",
                 0,
                 path,
                 "At the end of your turn\nif in hand: take 2 damage",
                 false,
                 false,
                 false,
                 parent)
{}

void BURN::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}