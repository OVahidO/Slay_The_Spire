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

    auto *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(1, 6);
    shadow->setColor(QColor(0, 0, 0, 160));

    loadTypeIcon();
}

QRectF Card::boundingRect() const
{
    return QRectF(0, 0, 170, 240);
}

void Card::loadPixmap()
{
    if (!m_sourcePath.isEmpty()) {
        m_cardPixmap = QPixmap(m_sourcePath);

        QRectF rect = boundingRect();

        QSizeF pixSize = m_cardPixmap.size();

        qreal scale = qMax(rect.width() / pixSize.width(), rect.height() / pixSize.height());

        QSizeF scaledSize = pixSize * scale;

        qreal x = rect.x() + (rect.width() - scaledSize.width()) / 2.0;
        qreal y = rect.y() + (rect.height() - scaledSize.height()) / 2.0;

        m_targetPixmapRect = QRectF(x, y, scaledSize.width(), scaledSize.height());
    }
    m_energyPixmap.load(":/icons/Pics/Icons/card-type-icon/ironclad_energy_icon.png");
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    QRectF rect = boundingRect();
    qreal radius = 10;

    QPainterPath cardShape;
    cardShape.addRoundedRect(rect, radius, radius);
    painter->setClipPath(cardShape);

    painter->fillRect(rect, QColor(20, 20, 25));

    if (!m_cardPixmap.isNull()) {
        painter->drawPixmap(m_targetPixmapRect, m_cardPixmap, m_cardPixmap.rect());
        painter->fillRect(rect, QColor(0, 0, 0, 15));
    }

    QRectF fadeRect(rect.x(), rect.y() + rect.height() * 0.55, rect.width(), rect.height() * 0.45);
    QLinearGradient gradient(fadeRect.topLeft(), fadeRect.bottomLeft());
    gradient.setColorAt(0.0, QColor(0, 0, 0, 20));
    gradient.setColorAt(0.35, QColor(0, 0, 0, 90));
    gradient.setColorAt(1.0, QColor(0, 0, 0, 245));
    painter->fillRect(fadeRect, gradient);

    QColor typeColor = colorForCardType(m_type);


    QRectF energyBadge(rect.left() + 10, rect.top() + 10, 34, 34);

    if (!m_energyPixmap.isNull())
        painter->drawPixmap(energyBadge, m_energyPixmap, m_energyPixmap.rect());

    QColor energyTextColor = (m_energyCost < m_baseEnergyCost) ? QColor(110, 255, 140) : Qt::white;

    QFont font("Oxanium");
    font.setBold(true);
    font.setPixelSize(18);

    painter->setFont(font);
    painter->setPen(energyTextColor);

    painter->drawText(energyBadge, Qt::AlignCenter, QString::number(m_energyCost));

    QRectF textRect(rect.x() + 14, fadeRect.y() + 8, rect.width() - 28, fadeRect.height() - 16);

    QRectF nameIconRect(textRect.left(), textRect.top() + 2, 18, 18);

    painter->drawPixmap(nameIconRect, m_typeIcon, m_typeIcon.rect());

    QColor nameColor = m_isUpgraded ? QColor(90, 230, 110) : Qt::white;
    QFont nameFont("Cinzel", 15, QFont::Bold);
    painter->setFont(nameFont);
    painter->setPen(nameColor);
    QRectF nameTextRect = textRect.adjusted(24, 0, 0, 0);
    painter->drawText(nameTextRect, Qt::AlignTop | Qt::AlignLeft, m_name);

    QRectF descRect = textRect.adjusted(0, 26, 0, 0);
    QTextDocument doc;
    doc.setDefaultFont(QFont("Rajdhani", 10));
    doc.setTextWidth(descRect.width());
    QString dynamicText = getDynamicDescription(m_ownerPlayer, m_hoveredEnemy);
    doc.setHtml(QString("<div style='color:#dcdcdc;'>%1</div>").arg(highlightKeywords(dynamicText)));
    painter->save();
    painter->translate(descRect.topLeft());
    doc.drawContents(painter, QRectF(0, 0, descRect.width(), descRect.height()));
    painter->restore();

    painter->setClipping(false);
    painter->setPen(QPen(typeColor.darker(130), 2.5));
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(cardShape);
}

