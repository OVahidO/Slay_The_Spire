#include "card.h"
#include "player.h"
#include "enemy.h"

Card::Card(QString name,
           CardType type,
           int energyCost,
           QString path,
           QString description,
           bool isRare,
           bool isExhaust,
           bool requiresTarget,
           QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_ID(rand() % 1000 + 1000)
    , m_name(name)
    , m_type(type)
    , m_energyCost(energyCost)
    , m_isRare(isRare)
    , m_isExhaust(isExhaust)
    , m_needTarget(requiresTarget)
    , m_description(description)
{
    if((type == CardType::Status && name != "SLIME") || (type == CardType::Curse && name != "J_A_X"))
        setAcceptHoverEvents(false);
    else
        setAcceptHoverEvents(true);
}

QRectF Card::boundingRect() const
{
    return QRectF(0, 0, 250, 322);
}

void Card::loadPixmap()
{
    if (!m_sourcePath.isEmpty()) {
        m_cardPixmap = QPixmap(m_sourcePath);
    }
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    if (!m_cardPixmap.isNull())
        painter->drawPixmap(boundingRect(), m_cardPixmap, m_cardPixmap.rect());
}

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    this->setScale(1.5);
    setRotation(0);
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    this->setScale(1);
    //setRotation(0) -> Ahoora information//
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    Player* p = nullptr;
    Enemy* e = nullptr;

    qreal maxArea = 0.0;

    QRectF cardRect = this->sceneBoundingRect();

    for(auto& item : collidingItems())
    {
        QRectF itemReact = item->sceneBoundingRect();
        QRectF intersected = cardRect.intersected(itemReact);
        qreal Area = intersected.width() * intersected.height();
        if(Area > maxArea)
        {
            p = dynamic_cast<Player*>(item);
            e = dynamic_cast<Enemy*>(item);
            if(p || e)
            {
                maxArea = Area;
            }
        }
    }
    if(this->m_needTarget && (p || e))
        emit this->targetCardPlayed(this,p,e);
    else if(!this->m_needTarget)
        emit this->noTargetCardPlayed(this);

    QGraphicsItem::mouseReleaseEvent(event);
}

int Card::ID() const
{
    return m_ID;
}
QString Card::name() const
{
    return m_name;
}
int Card::energyCost() const
{
    return m_energyCost;
}
CardType Card::cardType() const
{
    return m_type;
}
QString Card::sourcePath() const
{
    return m_sourcePath;
}
QString Card::description() const
{
    return m_description;
}
bool Card::isRare() const
{
    return m_isRare;
}
bool Card::needTarget() const
{
    return m_needTarget;
}
bool Card::isExhaust() const
{
    return m_isExhaust;
}
bool Card::applyEffect(GamePlay *gameplay)
{
    return false;
}