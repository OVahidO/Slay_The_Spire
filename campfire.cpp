#include "campfire.h"

#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QTimer>
#include <QVBoxLayout>
#include <algorithm>

#include "card.h"
#include "cardIDeas.h"
#include "carddialogs.h"
#include "gameplay.h"
#include "player.h"

#include "relic.h"

CampfireGraphicsView::CampfireGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void CampfireGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *item = itemAt(event->pos());
    if (item)
        emit itemClicked(item);

    QGraphicsView::mousePressEvent(event);
}

CampfireOptionItem::CampfireOptionItem(const QString &title,
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

QRectF CampfireOptionItem::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void CampfireOptionItem::paint(QPainter *painter,
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

void CampfireOptionItem::setOptionEnabled(bool enabled)
{
    m_enabled = enabled;
    setAcceptedMouseButtons(enabled ? Qt::LeftButton : Qt::NoButton);
    update();
}

bool CampfireOptionItem::isOptionEnabled() const
{
    return m_enabled;
}

void CampfireOptionItem::setSubtitle(const QString &subtitle)
{
    m_subtitle = subtitle;
    update();
}

void CampfireOptionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_enabled)
        emit clicked();

    QGraphicsObject::mousePressEvent(event);
}

void CampfireOptionItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    update();
    QGraphicsObject::hoverEnterEvent(event);
}

void CampfireOptionItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    update();
    QGraphicsObject::hoverLeaveEvent(event);
}

Campfire::Campfire(Player *player, GamePlay *gamePlay, QWidget *parent)
    : QWidget(parent)
    , m_player(player)
    , m_gamePlay(gamePlay)
{
    setFixedSize(1280, 640);

    setupScene();
    checkAvailableOptions();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_view);
}

Campfire::~Campfire() {}

void Campfire::setupScene()
{
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 1280, 640);

    QLinearGradient bgGradient(0, 0, 0, 640);
    bgGradient.setColorAt(0.0, QColor(40, 25, 20));
    bgGradient.setColorAt(1.0, QColor(15, 10, 8));
    m_scene->setBackgroundBrush(bgGradient);

    m_view = new CampfireGraphicsView(m_scene, this);

    m_titleItem = new QGraphicsTextItem("Campfire - Choose one option");
    QFont titleFont("Arial", 20, QFont::Bold);
    m_titleItem->setFont(titleFont);
    m_titleItem->setDefaultTextColor(QColor(250, 180, 90));
    m_titleItem->setPos(1280 / 2 - 220, 60);
    m_scene->addItem(m_titleItem);

    m_restOption = new CampfireOptionItem("Rest", "Heal 20% of Max HP", ":/icons/Pics/Map/rest.png");
    m_smithOption
        = new CampfireOptionItem("Smith",
                                 "Upgrade a card\nfrom your deck",
                                 ":/icons/Pics/Icons/card-type-icon/black crossed sword.png");
    m_liftOption = new CampfireOptionItem("Lift",
                                          "Gain 1 permanent\nStrength (Girya)",
                                          ":/icons/Pics/Icons/relic/normal/girya.png");
    m_reviveOption = new CampfireOptionItem("Revive",
                                            "Burn a card to\nrevive your teammate",
                                            ":/icons/Pics/Icons/hpIcon.png");

    connect(m_restOption, &CampfireOptionItem::clicked, this, &Campfire::onRestClicked);
    connect(m_smithOption, &CampfireOptionItem::clicked, this, &Campfire::onSmithClicked);
    connect(m_liftOption, &CampfireOptionItem::clicked, this, &Campfire::onLiftClicked);
    connect(m_reviveOption, &CampfireOptionItem::clicked, this, &Campfire::onReviveClicked); // ADD

    qreal spacing = 260;
    qreal startX = 1280 / 2 - (spacing * 4 / 2) + (spacing - 220) / 2;
    qreal optionY = 220;

    m_restOption->setPos(startX, optionY);
    m_smithOption->setPos(startX + spacing, optionY);
    m_liftOption->setPos(startX + spacing * 2, optionY);
    m_reviveOption->setPos(startX + spacing * 3, optionY);

    m_scene->addItem(m_restOption);
    m_scene->addItem(m_smithOption);
    m_scene->addItem(m_liftOption);
    m_scene->addItem(m_reviveOption);
}

void Campfire::checkAvailableOptions()
{
    bool hasCoffeeDripper = false;
    bool hasGirya = false;
    Relic *giryaRelic = nullptr;

    for (Relic *r : m_player->relics()) {
        if (r->name() == "Coffee Dripper")
            hasCoffeeDripper = true;
        else if (r->name() == "Girya") {
            hasGirya = true;
            giryaRelic = r;
        }
    }

    if (hasCoffeeDripper) {
        m_restOption->setOptionEnabled(false);
        m_restOption->setSubtitle("Disabled by\nCoffee Dripper");
    } else if (m_player->currentHP() >= m_player->maxHP()) {
        m_restOption->setOptionEnabled(false);
        m_restOption->setSubtitle("Already at\nfull HP");
    }

    bool showLift = hasGirya && giryaRelic && giryaRelic->counter() > 0;
    m_liftOption->setVisible(showLift);

    bool hasEliminatedTeammate = false;
    if (m_gamePlay && m_gamePlay->isCoopMode()) {
        for (Player *p : m_gamePlay->allPlayers())
            if (p && p->currentHP() <= 0) {
                hasEliminatedTeammate = true;
                break;
            }
    }
    m_reviveOption->setVisible(hasEliminatedTeammate);
}

