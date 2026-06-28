#include "statuscards.h"

StatusCard::StatusCard(QString name, int energyCost, QString path, bool isRare, QGraphicsItem *parent)
    :Card(name, CardType::Status, energyCost, path, isRare, parent) {}

StatusCard::~StatusCard() {}

DAZE::DAZE(QString path, QGraphicsItem *parent)
    :StatusCard("DAZE", 0, path, false, parent) {}

void DAZE::applyEffect(Player* player, Enemy* targetEnemy) {Q_UNUSED(player); Q_UNUSED(targetEnemy);}

SLIME::SLIME(QString path, QGraphicsItem *parent)
    :StatusCard("SLIME", 1, path, false, parent) {}

void SLIME::applyEffect(Player* player, Enemy* targetEnemy) {Q_UNUSED(player); Q_UNUSED(targetEnemy);}

WOUND::WOUND(QString path, QGraphicsItem *parent)
    :StatusCard("WOUND", 0, path, false, parent) {}

void WOUND::applyEffect(Player* player, Enemy* targetEnemy) {Q_UNUSED(player); Q_UNUSED(targetEnemy);}

BURN::BURN(QString path, QGraphicsItem *parent)
    :StatusCard("BURN", 0, path, false, parent) {}

void BURN::applyEffect(Player* player, Enemy* targetEnemy) {Q_UNUSED(player); Q_UNUSED(targetEnemy);}