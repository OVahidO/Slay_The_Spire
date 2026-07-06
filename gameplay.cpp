#include "gameplay.h"
#include "attackcards.h"
#include "skillcards.h"
#include "enemy.h"
#include "player.h"
#include "potion.h"
#include "statuscards.h"
#include "ui_gameplay.h"
#include <QString>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>

GamePlay::GamePlay(Player* player, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePlay)
{
    ui->setupUi(this);
    this->setFixedSize(1280 , 640);
    //setup attributes
    //////////////////

    m_player = player;

    connect(m_player, &Player::hpChanged, this, &GamePlay::updateHpLabels);
    connect(m_player, &Player::energyChanged, this, &GamePlay::updateEnergyLabel);
    connect(m_player, &Player::valueChanged, this, &GamePlay::updatePlayerInformLabels);

    updateHpLabels();

    //////////////////
    // ahoora's
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, width(), height());

    m_view = ui->graphicsView;
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform);
    m_view->setScene(m_scene);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ///////////////////
    creatEnergyUI();
    updateEnergyLabel();

    EndTurnButton* endTurnButton = new EndTurnButton();
    endTurnButton->setPos(1000, 400);
    m_scene->addItem(endTurnButton);
    connect(endTurnButton, &EndTurnButton::onClick, this, &GamePlay::endTurnButtonClicked);
    connect(this, &GamePlay::enemiesTurnEnded, endTurnButton, &EndTurnButton::activeButton);
    //////////////////
    m_drawPile.push_back(new Strike);
    m_drawPile.push_back(new Exhume);
    m_drawPile.push_back(new Strike);
    m_drawPile.push_back(new Defend);
    m_drawPile.push_back(new Defend);
    emit valueChanged();
    draw();
    //////////////////

    connect(this, &GamePlay::playerTurnEnded, this, &GamePlay::enemiesTurn);
    connect(this, &GamePlay::enemiesTurnEnded, this, &GamePlay::playerTurn);
    connect(this, &GamePlay::cardPlayed, this, &GamePlay::playedCardHandler);
    connect(this, &GamePlay::valueChanged, this, &GamePlay::update);
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

void GamePlay::playerReviveEnergy()
{
    m_player->setEnergy(m_player->maxEnergy());
}

void GamePlay::draw()
{
    int* cardsDrawn = new int(0);
    int targetCount = this->m_player->handSize();
    QTimer* drawTimer = new QTimer(this);
    connect(drawTimer, &QTimer::timeout, this, [this, targetCount, cardsDrawn, drawTimer]()
    {
        if(*cardsDrawn >= targetCount || m_drawPile.empty())
        {
            drawTimer->stop();
            drawTimer->deleteLater();
            delete cardsDrawn;
            return;
        }

        Card* card = m_drawPile.back();
        card->setPos(ui->drawPileButton->pos());
        m_player->HandsCards().push_back(card);
        m_scene->addItem(card);
        connect(card, &Card::cardEnterrdMouse, this, &GamePlay::updateHandsCardsLayout);
        connect(card, &Card::cardLeavedMouse, this, &GamePlay::updateHandsCardsLayout);
        connect(card, &Card::targetCardPlayed, this, &GamePlay::targetCardsHandler);
        connect(card, &Card::noTargetCardPlayed, this, &GamePlay::noTargetCardsHandler);
        m_drawPile.pop_back();
        emit valueChanged();

    updateHandsCardsLayout();
    (*cardsDrawn)++;

    });
    drawTimer->start(200);
}

void GamePlay::fillingDrawPile()
{
    if (m_discardPile.empty())
        return;
    std::random_device rand;
    std::mt19937 g(rand());

    std::shuffle(m_discardPile.begin(), m_discardPile.end(), g);

    m_drawPile = m_discardPile;
    m_discardPile.clear();
    emit valueChanged();
}

// bool GamePlay::isEnoughEnergy(int cardEnergyCost)
// {
//     /*if(m_player->energy >= cardEnergyCost)
//     {
//         m_player->loseEnergy(cardEnergyCost);
//         return true;
//     }*/
//     return false;
// }

int GamePlay::takeDamageToAllEnemies(int damage)
{
    int totalDamageDealt = 0;

    for (Enemy *enemy : m_enemys)
        totalDamageDealt += enemy->takeDamage(damage);

    return totalDamageDealt;
}

