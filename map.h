#ifndef MAP_H
#define MAP_H

#include <QGraphicsObject>
#include <QObject>
#include <QVector>

class MapButton;

class Map : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Map(QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
signals:

private:
    QVector<QVector<MapButton*>> m_levels;
    void connectLevels();
};

#endif // MAP_H
