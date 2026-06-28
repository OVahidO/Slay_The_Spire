#include "statuscards.h"

StatusCard::StatusCard(QString name, int energyCost, QString path, bool isRare, QGraphicsItem *parent)
    :Card(name, CardType::Status, energyCost, path, isRare, parent) {}

StatusCard::~StatusCard() {}