#include "statuscards.h"

StatusCard::StatusCard(QString name, int energyCost, QString path, bool isRare, QGraphicsItem *parent)
    :Card(name, CardType::Status, energyCost, path, isRare, parent) {}

StatusCard::~StatusCard() {}

DAZE::DAZE(QString path, QGraphicsItem *parent)
    :StatusCard("DAZE", 0, path, false, parent) {}

void DAZE::applyEffect(Player* player, Enemy* targetEnemy) {Q_UNUSED(player); Q_UNUSED(targetEnemy);}