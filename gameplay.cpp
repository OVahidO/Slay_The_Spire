#include "gameplay.h"

#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QSequentialAnimationGroup>
#include <QString>
#include <QTimer>

#include "allenemies.h"
#include "attackcards.h"
#include "combatvisuals.h"
#include "enemy.h"
#include "player.h"
#include "potion.h"
#include "relic.h"
#include "skillcards.h"
#include "statuscards.h"
#include "ui_gameplay.h"

GamePlay::GamePlay(Player *player, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePlay)
{
    ui->setupUi(this);
    this->setFixedSize(1280, 640);

    m_player = player;

    connect(m_player, &Player::hpChanged, this, &GamePlay::updateHpLabels);
    connect(m_player, &Player::energyChanged, this, &GamePlay::updateEnergyLabel);
    connect(m_player, &Player::valueChanged, this, &GamePlay::updatePlayerInformLabels);

    updateHpLabels();

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, width(), height());

    m_view = ui->graphicsView;
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform);
    m_view->setScene(m_scene);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // setupBackground (":/backgrounds/Pics/Background/combat_placeholder.png");

    creatEnergyUI();
    updateEnergyLabel();

    EndTurnButton *endTurnButton = new EndTurnButton();
    endTurnButton->setPos(1020, 410);
    m_scene->addItem(endTurnButton);
    connect(endTurnButton, &EndTurnButton::onClick, this, &GamePlay::endTurnButtonClicked);
    connect(this, &GamePlay::enemiesTurnEnded, endTurnButton, &EndTurnButton::activeButton);


    m_deck.push_back(new class Strike);
    m_deck.push_back(new class Strike);
    m_deck.push_back(new class Strike);
    m_deck.push_back(new class Strike);
    m_deck.push_back(new class Strike);
    m_deck.push_back(new class Bash);
    m_deck.push_back(new class Defend);
    m_deck.push_back(new class Defend);
    m_deck.push_back(new class Defend);
    m_deck.push_back(new class Defend);

    for (Card *c : m_deck)
        c->setOwnerPlayer(m_player);

    emit valueChanged();

    connect(this, &GamePlay::playerTurnEnded, this, &GamePlay::enemiesTurn);
    connect(this, &GamePlay::enemiesTurnEnded, this, &GamePlay::playerTurn);
    connect(this, &GamePlay::cardPlayed, this, &GamePlay::playedCardHandler);
    connect(this, &GamePlay::valueChanged, this, &GamePlay::update);

    // startCombat() دیگر اینجا صدا زده نمی‌شود.
    // GamePlay اکنون یک نمونه‌ی پایدار در طول کل Run است (چون deck باید بین
    // مبارزات باقی بماند) و شروع واقعیِ هر مبارزه فقط توسط GameManager،
    // درست قبل از افزودن دشمنان به صحنه، با فراخوانی صریح startCombat() انجام می‌شود.

    // startCombat();
}

GamePlay::~GamePlay()
{
    delete ui;
}

int GamePlay::turn() const
{
    return m_turn;
}
void GamePlay::setTurn(int turn)
{
    m_turn = (turn < 0) ? 0 : turn;
}
void GamePlay::addTurn(int n)
{
    m_turn += (n < 0) ? 0 : n;
}
Player *&GamePlay::player()
{
    return m_player;
}

// ---------------- Enemies management ----------------

std::vector<Enemy *> &GamePlay::enemies()
{
    return m_enemys;
}

void GamePlay::addEnemy(Enemy *enemy)
{
    if (!enemy)
        return;

    m_enemys.push_back(enemy);
    m_scene->addItem(enemy);
}

void GamePlay::clearEnemies()
{
    for (Enemy *e : m_enemys) {
        m_scene->removeItem(e);
        delete e;
    }
    m_enemys.clear();
}

bool GamePlay::allEnemiesDead() const
{
    for (Enemy *e : m_enemys)
        if (!e->isDead())
            return false;

    return true;
}

void GamePlay::removeDeadEnemies()
{
    for (auto it = m_enemys.begin(); it != m_enemys.end();) {
        Enemy *e = *it;
        if (e->isDead()) {
            m_scene->removeItem(e);
            it = m_enemys.erase(it);
            e->deleteLater();
        } else {
            ++it;
        }
    }

    if (m_enemys.empty())
        emit combatWon();
}

