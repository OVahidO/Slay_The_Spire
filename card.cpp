#include "card.h"
#include "player.h"
#include "enemy.h"

Card::Card(QString name,
           CardType type,
           int energyCost,
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

    m_hoverAnimation = new QVariantAnimation(this);
    m_hoverAnimation->setDuration(150);
    this->setTransformOriginPoint(50 , 75);

    connect(m_hoverAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value){this->setScale(value.toReal());});
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
    Q_UNUSED(event);

    m_oldZValue = this->zValue();
    this->setZValue(1000);

    m_hoverAnimation->stop();
    m_hoverAnimation->setStartValue(this->scale());
    m_hoverAnimation->setEndValue(1.2);
    m_hoverAnimation->start();
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_hoverAnimation->stop();
    m_hoverAnimation->setStartValue(this->scale());
    m_hoverAnimation->setEndValue(1.0);
    m_hoverAnimation->start();

    this->setZValue(m_oldZValue);
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    Player* player = nullptr;
    Enemy* target = nullptr;

    qreal maxArea = 0.0;

    QRectF cardRect = this->sceneBoundingRect();

    for(auto& item : collidingItems())
    {
        QRectF itemReact = item->sceneBoundingRect();
        QRectF intersected = cardRect.intersected(itemReact);
        qreal Area = intersected.width() * intersected.height();
        if(Area > maxArea)
        {
            Player* p = dynamic_cast<Player*>(item);
            Enemy* e = dynamic_cast<Enemy*>(item);
            if(p || e)
            {
                maxArea = Area;
                player = p;
                target = e;
            }
        }
    }
    if(this->m_needTarget && (player || target))
        emit this->targetCardPlayed(this, player, target);
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

bool Card::isUpgraded() const
{
    return m_isUpgraded;
}

void Card::upgrade()
{
    m_isUpgraded = true;
}

CardLifetime Card::lifetime() const
{
    return m_lifetime;
}

void Card::setLifetime(CardLifetime lifetime)
{
    m_lifetime = lifetime;
}