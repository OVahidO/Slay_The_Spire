#ifndef SKILLCARDS_H
#define SKILLCARDS_H

#include "card.h"

class SkillCard : Card
{
public:
    explicit SkillCard(QString name, int energyCost, QString path, bool isRare = false, QGraphicsItem *parent = nullptr);
    virtual ~SkillCard();
};

#endif // SKILLCARDS_H