// ---------------- Energy / Draw ----------------

void GamePlay::playerReviveEnergy()
{
    m_player->setEnergy(m_player->maxEnergy());
}

void GamePlay::draw()
{
    drawCards(DRAW_COUNT_PER_TURN);
}

void GamePlay::drawCards(int count)
{
    int availableSlots = HAND_MAX_SIZE - static_cast<int>(m_player->HandsCards().size());
    int targetCount = qMin(count, qMax(0, availableSlots));

    if (targetCount <= 0)
        return;

    // int *cardsDrawn = new int(0);
    QTimer *drawTimer = new QTimer(this);
    connect(drawTimer,
            &QTimer::timeout,
            this,
            [this, targetCount, drawTimer, cardsDrawn = 0]() mutable {
                if (cardsDrawn >= targetCount
                    || static_cast<int>(m_player->HandsCards().size()) >= HAND_MAX_SIZE) {
                    drawTimer->stop();
                    drawTimer->deleteLater();
                    return;
                }

                if (m_drawPile.empty()) {
                    if (m_discardPile.empty()) {
                        drawTimer->stop();
                        drawTimer->deleteLater();
                        return;
                    }
                    fillingDrawPile();
                }

                if (m_drawPile.empty()) {
                    drawTimer->stop();
                    drawTimer->deleteLater();
                    return;
                }

                Card *card = m_drawPile.back();
                card->setPos(ui->drawPileButton->pos());
                m_player->HandsCards().push_back(card);
                m_scene->addItem(card);
                connect(card, &Card::cardEnteredMouse, this, &GamePlay::updateHandsCardsLayout);
                connect(card, &Card::cardLeavedMouse, this, &GamePlay::updateHandsCardsLayout);
                connect(card, &Card::targetCardPlayed, this, &GamePlay::targetCardsHandler);
                connect(card, &Card::noTargetCardPlayed, this, &GamePlay::noTargetCardsHandler);
                m_drawPile.pop_back();
                emit valueChanged();

                updateHandsCardsLayout();
                cardsDrawn++;
                connectCardVfxSignals(card);
            });

    drawTimer->start(200);
}

void GamePlay::fillingDrawPile()
{
    if (m_discardPile.empty())
        return;

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(m_discardPile.begin(), m_discardPile.end(), g);

    m_drawPile.insert(m_drawPile.end(), m_discardPile.begin(), m_discardPile.end());

    m_discardPile.clear();
    emit valueChanged();
}

bool GamePlay::isEnoughEnergy(int cardEnergyCost)
{
    return m_player->energy() >= cardEnergyCost;
}

int GamePlay::takeDamageToAllEnemies(int damage)
{
    int totalDamageDealt = 0;

    for (Enemy *enemy : m_enemys)
        if (!enemy->isDead())
            totalDamageDealt += enemy->takeDamage(damage);

    removeDeadEnemies();

    return totalDamageDealt;
}

void GamePlay::addCardToDiscardPile(Card *card)
{
    m_discardPile.push_back(card);
}

void GamePlay::addCardToExhaustPile(Card *card)
{
    m_ExhaustPile.push_back(card);
    m_player->triggerPowerEffects(PowerUseTime::OnExhaust, this);
}

void GamePlay::applyBurnDamage()
{
    for (Card *card : m_player->HandsCards()) {
        BURN *burn = dynamic_cast<BURN *>(card);
        if (burn)
            m_player->takeDamage(burn->burnDamage(), false);
    }
}

void GamePlay::upgradeAllBurnsInDeck()
{
    auto upgradeInPile = [](std::vector<Card *> &pile) {
        for (Card *card : pile) {
            BURN *burn = dynamic_cast<BURN *>(card);
            if (burn)
                burn->upgrade();
        }
    };

    upgradeInPile(m_drawPile);
    upgradeInPile(m_discardPile);
    upgradeInPile(m_ExhaustPile);

    for (Card *card : m_player->HandsCards()) {
        BURN *burn = dynamic_cast<BURN *>(card);
        if (burn)
            burn->upgrade();
    }
}

