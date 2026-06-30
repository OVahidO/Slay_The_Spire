#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsObject>
#include <QObject>
#include <QString>

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

    void setHp(int hp);
    void setCoin(int coin);
    void setEnergy(int energy);
    void setBlock(int block);
    void setMaxHp(int maxHp);

    void addMaxHp(int n = 1);
    void addBlock(int n = 1);
    void addEnergy(int n = 1);
    void loseHp(int n = 1);


signals:

private:
    int m_Hp = 100;
    int m_coin = 0;
    int m_energy = 3;
    int m_block = 0;
    int m_maxHp = 100;
};

#endif // PLAYER_H
