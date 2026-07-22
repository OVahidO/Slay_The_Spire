#include "shop.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QRandomGenerator>
#include <QTimer>

#include "card.h"
#include "gameplay.h"
#include "player.h"
#include "potion.h"

#include "attackcards.h"
#include "powercards.h"
#include "skillcards.h"

int Shop::s_cardRemovalUseCount = 0;

ShopGraphicsView::ShopGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ShopGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *item = itemAt(event->pos());
    if (item)
        emit itemClicked(item);

    QGraphicsView::mousePressEvent(event);
}

Shop::Shop(Player *player, GamePlay *gamePlay, QWidget *parent)
    : QWidget(parent)
    , m_player(player)
    , m_gamePlay(gamePlay)
{
    setFixedSize(1280, 640);

    setupScene();
    generateStock();
    layoutStockOnScene();
    updateGoldDisplay();
    updateAffordability();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_view);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(10, 5, 10, 5);

    m_removeCardBtn = new QPushButton(this);
    connect(m_removeCardBtn, &QPushButton::clicked, this, &Shop::onRemoveCardClicked);
    bottomLayout->addWidget(m_removeCardBtn);

    bottomLayout->addStretch();

    m_leaveBtn = new QPushButton("Leave Shop", this);
    connect(m_leaveBtn, &QPushButton::clicked, this, &Shop::onLeaveClicked);
    bottomLayout->addWidget(m_leaveBtn);

    mainLayout->addLayout(bottomLayout);

    m_removeCardBtn->setText(QString("Card Removal - %1g").arg(currentCardRemovalCost()));
    m_removeCardBtn->setEnabled(m_player->coin() >= currentCardRemovalCost());
}

Shop::~Shop()
{
    for (ShopCardEntry &entry : m_cardStock)
        if (!entry.purchased && entry.card)
            delete entry.card;

    for (ShopPotionEntry &entry : m_potionStock)
        if (!entry.purchased && entry.potion)
            delete entry.potion;
}

