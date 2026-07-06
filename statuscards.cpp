#include "statuscards.h"

StatusCard::StatusCard(QString name,
                       int energyCost,
                       QString description,
                       bool isRare,
                       bool isExhaust,
                       bool requiresTarget,
                       QGraphicsItem *parent)
    : Card(name, CardType::Status, energyCost, description, isRare, isExhaust, requiresTarget, parent)
{}

StatusCard::~StatusCard() {}

DAZE::DAZE(QGraphicsItem *parent)
    : StatusCard("DAZE", 0, "", false, false, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Status/dazed.png";
    loadPixmap();
}

void DAZE::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

SLIME::SLIME(QGraphicsItem *parent)
    : StatusCard("SLIME", 1, "", false, true, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Status/slimed.png";
    loadPixmap();
}

void SLIME::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

WOUND::WOUND(QGraphicsItem *parent)
    : StatusCard("WOUND", 0, "", false, false, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Status/wound.png";
    loadPixmap();
}

void WOUND::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

BURN::BURN(QGraphicsItem *parent)
    : StatusCard("BURN",
                 0,
                 "At the end of your turn\nif in hand: take 2 damage",
                 false,
                 false,
                 false,
                 parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Status/burn.png";
    loadPixmap();
}

void BURN::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}