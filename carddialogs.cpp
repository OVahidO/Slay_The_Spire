#include "carddialogs.h"
#include "card.h"
#include "event.h"
#include "gameplay.h"

#include <QFont>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <QTimer>
#include <QVBoxLayout>
#include <algorithm>

CardSelectionDialog::CardSelectionDialog(GamePlay *gamePlay,
                                         CardSelectionMode mode,
                                         int maxSelection,
                                         const QString &dialogTitle,
                                         QWidget *parent)
    : QDialog(parent)
    , m_gamePlay(gamePlay)
    , m_mode(mode)
    , m_maxSelection(maxSelection)
{
    setWindowTitle(dialogTitle);
    resize(900, 500);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 900, 500);

    QLinearGradient bgGradient(0, 0, 0, 500);
    bgGradient.setColorAt(0.0, QColor(35, 30, 45));
    bgGradient.setColorAt(1.0, QColor(15, 12, 20));
    m_scene->setBackgroundBrush(bgGradient);

    m_view = new EventGraphicsView(m_scene, this);
    connect(m_view, &EventGraphicsView::itemClicked, this, &CardSelectionDialog::onSceneItemClicked);
    layout->addWidget(m_view);

    QGraphicsTextItem *title = new QGraphicsTextItem(dialogTitle);
    QFont titleFont("Arial", 16, QFont::Bold);
    title->setFont(titleFont);
    title->setDefaultTextColor(Qt::white);
    title->setPos(20, 10);
    m_scene->addItem(title);

    layoutCards();

    if (m_selectableCards.isEmpty()) {
        QMessageBox::warning(this, "No Cards", "You don't have any eligible cards!");
        QTimer::singleShot(0, this, &QDialog::reject);
    }
}

CardSelectionDialog::~CardSelectionDialog()
{
    for (Card *card : m_selectableCards) {
        if (!card)
            continue;
        if (m_scene && m_scene->items().contains(card))
            m_scene->removeItem(card);
        delete card;
    }
}

void CardSelectionDialog::layoutCards()
{
    if (!m_gamePlay)
        return;

    const int cardStartX = 40;
    const int cardStartY = 70;
    const int cardSpacingX = 190;
    const int cardSpacingY = 260;
    const int perRow = 4;

    int index = 0;

    for (Card *card : m_gamePlay->deck()) {
        if (!card)
            continue;

        if (card->cardType() == CardType::Status || card->cardType() == CardType::Curse)
            continue;

        if (m_mode == CardSelectionMode::Upgrade && card->isUpgraded())
            continue;

        Card* clone = Card::Creat(static_cast<CardID>(card->ID()));
        m_selectableCards.append(clone);

        int row = index / perRow;
        int col = index % perRow;

        clone->setFlag(QGraphicsItem::ItemIsMovable, false);
        clone->setFlag(QGraphicsItem::ItemIsSelectable, false);
        clone->setPos(cardStartX + col * cardSpacingX, cardStartY + row * cardSpacingY);

        m_scene->addItem(clone);

        index++;
    }
}

void CardSelectionDialog::onSceneItemClicked(QGraphicsItem *item)
{
    Card *card = qgraphicsitem_cast<Card *>(item);
    if (!card && item->parentItem())
        card = qgraphicsitem_cast<Card *>(item->parentItem());

    if (!card || !m_selectableCards.contains(card))
        return;

    if (m_mode == CardSelectionMode::PickAny) {
        if (m_selectedCards.contains(card)) {
            m_selectedCards.removeAll(card);
            card->setScale(1.0);
        } else if (m_selectedCards.size() < m_maxSelection) {
            m_selectedCards.append(card);
            card->setScale(1.1);
        }

        if (m_selectedCards.size() == m_maxSelection)
            finalizeSelection();

        return;
    }

    m_selectedCards.clear();
    m_selectedCards.append(card);
    finalizeSelection();
}

void CardSelectionDialog::finalizeSelection()
{
    if (m_mode == CardSelectionMode::Upgrade) {
        for (Card *c : m_selectedCards)
            for(auto& card : m_gamePlay->deck())
                if(card->ID() == c->ID() && !card->isUpgraded())
                {
                    card->upgrade();
                    card->update();
                    c->upgrade();
                    break;
                }
    } else if (m_mode == CardSelectionMode::Remove) {
        auto &deck = m_gamePlay->deck();
        for (Card *c : m_selectedCards) {
            for (auto it = deck.begin(); it != deck.end(); it++) {
                if ((*it)->ID() == c->ID()) {
                    Card *originaldCard = *it;
                    deck.erase(it);
                    delete originaldCard;
                    m_scene->removeItem(c);
                    delete c;

                    int idx = m_selectableCards.indexOf(c);
                    if (idx != -1)
                        m_selectableCards[idx] = nullptr;

                    break;
                }
            }
        }
    }

    for (Card *c : m_selectableCards) {
        if (!c)
            continue;
        if (m_scene->items().contains(c))
            m_scene->removeItem(c);
        delete c;
    }
    m_selectableCards.clear();

    accept();
}

QVector<Card *> CardSelectionDialog::selectedCards() const
{
    return m_selectedCards;
}
