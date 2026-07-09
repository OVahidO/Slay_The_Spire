#ifndef PLAYER_H
#define PLAYER_H

#include "combatant.h"

class Card;
class Potion;

class Player : public Combatant
{
    Q_OBJECT
public:
    explicit Player(QString name, int maxHP, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    int coin() const;
    int energy() const;
    int maxEnergy() const;
    int handSize() const;

    QVector<Card *> &HandsCards();
    QVector<Potion *> &Potions();

    void setCoin(int coin);
    void setEnergy(int energy);
    void setMaxEnergy(int maxEnergy);
    void setHandSize(int handSize);

    bool cannotPlayAttacks() const;
    void setCannotPlayAttacks(bool value);

    void addMaxHp(int n = 1);
    void addEnergy(int n = 1);
    void loseHp(int n = 1);
    void loseEnergy(int n = 1);
    void heal(int n = 1);
    bool addPotion(Potion* potion);

signals:
    void hpChanged();
    void coinChanged();
    void energyChanged();
    void potionAdded(Potion* potion);
    void valueChanged();

private:
    bool m_cannotPlayAttacks = false;

    int m_coin = 0;
    int m_energy = 3;
    int m_maxEnergy = 3;
    int m_handSize = 5;

    QVector<Card *> m_HandsCards;
    QVector<Potion *> m_Potions{4 , nullptr};
};

#endif // PLAYER_H
