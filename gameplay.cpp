#include "gameplay.h"
#include "ui_gameplay.h"

GamePlay::GamePlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePlay)
{
    ui->setupUi(this);
}

GamePlay::~GamePlay()
{
    delete ui;
}

int GamePlay::turn() const {return m_turn;}
void GamePlay::setTurn(int turn) {m_turn = (turn < 0)? 0 : turn;}
void GamePlay::addTurn(int n) {m_turn += (n < 0)? 0 : n;}

void GamePlay::playerReviveEnergy()
{
    //m_player->setEnergy(m_player->maxEnergy);
}

void GamePlay::draw()
{
    /*for(int i=0 ; i<m_player->HandSize || m_drawPile.empty() ; i++)
    {
        m_player->m_HandsCards[i] = m_drawPile[m_drawPile.size() - 1];
        m_drawPile.pop_back();
    }*/
}

void GamePlay::fillingDrawPile()
{
    if(m_discardPile.empty())
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