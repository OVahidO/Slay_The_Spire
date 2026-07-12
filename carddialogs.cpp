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
            c->upgrade();
    } else if (m_mode == CardSelectionMode::Remove) {
        auto &deck = m_gamePlay->deck();
        for (Card *c : m_selectedCards) {
            auto it = std::find(deck.begin(), deck.end(), c);
            if (it != deck.end()) {
                deck.erase(it);
                m_scene->removeItem(c);
                delete c;
            }
        }
    }

    for (Card *c : m_selectableCards) {
        if (!c)
            continue;
        if (m_mode == CardSelectionMode::Remove && m_selectedCards.contains(c))
            continue; // قبلاً حذف و delete شده
        if (m_scene->items().contains(c))
            m_scene->removeItem(c);
    }

    accept();
}

QVector<Card *> CardSelectionDialog::selectedCards() const
{
    return m_selectedCards;
}
