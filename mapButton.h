#ifndef MAPBUTTON_H
#define MAPBUTTON_H

#include <QGraphicsObject>
#include <QObject>
#include <QVector>
#include <QVariantAnimation>

enum class MapButtonType {ENEMY, EVENT, ELITE, SHOP, CAMPFIRE, TREASURE, BOSS};

class MapButton : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MapButton(MapButtonType buttonType, int levelIndex, int levelPosIndex, QGraphicsItem *parent = nullptr);

    int levelIndex() const;
    int levelPosIndex() const;
    MapButtonType buttonType() const;

    QVector<MapButton*>& nextButtons();

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

signals:
    void onClick(MapButton* mapButton);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    QVariantAnimation* m_hoverAnimation;

private:
    int m_levelIndex;
    int m_levelPosIndex;
    MapButtonType m_buttonType;
    QString m_resourcePath;
    QPixmap m_pixmap;

    QVector<MapButton*> m_nextButtons;
};

#endif // MAPBUTTON_H
