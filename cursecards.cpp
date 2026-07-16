#include "cursecards.h"
#include "enemy.h"
#include "player.h"
#include "gameplay.h"

CurseCard::CurseCard(CardID ID,
                     QString name,
                     int energyCost,
                     QString description,
                     bool isRare,
                     bool isExhaust,
                     bool requiresTarget,
                     QGraphicsItem *parent)
    : Card(ID, name, CardType::Curse, energyCost, description, isRare, isExhaust, requiresTarget, parent)
{}

J_A_X::J_A_X(QGraphicsItem *parent)
    : CurseCard(CardID::JAX, "J_A_X", 0, "Lose 3 HP - Gain 2 Strength", false, false, true, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Curse/jax.png";
    loadPixmap();
}

void J_A_X::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr) {
        player->loseHp(3);
        player->applyBuffDebuff(BuffDebuffType::Strength, 2);
    }
}

static inline bool J_A_XRegistered = []()
{
    Card::creators()[CardID::JAX] = [](){return new class J_A_X;};
    return true;
}();

CurseOfTheBell::CurseOfTheBell(QGraphicsItem *parent)
    : CurseCard(CardID::CurseOfTheBell,
                "CurseOfTheBell",
                0,
                "You cannot remove this card from your deck",
                false,
                false,
                false,
                parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Curse/curse_of_the_bell.png";
    loadPixmap();
}

Card *J_A_X::clone() const
{
    return new J_A_X();
}

void CurseOfTheBell::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

static inline bool CurseOfTheBellRegistered = []()
{
    Card::creators()[CardID::CurseOfTheBell] = [](){return new class CurseOfTheBell;};
    return true;
}();

Card *CurseOfTheBell::clone() const
{
    return new CurseOfTheBell();
}

Regret::Regret(QGraphicsItem *parent)
    : CurseCard(CardID::Regret,
                "Regret",
                0,
                "Take 1 damage for every card in hand after your turn ends",
                false,
                false,
                false,
                parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Curse/regret.png";
    loadPixmap();
}

void Regret::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

static inline bool RegretRegistered = []()
{
    Card::creators()[CardID::Regret] = [](){return new class Regret;};
    return true;
}();

Card *Regret::clone() const
{
    return new Regret();
}