void GamePlay::addCardToDiscardPile(Card *card)
{
    m_discardPile.push_back(card);
}

///
void GamePlay::addCardToExhaustPile(Card *card)
{
    m_ExhaustPile.push_back(card);
    m_player->triggerPowerEffects(PowerUseTime::OnExhaust, this);
}

void GamePlay::applyBurnDamage()
{
    for (Card *card : m_player->HandsCards())
        if (dynamic_cast<BURN *>(card))
            m_player->takeDamage(2, false);
}
///

void GamePlay::addCardToHand(Card* card)
{
    m_player->HandsCards().push_back(card);
}

void GamePlay::drawFromExhaustPile()
{
    if(!m_ExhaustPile.empty())
    {
        addCardToHand(m_ExhaustPile.back());
        m_ExhaustPile.pop_back();
    }
}

void GamePlay::drawFromDrawPile()
{
    if(!m_drawPile.empty())
    {
        addCardToHand(m_drawPile.back());
        m_drawPile.pop_back();
    }
}

void GamePlay::playerTurn()
{
    //ui->Enable
    addTurn();
    playerReviveEnergy();
    if(m_drawPile.empty())
        fillingDrawPile();
    draw();
    update();
}

void GamePlay::enemiesTurn()
{
    m_player->setCannotPlayAttacks(false);

    // ui->Disable
    // Ahoora->addEnemiesAi...
    // if(m_player->Hp <= 0)
    // {
    //     emit playerDead();
    //     return;
    // }
    emit enemiesTurnEnded();
}

void GamePlay::endTurnButtonClicked()
{
    for(Card* card : m_player->HandsCards())
    {
        emit playedCardHandler(card);
    }
    emit playerTurnEnded();
}
void GamePlay::targetCardsHandler(Card *card, Player *player, Enemy *targetEnemy)
{
    if(m_player->energy() >= card->energyCost())
    {
        card->applyEffect(player, targetEnemy);
        card->applyEffect(this);
        emit cardPlayed(card);
        m_player->loseEnergy(card->energyCost());
    }
}

void GamePlay::noTargetCardsHandler(Card *card)
{
    if(m_player->energy() >= card->energyCost())
    {
        if(card->applyEffect(this))
        {
            emit cardPlayed(card);
            m_player->loseEnergy(card->energyCost());
        }
    }
}

void GamePlay::playedCardHandler(Card *card)
{
    m_player->HandsCards().erase(std::find(m_player->HandsCards().begin(), m_player->HandsCards().end(), card));
    updateHandsCardsLayout();

    card->disconnect();
    card->setZValue(1000);

    QParallelAnimationGroup* cardToDiscardPileAnim = new QParallelAnimationGroup(this);

    QPropertyAnimation* posAnim = new QPropertyAnimation(card, "pos");
    posAnim->setDuration(300);
    posAnim->setStartValue(card->pos());
    if(card->isExhaust())
        posAnim->setEndValue(ui->exhaustPileButton->pos());
    else
        posAnim->setEndValue(ui->discardPileButton->pos());
    posAnim->setEasingCurve(QEasingCurve::InQuad);

    QPropertyAnimation* rotateAnim = new QPropertyAnimation(card, "rotation");
    rotateAnim->setDuration(300);
    rotateAnim->setStartValue(card->rotation());
    rotateAnim->setEndValue(36);
    rotateAnim->setEasingCurve(QEasingCurve::InQuad);

    QPropertyAnimation* scaleAnim = new QPropertyAnimation(card, "scale");
    scaleAnim->setDuration(300);
    scaleAnim->setStartValue(card->scale());
    scaleAnim->setEndValue(0.5);

    cardToDiscardPileAnim->addAnimation(posAnim);
    cardToDiscardPileAnim->addAnimation(rotateAnim);
    cardToDiscardPileAnim->addAnimation(scaleAnim);

    if(card->isExhaust())
        m_ExhaustPile.push_back(card);
    else
        m_discardPile.push_back(card);
    emit valueChanged();

    connect(cardToDiscardPileAnim, &QParallelAnimationGroup::finished, this, [this, card, cardToDiscardPileAnim]()
    {
        m_scene->removeItem(card);

        cardToDiscardPileAnim->deleteLater();

    });

    cardToDiscardPileAnim->start();
}

