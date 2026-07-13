#include "event.h"
#include "allevents.h"

#include <QCursor>
#include <QFont>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QVector>
#include <functional>

EventGraphicsView::EventGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void EventGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *item = itemAt(event->pos());
    if (item)
        emit itemClicked(item);

    QGraphicsView::mousePressEvent(event);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

SelectableOptionItem::SelectableOptionItem(const QString &title,
                                           const QString &subtitle,
                                           const QString &iconPath,
                                           QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_title(title)
    , m_subtitle(subtitle)
{
    if (!iconPath.isEmpty())
        m_icon.load(iconPath);

    setAcceptHoverEvents(true);
    setCursor(Qt::PointingHandCursor);
}

QRectF SelectableOptionItem::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void SelectableOptionItem::paint(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    QRectF rect = boundingRect();
    qreal radius = 14;

    QColor bgColor = m_enabled ? QColor(45, 37, 30, 230) : QColor(30, 30, 30, 180);
    if (m_hovered && m_enabled)
        bgColor = QColor(65, 55, 45, 240);

    painter->setBrush(bgColor);
    painter->setPen(QPen(QColor(140, 118, 92), 2));
    painter->drawRoundedRect(rect, radius, radius);

    if (!m_icon.isNull()) {
        QRectF iconRect(rect.width() / 2 - 40, 20, 80, 80);
        painter->setOpacity(m_enabled ? 1.0 : 0.35);
        painter->drawPixmap(iconRect.toRect(), m_icon);
        painter->setOpacity(1.0);
    }

    QColor textColor = m_enabled ? QColor(229, 212, 179) : QColor(120, 120, 120);

    QFont titleFont("Arial", 15, QFont::Bold);
    painter->setFont(titleFont);
    painter->setPen(textColor);
    QRectF titleRect(10, 115, rect.width() - 20, 40);
    painter->drawText(titleRect, Qt::AlignCenter | Qt::TextWordWrap, m_title);

    QFont subFont("Arial", 10);
    painter->setFont(subFont);
    painter->setPen(m_enabled ? QColor(190, 170, 140) : QColor(100, 100, 100));
    QRectF subRect(10, 160, rect.width() - 20, 90);
    painter->drawText(subRect, Qt::AlignCenter | Qt::TextWordWrap, m_subtitle);
}

void SelectableOptionItem::setOptionEnabled(bool enabled)
{
    m_enabled = enabled;
    setAcceptedMouseButtons(enabled ? Qt::LeftButton : Qt::NoButton);
    update();
}

bool SelectableOptionItem::isOptionEnabled() const
{
    return m_enabled;
}

void SelectableOptionItem::setSubtitle(const QString &subtitle)
{
    m_subtitle = subtitle;
    update();
}

void SelectableOptionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_enabled)
        emit clicked();

    QGraphicsObject::mousePressEvent(event);
}

void SelectableOptionItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    update();
    QGraphicsObject::hoverEnterEvent(event);
}

void SelectableOptionItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    update();
    QGraphicsObject::hoverLeaveEvent(event);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Event::Event(const QString &eventTitle,
             const QString &backgroundPath,
             EventScope scope,
             Player *player,
             GamePlay *gamePlay,
             QWidget *parent)
    : QWidget(parent)
    , m_player(player)
    , m_gamePlay(gamePlay)
    , m_eventTitle(eventTitle)
    , m_scope(scope)
{
    setFixedSize(1280, 640);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 1280, 640);

    if (!backgroundPath.isEmpty()) {
        QPixmap bg(backgroundPath);
        if (!bg.isNull()) {
            m_backgroundItem = m_scene->addPixmap(
                bg.scaled(1280, 640, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
            m_backgroundItem->setZValue(-10);
        }
    }

    if (!m_backgroundItem) {
        QLinearGradient bgGradient(0, 0, 0, 640);
        bgGradient.setColorAt(0.0, QColor(30, 20, 35));
        bgGradient.setColorAt(1.0, QColor(10, 8, 12));
        m_scene->setBackgroundBrush(bgGradient);
    }

    m_view = new EventGraphicsView(m_scene, this);
    connect(m_view, &EventGraphicsView::itemClicked, this, [this](QGraphicsItem *item) {
        Q_UNUSED(item);
    });

    m_titleItem = new QGraphicsTextItem(m_eventTitle);
    QFont titleFont("Arial", 22, QFont::Bold);
    m_titleItem->setFont(titleFont);
    m_titleItem->setDefaultTextColor(QColor(250, 180, 90));
    m_titleItem->setPos(1280 / 2 - m_titleItem->boundingRect().width() / 2, 40);
    m_scene->addItem(m_titleItem);

    m_descriptionItem = new QGraphicsTextItem();
    QFont descFont("Arial", 12);
    m_descriptionItem->setFont(descFont);
    m_descriptionItem->setDefaultTextColor(QColor(220, 210, 200));
    m_descriptionItem->setTextWidth(900);
    m_descriptionItem->setPos(1280 / 2 - 450, 100);
    m_scene->addItem(m_descriptionItem);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_view);
}

EventScope Event::scope() const
{
    return m_scope;
}

QString Event::eventTitle() const
{
    return m_eventTitle;
}

void Event::setDescription(const QString &text)
{
    m_descriptionItem->setPlainText(text);

    QRectF bounds = m_descriptionItem->boundingRect();
    m_descriptionItem->setPos(1280 / 2 - bounds.width() / 2, 100);
}

void Event::addOption(const EventOption &option)
{
    m_options.append(option);

    auto *item = new SelectableOptionItem(option.title, option.subtitle, option.iconPath);
    m_optionItems.append(item);
    m_scene->addItem(item);

    int index = m_options.size() - 1;
    connect(item, &SelectableOptionItem::clicked, this, [this, index]() {
        if (index >= m_options.size())
            return;

        if (m_options[index].onSelected)
            m_options[index].onSelected();

        emit eventFinished();
    });

    relayoutOptions();
    refreshAvailability();
}

void Event::relayoutOptions()
{
    int count = m_optionItems.size();
    if (count == 0)
        return;

    qreal spacing = 260;
    qreal totalWidth = spacing * count;
    qreal startX = 1280 / 2.0 - totalWidth / 2.0 + (spacing - 220) / 2.0;
    qreal optionY = 260;

    for (int i = 0; i < count; ++i)
        m_optionItems[i]->setPos(startX + spacing * i, optionY);
}

void Event::refreshAvailability()
{
    for (int i = 0; i < m_options.size(); ++i) {
        bool available = m_options[i].isAvailable ? m_options[i].isAvailable() : true;
        m_optionItems[i]->setOptionEnabled(available);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Event *createRandomEvent(int actNumber, Player *player, GamePlay *gamePlay, QWidget *parent)
{
    QVector<std::function<Event *()>> pool;

    pool.append([&]() -> Event * { return new OminousForge(player, gamePlay, parent); });
    pool.append([&]() -> Event * { return new Lab(player, gamePlay, parent); });

    if (actNumber == 1) {
        pool.append([&]() -> Event * { return new GoldenIdolEvent(player, gamePlay, parent); });
        pool.append([&]() -> Event * { return new TheCleric(player, gamePlay, parent); });
    } else if (actNumber == 2) {
        pool.append([&]() -> Event * { return new Augmenter(player, gamePlay, parent); });
        pool.append([&]() -> Event * { return new FaceTrader(player, gamePlay, parent); });
        pool.append([&]() -> Event * { return new Colosseum(player, gamePlay, parent); });
        pool.append([&]() -> Event * { return new PleadingVagrant(player, gamePlay, parent); });
        pool.append([&]() -> Event * { return new TheJoust(player, gamePlay, parent); });
    }

    if (pool.isEmpty())
        return nullptr;

    int index = QRandomGenerator::global()->bounded(pool.size());
    return pool[index]();
}
