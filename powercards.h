#ifndef POWERCARDS_H
#define POWERCARDS_H

#include <QObject>
#include <QWidget>
#include "card.h"

class PowerCard : public Card {
public:
    PowerCard(QString name, int energyCost, QString path, QString description, bool requiresTarget, bool isRare, QGraphicsItem *parent);
    virtual ~PowerCard() = 0;
};

class Inflame : public PowerCard {
public:
    explicit Inflame(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class Brutality : public PowerCard {
public:
    explicit Brutality(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class DemonForm : public PowerCard {
public:
    explicit DemonForm(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class Metallicize : public PowerCard {
public:
    explicit Metallicize(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class Berserk : public PowerCard {
public:
    explicit Berserk(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

class DarkEmbrace : public PowerCard {
public:
    explicit DarkEmbrace(QString path, QGraphicsItem *parent = nullptr);
    void applyEffect(Player* player, Enemy* target) override;
};

#endif // POWERCARDS_H