void GamePlay::usedPotionHandler(Potion* potion)
{
    if(dynamic_cast<FirePotion*>(potion))
        for(Enemy* enemy : m_enemys)
        {
            potion->applyEffect(enemy);
        }
    else
    {
        if(m_player)
            potion->applyEffect(m_player);
    }

    if(m_player)
    {
        auto it = std::find(m_player->Potions().begin(), m_player->Potions().end(), potion);
        if(it != m_player->Potions().end())
        {
            m_player->Potions().erase(it);
            delete potion;
        }
    }
}

void GamePlay::updateHpLabels()
{

}

void GamePlay::updateEnergyLabel()
{
    m_energyLabel->setPlainText(QString::number(m_player->energy()) + "/" + QString::number(m_player->maxEnergy()));
}

void GamePlay::updatePlayerInformLabels()
{

}

void GamePlay::updateHandsCardsLayout(Card* hoveredCard) {
    auto& cards = m_player->HandsCards();
    int cardCount = cards.size();
    if (cardCount == 0) return;

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
    QGraphicsPixmapItem* energyBackground = new QGraphicsPixmapItem();
    QPixmap pixmap(":/icons/Pics/Icons/energyRedVFX.png");
    energyBackground->setPixmap(pixmap);
    energyBackground->setPos(20, 400);
    m_scene->addItem(energyBackground);

    QGraphicsTextItem* energyLabel = new QGraphicsTextItem(energyBackground);
    energyLabel->setPlainText("0 / 3");
    QFont font("Arial", 16, QFont::Bold);
    energyLabel->setFont(font);
    energyLabel->setDefaultTextColor(Qt::red);

    qreal textX = energyBackground->boundingRect().width()/2 - 18;
    qreal textY = energyBackground->boundingRect().height()/2 - 14;

    energyLabel->setPos(textX, textY);
    m_energyLabel = energyLabel;
}

EndTurnButton::EndTurnButton(QGraphicsItem* parent)
    :QGraphicsObject(parent)
{
    this->setAcceptHoverEvents(true);

    m_buttonPicture = new QGraphicsPixmapItem(this);
    m_plainText = new QGraphicsTextItem(m_buttonPicture);

    loadButtonPixmap(":/icons/Pics/Icons/endTurnButton.png");
    setButtonText("End Turn");
}

QRectF EndTurnButton::boundingRect() const
{
    if(m_buttonPicture)
    {
        return m_buttonPicture->boundingRect();
    }
    return QRectF(0, 0, 100, 100);
}

void EndTurnButton::paint(QPainter *painter,
           const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
    Q_UNUSED(painter); Q_UNUSED(option); Q_UNUSED(widget);
}

void EndTurnButton::loadButtonPixmap(QString pixmapPath)
{
    if(m_buttonPicture)
    {
        QPixmap pixmap(pixmapPath);
        m_buttonPicture->setPixmap(pixmap.scaled(350, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void EndTurnButton::setButtonText(QString plainText)
{
    if(m_plainText)
    {
        m_plainText->setPlainText(plainText);
        m_plainText->setFont(QFont("Arial", 14, QFont::Bold));
        qreal textX = (this->boundingRect().width() - m_plainText->boundingRect().width())/2.0;
        qreal textY = (this->boundingRect().height() - m_plainText->boundingRect().height())/2.0;
        m_plainText->setPos(textX, textY);
    }
}

void EndTurnButton::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    if (m_plainText) {
        m_plainText->setDefaultTextColor(Qt::red);
    }

    this->setPos(this->pos().x(), (this->pos().y())-5);

    QGraphicsDropShadowEffect* glowEffect = new QGraphicsDropShadowEffect(this);
    glowEffect->setOffset(0, 0);
    glowEffect->setColor(Qt::white);
    glowEffect->setBlurRadius(20);
    this->setGraphicsEffect(glowEffect);

    QGraphicsObject::hoverEnterEvent(event);
}

void EndTurnButton::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{

    if (m_plainText) {
        m_plainText->setDefaultTextColor(Qt::white);
    }

    this->setPos(this->pos().x(), (this->pos().y())+5);

    this->setGraphicsEffect(nullptr);

    QGraphicsObject::hoverLeaveEvent(event);
}

void EndTurnButton::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        this->setAcceptHoverEvents(false);
        if (m_buttonPicture) {
            m_buttonPicture->setAcceptHoverEvents(false);
        }
        this->setGraphicsEffect(nullptr);
        QGraphicsColorizeEffect* grayEffect = new QGraphicsColorizeEffect(this);
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