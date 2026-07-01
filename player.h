#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsObject>
#include <QObject>
#include <QString>
#include <QVector>

class Card;
class Player;

enum class PowerUseTime {StartTurn, EndTurn, Exhausted};

struct PowerEffect{
    int value;
    void (*func)(Player* player, int value);
    PowerUseTime useTime;
};

class Player : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Player(QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    int Hp() const;
    int coin() const;
    int energy() const;
    int block() const;
    int maxHp() const;
    int handSize() const;
    std::vector<Card*>& HandsCards();
    QVector<PowerEffect>& powerEffects();

    void setHp(int hp);
    void setCoin(int coin);
    void setEnergy(int energy);
    void setBlock(int block);
    void setMaxHp(int maxHp);
    void setHandSize(int handSize);

    void addMaxHp(int n = 1);
    void addBlock(int n = 1);
    void addEnergy(int n = 1);
    void loseHp(int n = 1);
    void heal(int n = 1);
    void takeDamage(int damage);


signals:

private:
    int m_Hp = 100;
    int m_coin = 0;
    int m_energy = 3;
    int m_block = 0;
    int m_maxHp = 100;
    int m_handSize = 5;
    QVector<PowerEffect> m_powerEffects;

    ///
    std::vector<Card*> m_HandsCards;
    ///
};

#endif // PLAYER_H
