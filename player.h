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
    virtual ~Player();

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
};

#endif // PLAYER_H
