#ifndef POWERCARDS_H
#define POWERCARDS_H

#include <QObject>
#include <QWidget>
#include "card.h"

class PowerCard : public Card {
public:
    PowerCard(QString name, CardType type, int energyCost, QString path, bool requiresTarget, bool isRare, QGraphicsItem *parent, QString description);
};

class Inflame : public PowerCard {
public:
    explicit Inflame(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

class Brutality : public PowerCard {
public:
    explicit Brutality(QGraphicsItem *parent = nullptr);
    void applyEffect(Enemy* target, Player* player) override;
};

#endif // POWERCARDS_H
