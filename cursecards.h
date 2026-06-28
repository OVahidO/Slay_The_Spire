#ifndef CURSECARDS_H
#define CURSECARDS_H

#include "card.h"

class CurseCard : public Card
{
public:
    explicit CurseCard(QString name, int energyCost, QString path, bool isRare = false, QGraphicsItem *parent = nullptr);
    virtual ~CurseCard();
};

#endif // CURSECARDS_H
