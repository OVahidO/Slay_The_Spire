#include "powercards.h"

PowerCard::PowerCard(QString name, CardType type, int energyCost, QString path, bool requiresTarget, bool isRare, QGraphicsItem *parent, QString description)
    : Card(name, type, energyCost, path, isRare, requiresTarget, parent, description) {}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Inflame::Inflame(QGraphicsItem *parent)
    : PowerCard("Inflame",
                CardType::Power,
                1,
                ":/cards/inflame.png",
                false,
                false,
                parent,
                "Gain 2 Strength")
{}

void Inflame::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(target);

    // if (player)
    //     player->applyStrength(2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Brutality::Brutality(QGraphicsItem *parent)
    : PowerCard("Brutality",
                CardType::Power,
                0,
                ":/cards/brutality.png",
                false,
                true,
                parent,
                "At the start of your turn, lose 1 HP and draw 1 card.")
{}

void Brutality::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(target);

    // if (player)
    //     player->applyPower("Brutality", 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

DemonForm::DemonForm(QGraphicsItem *parent)
    : PowerCard("Demon Form",
                CardType::Power,
                3,
                ":/cards/demon_form.png",
                false,
                true,
                parent,
                "At the start of your turn gain 3 Strength")
{}

void DemonForm::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(target);

    // if (player)
    //     player->applyBuff("Demon Form", 3);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Metallicize::Metallicize(QGraphicsItem *parent)
    : PowerCard("Metallicize",
                CardType::Power,
                1,
                ":/cards/metallicize.png",
                false,
                false,
                parent,
                "At the end of your turn get 3 block")
{}

void Metallicize::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(target);

    // if (player)
    //     player->applyBuff("Metallicize", 3);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Berserk::Berserk(QGraphicsItem *parent)
    : PowerCard("Berserk",
                CardType::Power,
                0,
                ":/cards/berserk.png",
                false,
                true,
                parent,
                "Gain 2 Vulnerable\nAt the start of your turn get 1 extra Energy")
{}
void Berserk::applyEffect(Enemy* target, Player* player) {
    Q_UNUSED(target);
    // if (player) {
    //     player->applyVulnerable(2);
    //     player->applyBuff("Berserk", 1);
    // }
}