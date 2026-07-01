#include "powercards.h"
#include "enemy.h"
#include "player.h"

PowerCard::PowerCard(QString name, int energyCost, QString path, QString description, bool requiresTarget, bool isRare, QGraphicsItem *parent)
    : Card(name, CardType::Power, energyCost, path, description, isRare, requiresTarget, parent) {}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Inflame::Inflame(QString path, QGraphicsItem *parent)
    : PowerCard("Inflame", 1, path, "Gain 2 Strength", false, false, parent) {}

void Inflame::applyEffect(Player* player, Enemy* target) {
    Q_UNUSED(target);

    // if (player)
    //     player->applyStrength(2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Brutality::Brutality(QString path, QGraphicsItem *parent)
    : PowerCard("Brutality", 0, path, "At the start of your turn, lose 1 HP and draw 1 card.", false, true, parent) {}

void Brutality::applyEffect(Player* player, Enemy* target) {
    Q_UNUSED(target);

    // if (player)
    //     player->applyPower("Brutality", 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

DemonForm::DemonForm(QString path, QGraphicsItem *parent)
    : PowerCard("Demon Form", 3, path, "At the start of your turn gain 3 Strength", false, true, parent) {}

void DemonForm::applyEffect(Player* player, Enemy* target) {
    Q_UNUSED(target);

    // if (player)
    //     player->applyBuff("Demon Form", 3);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Metallicize::Metallicize(QString path, QGraphicsItem *parent)
    : PowerCard("Metallicize", 1, path, "At the end of your turn get 3 block", false, false, parent) {}

void Metallicize::applyEffect(Player* player, Enemy* target) {
    Q_UNUSED(target);

    // if (player)
    //     player->applyBuff("Metallicize", 3);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Berserk::Berserk(QString path, QGraphicsItem *parent)
    : PowerCard("Berserk", 0, path, "Gain 2 Vulnerable\nAt the start of your turn get 1 extra Energy", false, true, parent) {}

void Berserk::applyEffect(Player* player, Enemy* target) {
    Q_UNUSED(target);
    // if (player) {
    //     player->applyVulnerable(2);
    //     player->applyBuff("Berserk", 1);
    // }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

DarkEmbrace::DarkEmbrace(QString path, QGraphicsItem *parent)
    : PowerCard("Dark Embrace", 2, path, "Every time a card is Exhausted, draw a card", false, false, parent) {}

void DarkEmbrace::applyEffect(Player* player, Enemy* target) {
    Q_UNUSED(target);
    // if (player)
    //     player->applyBuff("Dark Embrace", 1);
}