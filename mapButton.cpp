#include "mapButton.h"
#include <QPainter>

MapButton::MapButton(MapButtonType buttonType, int levelIndex, int levelPosIndex, QGraphicsItem *parent)
    : QGraphicsObject{parent}
    , m_levelIndex(levelIndex)
    , m_levelPosIndex(levelPosIndex)
    , m_buttonType(buttonType)
{
    switch (m_buttonType) {
    case MapButtonType::ENEMY:
        m_resourcePath = ":/Map/Pics/Map/monster.png";
        break;
    case MapButtonType::ELITE:
        m_resourcePath = ":/Map/Pics/Map/elite.png";
        break;
    case MapButtonType::EVENT:
        m_resourcePath = ":/Map/Pics/Map/event.png";
        break;
    case MapButtonType::SHOP:
        m_resourcePath = ":/Map/Pics/Map/shop.png";
        break;
    case MapButtonType::CAMPFIRE:
        m_resourcePath = ":/Map/Pics/Map/rest.png";
        break;
    case MapButtonType::TREASURE:
        m_resourcePath = ":/Map/Pics/Map/chest.png";
        break;
    default:
        break;
    }
    m_pixmap.load(m_resourcePath);
}

QRectF MapButton::boundingRect() const
{
    return QRectF(0, 0, 50, 50);
}

void MapButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(!m_pixmap.isNull())
        painter->drawPixmap(0, 0, m_pixmap);
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