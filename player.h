#ifndef PLAYER_H
#define PLAYER_H

#include "combatant.h"

class Card;
class Player;

enum class PowerUseTime { StartTurn, EndTurn, Exhausted };

struct PowerEffect
{
    int value;
    void (*func)(Player *player, int value);
    PowerUseTime useTime;
};

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
    int handSize() const;

    QVector<Card *> &HandsCards();
    QVector<PowerEffect> &powerEffects();

    void setCoin(int coin);
    void setEnergy(int energy);
    void setHandSize(int handSize);

    void addMaxHp(int n = 1);
    void addEnergy(int n = 1);
    void loseHp(int n = 1);
    void heal(int n = 1);

signals:

private:
    int m_coin = 0;
    int m_energy = 3;
    int m_handSize = 5;
    QVector<PowerEffect> m_powerEffects;

    QVector<Card *> m_HandsCards;
};

#endif // PLAYER_H