void GamePlay::addCardToDeck(Card *card)
{
    m_deck.push_back(card);
    card->setOwnerPlayer(m_player);
}

void GamePlay::addCardToDrawPile(Card *card, bool shuffleIn)
{
    if (!card)
        return;

    card->setOwnerPlayer(m_player);

    if (!shuffleIn || m_drawPile.empty()) {
        m_drawPile.push_back(card);
        emit valueChanged();
        return;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<size_t> dist(0, m_drawPile.size());
    size_t insertIndex = dist(g);

    m_drawPile.insert(m_drawPile.begin() + insertIndex, card);
    emit valueChanged();
}

void GamePlay::grantRelicToPlayer(Relic *relic)
{
    if (!relic || !m_player)
        return;

    relic->onEquip(this);
    m_player->addRelic(relic);
}

// ---------------- Combat lifecycle ----------------

void GamePlay::startCombat()
{
    m_drawPile.clear();
    m_discardPile.clear();
    m_ExhaustPile.clear();
    setTurn(0);

    std::vector<Card *> innateCards;
    std::vector<Card *> normalCards;

    for (Card *card : m_deck) {
        if (card->isInnate())
            innateCards.push_back(card);
        else
            normalCards.push_back(card);
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(normalCards.begin(), normalCards.end(), g);

    m_drawPile = normalCards;
    m_drawPile.insert(m_drawPile.end(), innateCards.begin(), innateCards.end());

    m_player->setBlock(0);
    m_player->triggerRelicsCombatStart(this);

    for (Enemy *e : m_enemys)
        e->applyBuffDebuff(BuffDebuffType::Strength, 0);

    playerTurn();
}

void GamePlay::removeTemporaryCardsFromPile(std::vector<Card *> &pile)
{
    pile.erase(std::remove_if(pile.begin(),
                              pile.end(),
                              [](Card *card) {
                                  if (card->lifetime() == CardLifetime::EndOfCombat) {
                                      delete card;
                                      return true;
                                  }

                                  return false;
                              }),
               pile.end());
}

void GamePlay::removeTemporaryCards()
{
    std::vector<std::vector<Card *> *> piles = {&m_deck,
                                                &m_drawPile,
                                                &m_discardPile,
                                                &m_ExhaustPile};

    for (auto *pile : piles)
        removeTemporaryCardsFromPile(*pile);

    auto &hand = m_player->HandsCards();

    hand.erase(std::remove_if(hand.begin(),
                              hand.end(),
                              [](Card *card) {
                                  if (card->lifetime() == CardLifetime::EndOfCombat) {
                                      delete card;
                                      return true;
                                  }

                                  return false;
                              }),
               hand.end());
}

void GamePlay::endCombat()
{
    m_player->triggerRelicsCombatEnd();

    removeTemporaryCards();

    m_player->setBlock(0);
    m_player->setBarricade(false);

    for (Card *card : m_player->HandsCards()) {
        if (card->lifetime() == CardLifetime::Permanent)
            m_discardPile.push_back(card);
        m_scene->removeItem(card);
    }

    m_drawPile.clear();
    m_ExhaustPile.clear();
    m_player->HandsCards().clear();

    clearEnemies();
}

std::vector<Card *> &GamePlay::deck()
{
    return m_deck;
}

void GamePlay::addCardToHand(Card *card)
{
    if (static_cast<int>(m_player->HandsCards().size()) >= HAND_MAX_SIZE) {
        addCardToDiscardPile(card);
        return;
    }

    m_player->HandsCards().push_back(card);
    card->setOwnerPlayer(m_player);
    m_scene->addItem(card);
    card->setPos(ui->drawPileButton->pos());

    connect(card, &Card::cardEnteredMouse, this, &GamePlay::updateHandsCardsLayout);
    connect(card, &Card::cardLeavedMouse, this, &GamePlay::updateHandsCardsLayout);
    connect(card, &Card::targetCardPlayed, this, &GamePlay::targetCardsHandler);
    connect(card, &Card::noTargetCardPlayed, this, &GamePlay::noTargetCardsHandler);

    connectCardVfxSignals(card);

    updateHandsCardsLayout();
    emit valueChanged();
}

void GamePlay::drawFromExhaustPile()
{
    if (!m_ExhaustPile.empty()) {
        addCardToHand(m_ExhaustPile.back());
        m_ExhaustPile.pop_back();
    }
}

void GamePlay::drawFromDrawPile()
{
    if (m_drawPile.empty())
        fillingDrawPile();

    if (!m_drawPile.empty()) {
        addCardToHand(m_drawPile.back());
        m_drawPile.pop_back();
    }
}

// ---------------- Turn flow ----------------

void GamePlay::playerTurn()
{
    //ui->Enable
    addTurn();
    playerReviveEnergy();
    if (m_drawPile.empty())
        fillingDrawPile();
    draw();
    update();
}

void GamePlay::enemiesTurn()
{
    m_player->setCannotPlayAttacks(false);

    m_player->triggerPowerEffects(PowerUseTime::EndTurn, this);
    m_player->triggerRelicsTurnEnd();

    applyBurnDamage();

    if (m_player->currentHP() <= 0) {
        emit playerDead();
        return;
    }

    for (size_t i = 0; i < m_enemys.size(); ++i) {
        Enemy *enemy = m_enemys[i];
        if (enemy->isDead())
            continue;

        enemy->applyEnemyIntent(this);

        if (m_player->currentHP() <= 0) {
            emit playerDead();
            return;
        }

        // Slime *slime = dynamic_cast<Slime *>(enemy);
        // if (slime && slime->needsToSplit()) {
        //     QVector<Enemy *> children = slime->createSplitChildren(
        //         m_isMultiplayer); // نکته: پارامتر ثابتِ false باید هماهنگ با حالت بازی شود؛ رجوع به بخش Multiplayer پایین

        //     for (Enemy *child : children)
        //         addEnemy(child);

        //     slime->markSplit();
        //     m_scene->removeItem(enemy);
        //     m_enemys.erase(m_enemys.begin() + i);
        //     enemy->deleteLater();
        //     --i;
        // }
    }

    removeDeadEnemies();

    m_player->tickDecayingBuffDebuff();

    for (Enemy *enemy : m_enemys)
        enemy->tickDecayingBuffDebuff();

    emit enemiesTurnEnded();
}

void GamePlay::discardHandToDiscardPile()
{
    auto handCopy = m_player->HandsCards();

    for (Card *card : handCopy) {
        card->disconnect();
        m_scene->removeItem(card);

        if (card->isExhaust())
            m_ExhaustPile.push_back(card);
        else
            m_discardPile.push_back(card);
    }

    m_player->HandsCards().clear();
    emit valueChanged();
}

void GamePlay::endTurnButtonClicked()
{
    discardHandToDiscardPile();

    emit playerTurnEnded();
}

void GamePlay::targetCardsHandler(Card *card, Player *player, Enemy *targetEnemy)
{
    if (m_player->cannotPlayCards())
        return;

    if (isEnoughEnergy(card->energyCost())) {
        card->applyEffect(player, targetEnemy);
        card->applyEffect(this);
        emit cardPlayed(card);
        m_player->loseEnergy(card->energyCost());

        for (Relic *r : m_player->relics())
            r->onCardPlayed(card, m_player);

        for (Enemy *e : m_enemys)
            if (!e->isDead())
                e->onAnyCardPlayed(card->cardType(), this);

        removeDeadEnemies();
    }
}

void GamePlay::noTargetCardsHandler(Card *card)
{
    if (m_player->cannotPlayCards())
        return;

    if (isEnoughEnergy(card->energyCost())) {
        if (card->applyEffect(this)) {
            emit cardPlayed(card);
            m_player->loseEnergy(card->energyCost());

            for (Relic *r : m_player->relics())
                r->onCardPlayed(card, m_player);

            for (Enemy *e : m_enemys)
                if (!e->isDead())
                    e->onAnyCardPlayed(card->cardType(), this);
        }
    }
}

void GamePlay::playedCardHandler(Card *card)
{
    auto &hand = m_player->HandsCards();
    auto it = std::find(hand.begin(), hand.end(), card);
    if (it != hand.end())
        hand.erase(it);

    updateHandsCardsLayout();

    card->disconnect();
    card->setZValue(1000);

    QParallelAnimationGroup *cardToDiscardPileAnim = new QParallelAnimationGroup(this);

    QPropertyAnimation *posAnim = new QPropertyAnimation(card, "pos");
    posAnim->setDuration(300);
    posAnim->setStartValue(card->pos());
    if (card->isExhaust())
        posAnim->setEndValue(ui->exhaustPileButton->pos());
    else
        posAnim->setEndValue(ui->discardPileButton->pos());
    posAnim->setEasingCurve(QEasingCurve::InQuad);

    QPropertyAnimation *rotateAnim = new QPropertyAnimation(card, "rotation");
    rotateAnim->setDuration(300);
    rotateAnim->setStartValue(card->rotation());
    rotateAnim->setEndValue(36);
    rotateAnim->setEasingCurve(QEasingCurve::InQuad);

    QPropertyAnimation *scaleAnim = new QPropertyAnimation(card, "scale");
    scaleAnim->setDuration(300);
    scaleAnim->setStartValue(card->scale());
    scaleAnim->setEndValue(0.5);

    cardToDiscardPileAnim->addAnimation(posAnim);
    cardToDiscardPileAnim->addAnimation(rotateAnim);
    cardToDiscardPileAnim->addAnimation(scaleAnim);

    bool isExhaustCard = card->isExhaust();

    if (isExhaustCard)
        m_ExhaustPile.push_back(card);
    else
        m_discardPile.push_back(card);
    emit valueChanged();

    if (isExhaustCard)
        m_player->triggerPowerEffects(PowerUseTime::OnExhaust, this);

    connect(cardToDiscardPileAnim,
            &QParallelAnimationGroup::finished,
            this,
            [this, card, cardToDiscardPileAnim]() {
                m_scene->removeItem(card);

                cardToDiscardPileAnim->deleteLater();
            });

    cardToDiscardPileAnim->start();
}

void GamePlay::usedPotionHandler(Potion *potion)
{
    if (dynamic_cast<FirePotion *>(potion)) {
        for (Enemy *enemy : m_enemys)
            potion->applyEffect(enemy);

        removeDeadEnemies();
    } else {
        if (m_player)
            potion->applyEffect(m_player);
    }

    if (m_player) {
        int potionIndex = m_player->Potions().indexOf(potion);
        if (potionIndex != -1)
            m_player->Potions()[potionIndex] = nullptr;

        delete potion;
    }
}

void GamePlay::updateHpLabels() {}

void GamePlay::updateEnergyLabel()
{
    m_energyLabel->setPlainText(QString("%1/%2").arg(m_player->energy()).arg(m_player->maxEnergy()));

    QRectF textRect = m_energyLabel->boundingRect();

    m_energyLabel->setPos((74 - textRect.width()) / 2, (74 - textRect.height()) / 2);
}

void GamePlay::updatePlayerInformLabels() {}

void GamePlay::updateHandsCardsLayout(Card *hoveredCard)
{
    auto &cards = m_player->HandsCards();
    int cardCount = cards.size();
    if (cardCount == 0)
        return;

    int hoveredIndex = -1;
    if (hoveredCard != nullptr) {
        auto it = std::find(cards.begin(), cards.end(), hoveredCard);
        if (it != cards.end()) {
            hoveredIndex = std::distance(cards.begin(), it);
        }
    }

    double radius = 1000.0;
    double cardSpacingAngle = 6.0;
    double maxSpreadAngle = 40.0;

    double totalAngle = (cardCount - 1) * cardSpacingAngle;
    if (totalAngle > maxSpreadAngle) {
        cardSpacingAngle = maxSpreadAngle / (cardCount - 1);
        totalAngle = maxSpreadAngle;
    }

    double centerX = this->width() / 2.25;
    double centerY = this->height() + radius - 200.0;
    double startAngle = -totalAngle / 2.0;
    double spreadOffset = 40.0;

    if (m_animGroup) {
        m_animGroup->stop();
        delete m_animGroup;
        m_animGroup = nullptr;
    }
    m_animGroup = new QParallelAnimationGroup(this);

    for (int i = 0; i < cardCount; ++i) {
        double currentAngle = startAngle + (i * cardSpacingAngle);
        double rad = currentAngle * M_PI / 180.0;

        double targetX = centerX + radius * std::sin(rad);
        double targetY = centerY - radius * std::cos(rad);
        double targetScale = 1.0;
        double targetRotation = currentAngle;
        int zValue = i;

        if (hoveredIndex != -1) {
            if (i < hoveredIndex) {
                targetX -= spreadOffset;
            } else if (i > hoveredIndex) {
                targetX += spreadOffset;
            } else {
                targetY -= 60.0;
                targetScale = 1.2;
                targetRotation = 0;
                zValue = 100;
            }
        }

        cards[i]->setZValue(zValue);

        QPropertyAnimation *posAnim = new QPropertyAnimation(cards[i], "pos");
        posAnim->setDuration(150);
        posAnim->setStartValue(cards[i]->pos());
        posAnim->setEndValue(QPointF(targetX, targetY));
        posAnim->setEasingCurve(QEasingCurve::OutQuad);

        QPropertyAnimation *rotAnim = new QPropertyAnimation(cards[i], "rotation");
        rotAnim->setDuration(150);
        rotAnim->setStartValue(cards[i]->rotation());
        rotAnim->setEndValue(targetRotation);
        rotAnim->setEasingCurve(QEasingCurve::OutQuad);

        m_animGroup->addAnimation(posAnim);
        m_animGroup->addAnimation(rotAnim);

        cards[i]->setScale(targetScale);
    }

    m_animGroup->start();
}

void GamePlay::update()
{
    ui->drawPileButton->setText(QString::number(m_drawPile.size()));
    ui->discardPileButton->setText(QString::number((m_discardPile.size())));
    ui->exhaustPileButton->setText(QString::number(m_ExhaustPile.size()));
}

void GamePlay::creatEnergyUI()
{
    QGraphicsPixmapItem *energyBackground = new QGraphicsPixmapItem();
    QPixmap pixmap(":/icons/Pics/Icons/energyRedVFX.png");
    energyBackground->setPixmap(pixmap);
    energyBackground->setPos(20, 400);
    m_scene->addItem(energyBackground);

    QGraphicsTextItem *energyLabel = new QGraphicsTextItem(energyBackground);
    energyLabel->setPlainText("0 / 3");
    QFont font("Belwe Bd BT", 14, QFont::Bold);

    energyLabel->setFont(font);
    energyLabel->setDefaultTextColor(Qt::white);

    QRectF textRect = energyLabel->boundingRect();
    QRectF bgRect = energyBackground->boundingRect();

    energyLabel->setPos((bgRect.width() - textRect.width()) / 2,
                        (bgRect.height() - textRect.height()) / 2);
    m_energyLabel = energyLabel;
}

EndTurnButton::EndTurnButton(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    this->setAcceptHoverEvents(true);

    m_buttonPicture = new QGraphicsPixmapItem(this);
    m_glow = new QGraphicsPixmapItem(this);

    QPixmap glow(":/icons/Pics/Icons/end_turn_button_glow.png");

    m_glow->setPixmap(glow.scaled(m_buttonPicture->pixmap().size(),
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation));

    m_glow->setVisible(false);
    m_glow->setZValue(-1);
    m_plainText = new QGraphicsTextItem(m_buttonPicture);

    loadButtonPixmap(":/icons/Pics/Icons/endTurnButton.png");
    setButtonText("End Turn");
}

QRectF EndTurnButton::boundingRect() const
{
    if (m_buttonPicture) {
        return m_buttonPicture->boundingRect();
    }
    return QRectF(0, 0, 100, 100);
}

void EndTurnButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void EndTurnButton::loadButtonPixmap(QString pixmapPath)
{
    if (m_buttonPicture) {
        QPixmap pixmap(pixmapPath);
        m_buttonPicture->setPixmap(
            pixmap.scaled(240, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void EndTurnButton::setButtonText(QString plainText)
{
    if (m_plainText) {
        m_plainText->setPlainText(plainText);
        m_plainText->setFont(QFont("Arial", 14, QFont::Bold));
        qreal textX = (this->boundingRect().width() - m_plainText->boundingRect().width()) / 2.0;
        qreal textY = (this->boundingRect().height() - m_plainText->boundingRect().height()) / 2.0;
        m_plainText->setPos(textX, textY);
    }
}

void EndTurnButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_glow->setVisible(true);

    if (m_plainText) {
        m_plainText->setDefaultTextColor(Qt::red);
    }

    this->setPos(this->pos().x(), (this->pos().y()) - 5);

    QGraphicsDropShadowEffect *glowEffect = new QGraphicsDropShadowEffect(this);
    glowEffect->setOffset(0, 0);
    glowEffect->setColor(Qt::white);
    glowEffect->setBlurRadius(20);
    this->setGraphicsEffect(glowEffect);

    QGraphicsObject::hoverEnterEvent(event);
}

void EndTurnButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_glow->setVisible(false);

    if (m_plainText) {
        m_plainText->setDefaultTextColor(Qt::white);
    }

    this->setPos(this->pos().x(), (this->pos().y()) + 5);

    this->setGraphicsEffect(nullptr);

    QGraphicsObject::hoverLeaveEvent(event);
}

void EndTurnButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->setAcceptHoverEvents(false);
        if (m_buttonPicture) {
            m_buttonPicture->setAcceptHoverEvents(false);
        }
        this->setGraphicsEffect(nullptr);
        QGraphicsColorizeEffect *grayEffect = new QGraphicsColorizeEffect(this);
        grayEffect->setColor(Qt::black);
        grayEffect->setStrength(0.9);
        this->setGraphicsEffect(grayEffect);

        this->setButtonText("Enemy Turn");
        m_plainText->setDefaultTextColor(Qt::gray);

        emit onClick();
    }
    QGraphicsObject::mousePressEvent(event);
}

void EndTurnButton::activeButton()
{
    this->setAcceptHoverEvents(true);
    this->m_buttonPicture->setAcceptHoverEvents(true);
    this->setGraphicsEffect(nullptr);
    this->setButtonText("End Turn");
    this->m_plainText->setDefaultTextColor(Qt::white);
}

/// for dual wield
Card *GamePlay::selectedHandCard() const
{
    return m_selectedHandCard;
}

void GamePlay::setSelectedHandCard(Card *card)
{
    m_selectedHandCard = card;
}

void GamePlay::connectCardVfxSignals(Card *card)
{
    connect(card, &Card::enemyHoverChanged, this, &GamePlay::onCardEnemyHoverChanged);
}

void GamePlay::onCardEnemyHoverChanged(Enemy *enemy)
{
    if (enemy)
        showTargetingFrame(enemy);
    else
        hideTargetingFrame();
}

void GamePlay::setupBackground(const QString &imagePath)
{
    if (m_backgroundItem) {
        m_scene->removeItem(m_backgroundItem);
        delete m_backgroundItem;
        m_backgroundItem = nullptr;
    }

    QPixmap bg(imagePath); // Placeholder: مسیر واقعی رو خودت جایگزین کن
    if (bg.isNull())
        return;

    bg = bg.scaled(m_scene->sceneRect().size().toSize(),
                   Qt::IgnoreAspectRatio,
                   Qt::SmoothTransformation);

    m_backgroundItem = m_scene->addPixmap(bg);
    m_backgroundItem->setZValue(-100);
    m_backgroundItem->setPos(0, 0);

    // اگر گیف انیمیشنی خواستی: به‌جای QPixmap از QMovie + QGraphicsProxyWidget(QLabel)
    // استفاده کن، چون QGraphicsPixmapItem فریم‌های متحرک را پشتیبانی نمی‌کند.
}

void GamePlay::showTargetingFrame(Enemy *enemy)
{
    if (!enemy || enemy->isDead())
        return;

    if (!m_targetFrame) {
        m_targetFrame = new TargetFrame();
        // TODO: مسیر ۴ عکس گوشه قاب هدف‌گیری رو اینجا بذار
        m_targetFrame->setCornerPixmaps(":/icons/Pics/Icons/target/corner_tl.png",
                                        ":/icons/Pics/Icons/target/corner_tr.png",
                                        ":/icons/Pics/Icons/target/corner_bl.png",
                                        ":/icons/Pics/Icons/target/corner_br.png");
        m_scene->addItem(m_targetFrame);
    }

    m_targetFrame->attachToTarget(enemy);
}

void GamePlay::hideTargetingFrame()
{
    if (m_targetFrame)
        m_targetFrame->hideFrame();
}

void GamePlay::playAttackJolt(Combatant *attacker, bool attackerIsPlayer)
{
    if (!attacker)
        return;

    QPointF originalPos = attacker->pos();
    qreal joltDistance = attackerIsPlayer ? 40.0 : -40.0;

    QPropertyAnimation *forward = new QPropertyAnimation(attacker, "pos");
    forward->setDuration(120);
    forward->setStartValue(originalPos);
    forward->setEndValue(QPointF(originalPos.x() + joltDistance, originalPos.y()));
    forward->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation *backward = new QPropertyAnimation(attacker, "pos");
    backward->setDuration(150);
    backward->setStartValue(QPointF(originalPos.x() + joltDistance, originalPos.y()));
    backward->setEndValue(originalPos);
    backward->setEasingCurve(QEasingCurve::InQuad);

    QSequentialAnimationGroup *joltSequence = new QSequentialAnimationGroup(this);
    joltSequence->addAnimation(forward);
    joltSequence->addAnimation(backward);

    connect(joltSequence, &QSequentialAnimationGroup::finished, joltSequence, &QObject::deleteLater);

    joltSequence->start();
}

void GamePlay::showFloatingDamage(Combatant *target, int amount, bool isHeal)
{
    if (!target || amount == 0)
        return;

    FloatingDamageText *floatingText = new FloatingDamageText(amount, isHeal);

    QRectF targetRect = target->boundingRect();
    QPointF spawnPos = target->pos() + QPointF(targetRect.width() / 2.0, targetRect.height() / 3.0);

    floatingText->setPos(spawnPos);
    floatingText->setZValue(2000);
    m_scene->addItem(floatingText);

    connect(floatingText, &FloatingDamageText::finished, this, [this, floatingText]() {
        m_scene->removeItem(floatingText);
        floatingText->deleteLater();
    });

    floatingText->play();
}

void GamePlay::triggerScreenShake(int intensity, int durationMs)
{
    if (!m_view)
        return;

    QTimer *shakeTimer = new QTimer(this);
    auto elapsed = std::make_shared<int>(0);
    const int stepInterval = 30; // ms

    connect(shakeTimer, &QTimer::timeout, this, [this, shakeTimer, intensity, durationMs, elapsed]() {
        *elapsed += stepInterval;

        if (*elapsed >= durationMs) {
            m_view->resetTransform();
            shakeTimer->stop();
            shakeTimer->deleteLater();
            return;
        }

        int dx = QRandomGenerator::global()->bounded(-intensity, intensity + 1);
        int dy = QRandomGenerator::global()->bounded(-intensity, intensity + 1);

        m_view->resetTransform();
        m_view->translate(dx, dy);
    });

    shakeTimer->start(stepInterval);
}

void GamePlay::addRemotePlayer(Player *player)
{
    if (!player || m_remotePlayers.contains(player))
        return;

    m_remotePlayers.append(player);
    player->setIsLocalPlayer(false);

    connect(player, &Player::hpChanged, this, [this, player]() {
        if (player->currentHP() <= 0)
            emit playerEliminated(player);
    });
}

Player *GamePlay::remotePlayer() const
{
    return m_remotePlayers.isEmpty() ? nullptr : m_remotePlayers.first();
}

QVector<Player *> GamePlay::allPlayers() const
{
    QVector<Player *> result;
    if (m_player)
        result.append(m_player);
    result += m_remotePlayers;
    return result;
}

bool GamePlay::allPlayersDead() const
{
    for (Player *p : allPlayers())
        if (p && p->currentHP() > 0)
            return false;
    return true;
}

bool GamePlay::isCoopMode() const
{
    return m_coopMode;
}

void GamePlay::setCoopMode(bool enabled)
{
    m_coopMode = enabled;
}
