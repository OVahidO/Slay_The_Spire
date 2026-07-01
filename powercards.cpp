#include "powercards.h"
#include "enemy.h"
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

    // if (player)
    //     player->applyStrength(2);
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

    // if (player)
    //     player->applyPower("Brutality", 1);
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

    // if (player)
    //     player->applyBuff("Demon Form", 3);
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

    // if (player)
    //     player->applyBuff("Metallicize", 3);
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
    // if (player) {
    //     player->applyVulnerable(2);
    //     player->applyBuff("Berserk", 1);
    // }
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
    // if (player)
    //     player->applyBuff("Dark Embrace", 1);
}