void Campfire::onRestClicked()
{
    if (!m_player)
        return;

    int healAmount = qRound(m_player->maxHP() * 0.20);
    m_player->heal(healAmount);

    emit campfireFinished();
}

void Campfire::onSmithClicked()
{
    if (!m_gamePlay)
        return;

    UpgradeDialog dialog(m_gamePlay, this);

    if (dialog.exec() == QDialog::Accepted && dialog.cardWasUpgraded())
        emit campfireFinished();
}

void Campfire::onLiftClicked()
{
    if (!m_player)
        return;

    for (Relic *r : m_player->relics()) {
        if (r->name() == "Girya" && r->counter() > 0) {
            r->setCounter(r->counter() - 1);
            m_player->applyBuffDebuff(BuffDebuffType::Strength, 1);
            break;
        }
    }

    emit campfireFinished();
}

void Campfire::onReviveClicked()
{
    if (!m_gamePlay)
        return;

    // نکته‌ی مهم: از CardSelectionMode::PickAny استفاده شده، نه Remove.
    // چون در حالت Remove، دیالوگ خودش کارت را delete می‌کند و اگر بعد از dialog.exec()
    // بخواهیم card->isRare()/card->ID() را بخوانیم، به یک Use-After-Free برمی‌خوریم.
    // در PickAny کارت فقط انتخاب می‌شود، پاک نمی‌شود؛ خودمان دستی مدیریتش می‌کنیم.
    CardSelectionDialog dialog(m_gamePlay,
                               CardSelectionMode::PickAny,
                               1,
                               "Revive - Choose a card to burn",
                               this);

    if (dialog.exec() != QDialog::Accepted || dialog.selectedCards().isEmpty())
        return;

    Card *burnedCard = dialog.selectedCards().first();

    int chance = 50;
    if (burnedCard->isRare())
        chance = 100;
    else if (burnedCard->ID() == static_cast<int>(CardID::Strike)
             || burnedCard->ID() == static_cast<int>(CardID::Defend))
        chance = 2;

    bool success = QRandomGenerator::global()->bounded(100) < chance;

    auto &deck = m_gamePlay->deck();
    auto it = std::find(deck.begin(), deck.end(), burnedCard);
    if (it != deck.end())
        deck.erase(it);

    delete burnedCard;

    if (success)
        emit reviveRequested();

    emit campfireFinished();
}

UpgradeDialog::UpgradeDialog(GamePlay *gamePlay, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Select a Card to Upgrade (Smith)");
    resize(900, 500);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 900, 500);

    QLinearGradient bgGradient(0, 0, 0, 500);
    bgGradient.setColorAt(0.0, QColor(35, 30, 45));
    bgGradient.setColorAt(1.0, QColor(15, 12, 20));
    m_scene->setBackgroundBrush(bgGradient);

    m_view = new CampfireGraphicsView(m_scene, this);
    connect(m_view, &CampfireGraphicsView::itemClicked, this, &UpgradeDialog::onSceneItemClicked);

    layout->addWidget(m_view);

    QGraphicsTextItem *title = new QGraphicsTextItem("Choose a card to upgrade");
    QFont titleFont("Arial", 16, QFont::Bold);
    title->setFont(titleFont);
    title->setDefaultTextColor(Qt::white);
    title->setPos(20, 10);
    m_scene->addItem(title);

    const int cardStartX = 40;
    const int cardStartY = 70;
    const int cardSpacingX = 190;
    const int cardSpacingY = 260;
    const int perRow = 4;

    int index = 0;
    bool hasUpgradableCards = false;

    if (gamePlay) {
        for (Card *card : gamePlay->deck()) {
            if (card && !card->isUpgraded() && card->cardType() != CardType::Status
                && card->cardType() != CardType::Curse) {
                hasUpgradableCards = true;
                m_selectableCards.append(card);

                int row = index / perRow;
                int col = index % perRow;

                card->setFlag(QGraphicsItem::ItemIsMovable, false);
                card->setFlag(QGraphicsItem::ItemIsSelectable, false);
                card->setPos(cardStartX + col * cardSpacingX, cardStartY + row * cardSpacingY);

                m_scene->addItem(card);

                index++;
            }
        }
    }

    if (!hasUpgradableCards) {
        QMessageBox::warning(this, "No Cards", "You don't have any upgradable cards!");
        QTimer::singleShot(0, this, &QDialog::reject);
    }
}

void UpgradeDialog::onSceneItemClicked(QGraphicsItem *item)
{
    Card *card = qgraphicsitem_cast<Card *>(item);

    if (!card && item->parentItem())
        card = qgraphicsitem_cast<Card *>(item->parentItem());

    if (!card || !m_selectableCards.contains(card))
        return;

    card->upgrade();
    card->update();

    m_cardWasUpgraded = true;

    QMessageBox::information(this, "Smith Successful", card->name() + " has been upgraded!");
    accept();
}

UpgradeDialog::~UpgradeDialog()
{
    for (Card *card : m_selectableCards)
        if (card && m_scene && m_scene->items().contains(card))
            m_scene->removeItem(card);
}

bool UpgradeDialog::cardWasUpgraded() const
{
    return m_cardWasUpgraded;
}
