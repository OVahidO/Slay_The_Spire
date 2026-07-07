#include "mapButton.h"

MapButton::MapButton(MapButtonType buttonType, int levelIndex, int levelPosIndex, QGraphicsItem *parent)
    : QGraphicsObject{parent}
    , m_levelIndex(levelIndex)
    , m_levelPosIndex(levelPosIndex)
    , m_buttonType(buttonType)
{}

QRectF MapButton::boundingRect() const
{
    return QRectF(0, 0, 0, 0);
}

void MapButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_UNUSED(painter);
}

int MapButton::levelIndex() const
{
    return m_levelIndex;
}
int MapButton::levelPosIndex() const
{
    return m_levelPosIndex;
}
MapButtonType MapButton::buttonType() const
{
    return m_buttonType;
}

QVector<MapButton*>& MapButton::nextButtons()
{
    return m_nextButtons;
}