void Shop::setupScene()
{
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 1280, 560);

    QPixmap bg(":/Shop/Pics/Background/Shop/shop.png");
    QGraphicsPixmapItem* background = m_scene->addPixmap(bg.scaled(1280, 640, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    background->setZValue(-1000);

    m_view = new ShopGraphicsView(m_scene, this);
    connect(m_view, &ShopGraphicsView::itemClicked, this, &Shop::onSceneItemClicked);

    m_goldItem = new QGraphicsTextItem();
    QFont goldFont("Arial", 16, QFont::Bold);
    m_goldItem->setFont(goldFont);
    m_goldItem->setDefaultTextColor(QColor(250, 210, 60));
    m_goldItem->setPos(20, 15);
    m_scene->addItem(m_goldItem);
}

void Shop::generateStock()
{
    const int normalCardCount = 4;
    const int rareCardCount = 1;

    for (int i = 0; i < normalCardCount; ++i) {
        ShopCardEntry entry;
        entry.card = createRandomCard(false);
        entry.isRare = false;

        bool onSale = QRandomGenerator::global()->bounded(100) < 30;
        entry.isOnSale = onSale;
        entry.price = onSale ? 80 : rollNormalCardPrice();

        m_cardStock.append(entry);
    }

    for (int i = 0; i < rareCardCount; ++i) {
        ShopCardEntry entry;
        entry.card = createRandomCard(true);
        entry.isRare = true;
        entry.isOnSale = false;
        entry.price = rollRareCardPrice();

        m_cardStock.append(entry);
    }

    const int potionCount = 3;
    for (int i = 0; i < potionCount; ++i) {
        PotionSpawn spawn = createRandomPotion();

        ShopPotionEntry entry;
        entry.potion = spawn.potion;
        entry.price = rollPotionPrice(spawn.potion);

        entry.icon = new QGraphicsPixmapItem(QPixmap(spawn.iconPath).scaled(50,50,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        m_scene->addItem(entry.icon);

        m_potionStock.append(entry);
    }
}

Card *Shop::createRandomCard(bool rare) const
{
    if (!rare) {
        int r = QRandomGenerator::global()->bounded(10);
        switch (r) {
        case 0:
            return new class Strike();
        case 1:
            return new class Bash();
        case 2:
            return new class Clash();
        case 3:
            return new class Hemokinesis();
        case 4:
            return new class BloodForBlood();
        case 5:
            return new class Whirlwind();
        case 6:
            return new class Defend();
        case 7:
            return new class Entrench();
        case 8:
            return new class Bloodletting();
        case 9:
            return new class Inflame();
        default:
            return new class Strike();
        }
    } else {
        int r = QRandomGenerator::global()->bounded(8);
        switch (r) {
        case 0:
            return new class Bludgeon();
        case 1:
            return new class Reaper();
        case 2:
            return new class Feed();
        case 3:
            return new class Immolate();
        case 4:
            return new class Exhume();
        case 5:
            return new class Limit_Break();
        case 6:
            return new class Offering();
        case 7:
            return new class Impervious();
        default:
            return new class Bludgeon();
        }
    }
}

Shop::PotionSpawn Shop::createRandomPotion() const
{
    int r = QRandomGenerator::global()->bounded(5);
    switch (r) {
    case 0:
        return {new BlockPotion(), ":/Potions/Pics/Potions/block_potion.png"};
    case 1:
        return {new FirePotion(), ":/Potions/Pics/Potions/fire_potion.png"};
    case 2:
        return {new EnergyPotion(), ":/Potions/Pics/Potions/energy_potion.png"};
    case 3:
        return {new BlockPotion(), ":/Potions/Pics/Potions/block_potion.png"};
    case 4:
        return {new Fairy_in_a_Bottle(), ":/Potions/Pics/Potions/fairy_in_a_bottle.png"};
    default:
        return {new SwiftPotion(), ":/Potions/Pics/Potions/swift_potion.png"};
    }
}

int Shop::rollNormalCardPrice() const
{
    return 100 + QRandomGenerator::global()->bounded(51); // 100-150
}

int Shop::rollRareCardPrice() const
{
    return 280 + QRandomGenerator::global()->bounded(21); // 280-300
}

int Shop::rollPotionPrice(Potion *potion) const
{
    if (dynamic_cast<Fairy_in_a_Bottle *>(potion))
        return 125;

    return 60 + QRandomGenerator::global()->bounded(21); // 60-80
}

int Shop::currentCardRemovalCost() const
{
    return s_baseRemovalCost + (s_cardRemovalUseCount * s_removalCostStep);
}

void Shop::layoutStockOnScene()
{
    const int cardStartX = 60;
    const int cardY = 60;
    const int cardSpacingX = 200;

    for (int i = 0; i < m_cardStock.size(); ++i) {
        ShopCardEntry &entry = m_cardStock[i];

        entry.card->setFlag(QGraphicsItem::ItemIsMovable, false);
        entry.card->setFlag(QGraphicsItem::ItemIsSelectable, false);
        entry.card->setPos(cardStartX + i * cardSpacingX, cardY);
        m_scene->addItem(entry.card);

        QString label = QString("%1g").arg(entry.price);
        if (entry.isOnSale)
            label = "SALE! " + label;
        if (entry.isRare)
            label = "[RARE] " + label;

        entry.priceTag = new QGraphicsTextItem(label);
        entry.priceTag->setDefaultTextColor(entry.isOnSale ? QColor(80, 220, 100) : Qt::white);
        QFont priceFont("Arial", 11, QFont::Bold);
        entry.priceTag->setFont(priceFont);
        entry.priceTag->setPos(cardStartX + i * cardSpacingX + 10, cardY + 245);
        m_scene->addItem(entry.priceTag);
    }

    const int potionStartX = 60;
    const int potionY = 400;
    const int potionSpacingX = 120;

    for (int i = 0; i < m_potionStock.size(); ++i) {
        ShopPotionEntry &entry = m_potionStock[i];

        entry.icon->setPos(potionStartX + i * potionSpacingX, potionY);
        entry.icon->setScale(1.5);

        entry.priceTag = new QGraphicsTextItem(QString("%1g").arg(entry.price));
        entry.priceTag->setDefaultTextColor(Qt::white);
        QFont priceFont("Arial", 11, QFont::Bold);
        entry.priceTag->setFont(priceFont);
        entry.priceTag->setPos(potionStartX + i * potionSpacingX, potionY + 90);
        m_scene->addItem(entry.priceTag);
    }
}

void Shop::updateGoldDisplay()
{
    m_goldItem->setPlainText(QString("Gold: %1").arg(m_player->coin()));
}

void Shop::updateAffordability()
{
    for (ShopCardEntry &entry : m_cardStock) {
        if (entry.purchased)
            continue;

        bool affordable = m_player->coin() >= entry.price;
        entry.card->setOpacity(affordable ? 1.0 : 0.4);
    }

    bool hasFreeSlot = false;
    for (Potion *p : m_player->Potions())
        if (p == nullptr) {
            hasFreeSlot = true;
            break;
        }

    for (ShopPotionEntry &entry : m_potionStock) {
        if (entry.purchased)
            continue;

        bool affordable = m_player->coin() >= entry.price && hasFreeSlot;
        entry.icon->setOpacity(affordable ? 1.0 : 0.4);
    }

    if (m_removeCardBtn)
        m_removeCardBtn->setEnabled(m_player->coin() >= currentCardRemovalCost());
}

void Shop::onSceneItemClicked(QGraphicsItem *item)
{
    for (int i = 0; i < m_cardStock.size(); ++i) {
        if (m_cardStock[i].card == item && !m_cardStock[i].purchased) {
            buyCard(i);
            return;
        }
    }

    for (int i = 0; i < m_potionStock.size(); ++i) {
        if (m_potionStock[i].icon == item && !m_potionStock[i].purchased) {
            buyPotion(i);
            return;
        }
    }
}

void Shop::buyCard(int index)
{
    ShopCardEntry &entry = m_cardStock[index];

    if (m_player->coin() < entry.price)
        return;

    m_player->setCoin(m_player->coin() - entry.price);
    entry.purchased = true;

    m_scene->removeItem(entry.card);
    m_scene->removeItem(entry.priceTag);
    delete entry.priceTag;
    entry.priceTag = nullptr;

    m_gamePlay->addCardToDeck(entry.card);

    updateGoldDisplay();
    updateAffordability();
}

void Shop::buyPotion(int index)
{
    ShopPotionEntry &entry = m_potionStock[index];

    if (m_player->coin() < entry.price)
        return;

    if (!m_player->addPotion(entry.potion))
        return;

    m_player->setCoin(m_player->coin() - entry.price);
    entry.purchased = true;

    m_scene->removeItem(entry.icon);
    m_scene->removeItem(entry.priceTag);
    delete entry.icon;
    delete entry.priceTag;
    entry.icon = nullptr;
    entry.priceTag = nullptr;

    updateGoldDisplay();
    updateAffordability();
}

void Shop::onRemoveCardClicked()
{
    int cost = currentCardRemovalCost();

    if (m_player->coin() < cost)
        return;

    ShopRemoveCardDialog dialog(m_gamePlay, this);

    if (dialog.exec() == QDialog::Accepted && dialog.cardWasRemoved()) {
        m_player->setCoin(m_player->coin() - cost);
        s_cardRemovalUseCount++;

        m_removeCardBtn->setText(QString("Card Removal - %1g").arg(currentCardRemovalCost()));

        updateGoldDisplay();
        updateAffordability();
    }
}

void Shop::onLeaveClicked()
{
    emit shopFinished();
}

ShopRemoveCardGraphicsView::ShopRemoveCardGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ShopRemoveCardGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *item = itemAt(event->pos());
    if (item)
        emit itemClicked(item);

    QGraphicsView::mousePressEvent(event);
}

ShopRemoveCardDialog::ShopRemoveCardDialog(GamePlay *gamePlay, QWidget *parent)
    : QDialog(parent)
{

    setWindowTitle("Select a Card to Remove");
    resize(900, 500);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 900, 500);

    QLinearGradient bgGradient(0, 0, 0, 500);
    bgGradient.setColorAt(0.0, QColor(35, 30, 45));
    bgGradient.setColorAt(1.0, QColor(15, 12, 20));
    m_scene->setBackgroundBrush(bgGradient);

    m_view = new ShopRemoveCardGraphicsView(m_scene, this);

    connect(m_view, &ShopRemoveCardGraphicsView::itemClicked, this, [this, gamePlay](QGraphicsItem* item) {
                Card* card = dynamic_cast<Card*>(item);
                auto &deck = gamePlay->deck();
                for(auto it = deck.begin(); it != deck.end(); it++)
                {
                    if((*it)->ID() == card->ID())
                    {
                        Card* originaldCard = *it;
                        deck.erase(it);
                        delete originaldCard;
                        delete card;
                        m_cardWasRemoved = true;

                        QMessageBox::information(this,
                                                 "Card Removed",
                                                 "The card has been removed from your deck!");
                        this->accept();
                        break;
                    }
                }
            });

    layout->addWidget(m_view);

    QGraphicsTextItem *title = new QGraphicsTextItem("Select a Card to Remove");
    QFont titleFont("Arial", 16, QFont::Bold);
    title->setFont(titleFont);
    title->setDefaultTextColor(Qt::white);
    title->setPos(20, 10);
    m_scene->addItem(title);

    const int cardStartX = 40;
    const int cardStartY = 70;
    const int cardSpacingX = 210;
    const int cardSpacingY = 260;
    const int perRow = 4;
    bool hasRemovableCards = false;

    int index = 0;
    bool hasUpgradableCards = false;

    if (gamePlay) {
        for (Card *card : gamePlay->deck()) {
            if (!card)
                continue;

            if (card->cardType() == CardType::Status || card->cardType() == CardType::Curse)
                continue;

            hasRemovableCards = true;

            int row = index / perRow;
            int col = index % perRow;

            Card* clone = Card::Creat(static_cast<CardID>(card->ID()));

            clone->setFlag(QGraphicsItem::ItemIsMovable, false);
            clone->setFlag(QGraphicsItem::ItemIsSelectable, false);
            clone->setPos(cardStartX + col * cardSpacingX, cardStartY + row * cardSpacingY);

            m_scene->addItem(clone);

            index++;
        }
    }

    if (!hasRemovableCards) {
        QMessageBox::warning(this, "No Cards", "You don't have any removable cards!");
        QTimer::singleShot(0, this, &QDialog::reject);
    }
}

bool ShopRemoveCardDialog::cardWasRemoved() const
{
    return m_cardWasRemoved;
}