#ifndef STATUSCARDS_H
#define STATUSCARDS_H

#include "card.h"

class StatusCard : public Card
{
public:
    explicit StatusCard(QString name, int energyCost, QString path, bool isRare = false, QGraphicsItem *parent = nullptr);
    virtual ~StatusCard();
};

#endif // STATUSCARDS_H
