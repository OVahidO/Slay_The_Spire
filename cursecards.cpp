#include "cursecards.h"

CurseCard::CurseCard(QString name, int energyCost, QString path, bool isRare, QGraphicsItem *parent)
    :Card(name, CardType::Curse, energyCost, path, isRare, parent) {}

CurseCard::~CurseCard() {}