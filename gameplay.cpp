#include "gameplay.h"
#include "attackcards.h"
#include "enemy.h"
#include "player.h"
#include "statuscards.h"
#include "ui_gameplay.h"

GamePlay::GamePlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePlay)
{
    ui->setupUi(this);
    //setup attributes
    //////////////////
    // ahoora's
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 1200, 800);

    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_view);
    this->setLayout(layout);

    Card *testCard = new Strike();
    testCard->setPos(400, 400);

    testCard->setFlag(QGraphicsItem::ItemIsSelectable);
    testCard->setFlag(QGraphicsItem::ItemIsMovable);

    m_scene->addItem(testCard);
    connect(this, &GamePlay::playerTurnEnded, this, &GamePlay::enemiesTurn);
    connect(this, &GamePlay::enemiesTurnEnded, this, &GamePlay::playerTurn);
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
    //m_player->setEnergy(m_player->maxEnergy);
}

void GamePlay::draw()
{
    /*for(int i=0 ; i<m_player->HandSize && !m_drawPile.empty() ; i++)
    {
        m_player->m_HandsCards.push_back(m_drawPile[m_drawPile.size() - 1]);
        m_drawPile.pop_back();
        connect(m_player->m_HandsCards[i], &Card::targetedCard, this, &GamePlay::targetCardsHandler);
        connect(m_player->m_HandsCards[i], &Card::noTargetedCard, this, &GamePlay::noTargetCardsHandler);
    }*/
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
}

bool GamePlay::isEnoughEnergy(int cardEnergyCost)
{
    /*if(m_player->energy >= cardEnergyCost)
    {
        m_player->loseEnergy(cardEnergyCost);
        return true;
    }*/
    return false;
}

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
    if (m_drawPile.empty())
        fillingDrawPile();
    draw();
    //ui->Update;
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
    // emit enemiesTurnEnded();
}
void GamePlay::targetCardsHandler(Card *card, Player *player, Enemy *targetEnemy)
{
    if (card->cardType() == CardType::Attack && player->cannotPlayAttacks())
        return;

    // if(isEnoughEnergy(card->energyCost))
    // {
    //     card->applyEffect(player, targetEnemy);
    //     emit cardPlayed(card);
    //     return;
    // }
}

void GamePlay::noTargetCardsHandler(Card *card)
{
    if (card->cardType() == CardType::Attack && m_player->cannotPlayAttacks())
        return;

    //switch case for handle card effects//
}

void GamePlay::playedCardHandler(Card *card)
{
    m_discardPile.push_back(card);
    //m_player->m_HandsCards.erase(std::find(m_HandsCards.begin(), m_HandsCards.end(), card));
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

// when player selected card call gameplay->setSelectedHandCard(card);

///