QColor Card::colorForCardType(CardType type) const
{
    switch (type) {
    case CardType::Attack:
        return QColor(120, 25, 25);

    case CardType::Skill:
        return QColor(35, 75, 125);

    case CardType::Power:
        return QColor(105, 45, 135);

    case CardType::Status:
        return QColor(90, 90, 90);

    case CardType::Curse:
        return QColor(55, 20, 70);
    }
    return Qt::gray;
}

void Card::loadTypeIcon()
{
    switch (m_type) {
    case CardType::Attack:
        if (m_isRare)
            m_typeIcon.load(":/icons/Pics/Icons/card-type-icon/black crossed sword-gold.png");
        else
            m_typeIcon.load(":/icons/Pics/Icons/card-type-icon/black crossed sword.png");
        break;

    case CardType::Skill:
        if (m_isRare)
            m_typeIcon.load(":/icons/Pics/Icons/card-type-icon/vector shield icon-gold.png");
        else
            m_typeIcon.load(":/icons/Pics/Icons/card-type-icon/vector shield icon.png");
        break;

    case CardType::Power:
        if (m_isRare)
            m_typeIcon.load(":/icons/Pics/Icons/card-type-icon/power-up-gold.png");
        else
            m_typeIcon.load(":/icons/Pics/Icons/card-type-icon/power-up.png");
        break;

    case CardType::Status:
        if (m_isRare)
            m_typeIcon.load(":/icons/Pics/Icons/card-type-icon/influence-gold.png");
        else
            m_typeIcon.load(":/icons/Pics/Icons/card-type-icon/influence.png");
        break;

    case CardType::Curse:
        if (m_isRare)
            m_typeIcon.load(":/icons/Pics/Icons/card-type-icon/virus-gold.png");
        else
            m_typeIcon.load(":/icons/Pics/Icons/card-type-icon/virus.png");
        break;
    }
}

QString Card::highlightKeywords(const QString &text) const
{
    static const QStringList keywords = {"Strength",
                                         "Dexterity",
                                         "Vulnerable",
                                         "Weak",
                                         "Frail",
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

void Card::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);

    m_oldZValue = this->zValue();
    this->setZValue(1000);

    m_hoverAnimation->stop();
    m_hoverAnimation->setStartValue(this->scale());
    m_hoverAnimation->setEndValue(1.2);
    m_hoverAnimation->start();

    emit cardEnteredMouse(this);
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

    this->setHoveredEnemy(nullptr);

    QGraphicsItem::mouseReleaseEvent(event);
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    Enemy *currentHoveredEnemy = nullptr;
    qreal maxArea = 0.0;
    QRectF cardRect = this->sceneBoundingRect();

    for (auto &item : collidingItems()) {
        QRectF itemReact = item->sceneBoundingRect();
        QRectF intersected = cardRect.intersected(itemReact);
        qreal Area = intersected.width() * intersected.height();

        if (Area > maxArea) {
            Enemy *e = dynamic_cast<Enemy *>(item);
            if (e) {
                maxArea = Area;
                currentHoveredEnemy = e;
            }
        }
    }

    this->setHoveredEnemy(currentHoveredEnemy);
}

void Card::setHoveredEnemy(Enemy *enemy)
{
    m_hoveredEnemy = enemy;
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

bool Card::isInnate() const
{
    return m_isInnate;
}

void Card::setInnate(bool value)
{
    m_isInnate = value;
}

QString Card::getDynamicDescription(Player *player, Enemy *target) const
{
    return m_description;
}

void Card::setOwnerPlayer(Player *player)
{
    m_ownerPlayer = player;
}