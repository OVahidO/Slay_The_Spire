#include "powercards.h"
#include "enemy.h"
#include "gameplay.h"
#include "player.h"
PowerCard::PowerCard(QString name,
                     int energyCost,
                     QString description,
                     bool requiresTarget,
                     bool isExhaust,
                     bool isRare,
                     QGraphicsItem *parent)
    : Card(name, CardType::Power, energyCost, description, isRare, isExhaust, requiresTarget, parent)
{}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Inflame::Inflame(QGraphicsItem *parent)
    : PowerCard("Inflame", 1, "Gain 2 Strength", false, false, false, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Power/Inflame.png";
    loadPixmap();
}

void Inflame::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player)
        player->applyBuffDebuff(BuffDebuffType::Strength, m_strengthValue);
}

void Inflame::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_strengthValue += 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Brutality::Brutality(QGraphicsItem *parent)
    : PowerCard("Brutality",
                0,
                "At the start of your turn, lose 1 HP and draw 1 card.",
                false,
                false,
                true,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Power/Brutality.png";
    loadPixmap();
}

void Brutality::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->powerEffects().append(PowerEffect{1,
                                                  [](Combatant *self, int value, GamePlay *game) {
                                                      Player *p = dynamic_cast<Player *>(self);
                                                      if (p)
                                                          p->loseHp(value);

                                                      if (game)
                                                          game->drawFromDrawPile();
                                                  },
                                                  PowerUseTime::StartTurn});
    }
}

void Brutality::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    // will be implemented
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

DemonForm::DemonForm(QGraphicsItem *parent)
    : PowerCard("Demon Form",
                3,
                "At the start of your turn gain 3 Strength",
                false,
                false,
                true,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Power/DemonForm.png";
    loadPixmap();
}

void DemonForm::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->powerEffects().append(
            PowerEffect{m_strengthPerTurn,
                        [](Combatant *self, int value, GamePlay *) {
                            self->applyBuffDebuff(BuffDebuffType::Strength, value);
                        },
                        PowerUseTime::StartTurn});
    }
}

void DemonForm::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_strengthPerTurn += 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Metallicize::Metallicize(QGraphicsItem *parent)
    : PowerCard("Metallicize", 1, "At the end of your turn get 3 block", false, false, false, parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Power/Metallicize.png";
    loadPixmap();
}

void Metallicize::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->powerEffects().append(
            PowerEffect{m_blockPerTurn,
                        [](Combatant *self, int value, GamePlay *) { self->addBlock(value); },
                        PowerUseTime::EndTurn});
    }
}

void Metallicize::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_blockPerTurn += 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Berserk::Berserk(QGraphicsItem *parent)
    : PowerCard("Berserk",
                0,
                "Gain 2 Vulnerable\nAt the start of your turn get 1 extra Energy",
                false,
                false,
                true,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Power/Berserk.png";
    loadPixmap();
}

void Berserk::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->applyBuffDebuff(BuffDebuffType::Vulnerable, m_vulnerableAmount);

        player->powerEffects().append(PowerEffect{1,
                                                  [](Combatant *self, int value, GamePlay *) {
                                                      Player *p = dynamic_cast<Player *>(self);
                                                      if (p)
                                                          p->addEnergy(value);
                                                  },
                                                  PowerUseTime::StartTurn});
    }
}

void Berserk::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_vulnerableAmount -= 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

DarkEmbrace::DarkEmbrace(QGraphicsItem *parent)
    : PowerCard("Dark Embrace",
                2,
                "Every time a card is Exhausted, draw a card",
                false,
                false,
                false,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Power/DarkEmbrace.png";
    loadPixmap();
}

void DarkEmbrace::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->powerEffects().append(PowerEffect{0,
                                                  [](Combatant *self, int value, GamePlay *game) {
                                                      Q_UNUSED(self);
                                                      Q_UNUSED(value);

                                                      if (game)
                                                          game->drawFromDrawPile();
                                                  },
                                                  PowerUseTime::OnExhaust});
    }
}

void DarkEmbrace::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_energyCost -= 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Barricade::Barricade(QGraphicsItem *parent)
    : PowerCard("Barricade",
                3,
                "Block is not removed at the start of your turn",
                false,
                false,
                true,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Power/Barricade.png";
    loadPixmap();
}

void Barricade::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player)
        player->setBarricade(true);
}

void Barricade::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_energyCost -= 1;
    // m_sourcePath = "";
    // loadPixmap();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

FeelNoPain::FeelNoPain(QGraphicsItem *parent)
    : PowerCard("FeelNoPain",
                1,
                "Every time a card is Exhausted, gain 3 block",
                false,
                false,
                false,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Power/FeelNoPain.png";
    loadPixmap();
}

void FeelNoPain::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    // if (player) {
    // }
}

void FeelNoPain::upgrade()
{
    if (m_isUpgraded)
        return;
}