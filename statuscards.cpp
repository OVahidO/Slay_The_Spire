#include "statuscards.h"

StatusCard::StatusCard(CardID ID,
                       QString name,
                       int energyCost,
                       QString description,
                       bool isRare,
                       bool isExhaust,
                       bool requiresTarget,
                       QGraphicsItem *parent)
    : Card(ID, name, CardType::Status, energyCost, description, isRare, isExhaust, requiresTarget, parent)
{}

StatusCard::~StatusCard() {}

DAZE::DAZE(QGraphicsItem *parent)
    : StatusCard(CardID::Daze, "DAZE", 0, "", false, false, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Status/dazed.png";
    loadPixmap();
}

void DAZE::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

// static inline bool DAZERegistered = []()
// {
//     Card::creators()[CardID::Daze] = [](){return new class DAZE;};
//     return true;
// }();

SLIME::SLIME(QGraphicsItem *parent)
    : StatusCard(CardID::Slime, "SLIME", 1, "", false, true, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Status/slimed.png";
    loadPixmap();
}

void SLIME::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

// static inline bool SLIMERegistered = []()
// {
//     Card::creators()[CardID::Slime] = [](){return new class SLIME;};
//     return true;
// }();

WOUND::WOUND(QGraphicsItem *parent)
    : StatusCard(CardID::Wound, "WOUND", 0, "", false, false, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Status/wound.png";
    loadPixmap();
}

void WOUND::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

// static inline bool WOUNDRegistered = []()
// {
//     Card::creators()[CardID::Wound] = [](){return new class WOUND;};
//     return true;
// }();

BURN::BURN(QGraphicsItem *parent)
    : StatusCard(CardID::Burn,
                 "BURN",
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

// static inline bool BURNRegistered = []()
// {
//     Card::creators()[CardID::Burn] = [](){return new class BURN;};
//     return true;
// }();