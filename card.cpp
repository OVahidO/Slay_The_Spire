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
    , m_baseEnergyCost(energyCost)
    , m_isRare(isRare)
    , m_isExhaust(isExhaust)
    , m_needTarget(requiresTarget)
    , m_description(description)
{
    if((type == CardType::Status && name != "SLIME") || (type == CardType::Curse && name != "J_A_X"))
        setAcceptHoverEvents(false);
    else
    {
        setAcceptHoverEvents(true);
        this->setFlag(QGraphicsItem::ItemIsMovable);
        this->setFlag(QGraphicsItem::ItemIsSelectable);
    }

    this->setTransformOriginPoint(85 , 120);

    m_hoverAnimation = new QVariantAnimation(this);
    m_hoverAnimation->setDuration(150);

    connect(m_hoverAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant& value){this->setScale(value.toReal());});

    auto *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 6);
    shadow->setColor(QColor(0, 0, 0, 160));
    setGraphicsEffect(shadow);
}

QRectF Card::boundingRect() const
{
    return QRectF(0, 0, 170, 240);
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

    QRectF rect = boundingRect();
    qreal radius = 16;

    QPainterPath cardShape;
    cardShape.addRoundedRect(rect, radius, radius);
    painter->setClipPath(cardShape);

    painter->fillRect(rect, QColor(20, 20, 25));

    if (!m_cardPixmap.isNull()) {
        QSizeF pixSize = m_cardPixmap.size();
        qreal scale = qMax(rect.width() / pixSize.width(), rect.height() / pixSize.height());
        QSizeF scaledSize = pixSize * scale;

        qreal x = rect.x() + (rect.width() - scaledSize.width()) / 2.0;
        qreal y = rect.y() + (rect.height() - scaledSize.height()) / 2.0;

        QRectF targetRect(x, y, scaledSize.width(), scaledSize.height());
        painter->drawPixmap(targetRect, m_cardPixmap, m_cardPixmap.rect());
    }

    QRectF fadeRect(rect.x(), rect.y() + rect.height() * 0.55, rect.width(), rect.height() * 0.45);
    QLinearGradient gradient(fadeRect.topLeft(), fadeRect.bottomLeft());
    gradient.setColorAt(0.0, QColor(0, 0, 0, 0));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 230));
    painter->fillRect(fadeRect, gradient);

    QColor typeColor = colorForCardType(m_type);
    QRectF energyBadge(rect.x() + 12, rect.y() + 12, 40, 40);

    painter->setBrush(typeColor);
    painter->setPen(QPen(Qt::white, 2));
    painter->drawEllipse(energyBadge);

    QColor energyTextColor = (m_energyCost < m_baseEnergyCost) ? QColor(90, 230, 120) : Qt::white;
    painter->setPen(energyTextColor);

    QFont energyFont("Oxanium", 16, QFont::Bold);
    painter->setFont(energyFont);
    painter->drawText(energyBadge, Qt::AlignCenter, QString::number(m_energyCost));

    QRectF textRect(rect.x() + 14, fadeRect.y() + 8, rect.width() - 28, fadeRect.height() - 16);

    QColor nameColor = m_isUpgraded ? QColor(90, 230, 110) : Qt::white;
    QFont nameFont("Cinzel", 15, QFont::Bold);
    painter->setFont(nameFont);
    painter->setPen(nameColor);
    painter->drawText(textRect, Qt::AlignTop | Qt::AlignLeft, m_name);

    QRectF descRect = textRect.adjusted(0, 26, 0, 0);

    QTextDocument doc;
    doc.setDefaultFont(QFont("Rajdhani", 10));
    doc.setTextWidth(descRect.width());
    doc.setHtml(
        QString("<div style='color:#dcdcdc;'>%1</div>").arg(highlightKeywords(m_description)));

    painter->save();
    painter->translate(descRect.topLeft());
    doc.drawContents(painter, QRectF(0, 0, descRect.width(), descRect.height()));
    painter->restore();

    painter->setClipping(false);
    painter->setPen(QPen(typeColor, 3));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(cardShape);
}

QColor Card::colorForCardType(CardType type) const
{
    switch (type) {
    case CardType::Attack:
        return QColor(200, 40, 40);
    case CardType::Skill:
        return QColor(40, 130, 200);
    case CardType::Power:
        return QColor(150, 60, 190);
    case CardType::Status:
        return QColor(120, 120, 120);
    case CardType::Curse:
        return QColor(70, 20, 90);
    }
    return Qt::gray;
}

///
QString Card::highlightKeywords(const QString &text) const
{
    static const QStringList keywords = {"Strength",
                                         "Dexterity",
                                         "Vulnerable",
                                         "Weak",
                                         "Frail",
                                         "Block",
                                         "Energy",
                                         "Exhaust",
                                         "Ethereal",
                                         "Retain"};

    QString escaped = text.toHtmlEscaped();
    escaped.replace("\n", "<br>");

    for (const QString &keyword : keywords) {
        QRegularExpression pattern("\\b(" + QRegularExpression::escape(keyword) + "s?)\\b",
                                   QRegularExpression::CaseInsensitiveOption);
        escaped.replace(pattern, "<span style='color:#f2c94c;'>\\1</span>");
    }

    return escaped;
}
///

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_oldZValue = this->zValue();
    this->setZValue(1000);

    m_hoverAnimation->stop();
    m_hoverAnimation->setStartValue(this->scale());
    m_hoverAnimation->setEndValue(1.2);
    m_hoverAnimation->start();

    emit cardEnterrdMouse(this);
}

void Card::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_hoverAnimation->stop();
    m_hoverAnimation->setStartValue(this->scale());
    m_hoverAnimation->setEndValue(1.0);
    m_hoverAnimation->start();

    this->setZValue(m_oldZValue);

    emit cardLeavedMouse(nullptr);
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
    {
        if((this->cardType() == CardType::Attack && target) || (this->cardType() != CardType::Attack && player))
            emit this->targetCardPlayed(this, player, target);
    }
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