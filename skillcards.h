#ifndef SKILLCARDS_H
#define SKILLCARDS_H

#include "card.h"

class SkillCard : public Card
{
public:
    explicit SkillCard(QString name, int energyCost, QString path, bool isRare = false, QGraphicsItem *parent = nullptr);
    virtual ~SkillCard();
};

class Defend : public SkillCard
{
public:
    explicit Defend(QString path, QGraphicsItem *parent = nullptr);

    void applyEffect(Player* player = nullptr, Enemy* targetEnemy = nullptr);
};

#endif // SKILLCARDS_H
