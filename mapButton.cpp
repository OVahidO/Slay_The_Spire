#include "mapButton.h"
#include <QPainter>
#include <QCursor>

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

    this->setAcceptHoverEvents(true);
    this->setTransformOriginPoint(m_pixmap.width()/2, m_pixmap.height()/2);
    this->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    this->setEnabled(false);

    m_hoverAnimation = new QVariantAnimation(this);
    m_hoverAnimation->setDuration(100);

    connect(m_hoverAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value){this->setScale(value.toReal());});
}

QRectF MapButton::boundingRect() const
{
    return QRectF(0, 0, m_pixmap.width(), m_pixmap.height());
}

void MapButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();

    if(!m_pixmap.isNull() && isEnabled())
        painter->drawPixmap(0, 0, m_pixmap);
    if(!isEnabled())
    {
        QPixmap disablePix = m_pixmap;
        QPainter pixPainter(&disablePix);
        pixPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        pixPainter.fillRect(disablePix.rect(), Qt::gray);
        pixPainter.end();
        painter->drawPixmap(0, 0, disablePix);
    }

    painter->restore();
}

void MapButton::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    for(auto& button : this->m_nextButtons)
        button->setEnabled(true);
    this->setEnabled(false);

    emit onClick(this);

    QGraphicsObject::mousePressEvent(event);
}

void MapButton::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    m_hoverAnimation->stop();
    m_hoverAnimation->setStartValue(this->scale());
    m_hoverAnimation->setEndValue(1.2);
    m_hoverAnimation->start();

    QGraphicsObject::hoverEnterEvent(event);
}

void MapButton::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    m_hoverAnimation->stop();
    m_hoverAnimation->setStartValue(this->scale());
    m_hoverAnimation->setEndValue(1.0);
    m_hoverAnimation->start();

    QGraphicsObject::hoverLeaveEvent(event);
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