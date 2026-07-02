#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QWidget>
#include <algorithm>
#include <random>

// Temporary player and Enemy;
class Player;
class Enemy;
class Card;
//////////////////////////////

namespace Ui {
class GamePlay;
}

class GamePlay : public QWidget
{
    Q_OBJECT

public:
    explicit GamePlay(QWidget *parent = nullptr);
    ~GamePlay();

    int turn() const;
    void setTurn(int turn);
    void addTurn(int n = 1);
    Player *&player();

    void playerReviveEnergy();
    void draw();
    void fillingDrawPile();
    bool isEnoughEnergy(int cardEnergyCost);
    int takeDamageToAllEnemies(int damage);
    void addCardToDiscardPile(Card* card);
    void addCardToHand(Card* card);
    void drawFromExhaustPile();
    void drawFromDrawPile();

signals:
    void enemiesTurnEnded();
    void playerTurnEnded();
    void playerDead();
    void cardPlayed(Card *);

public slots:
    void playerTurn();

    void enemiesTurn();

    void targetCardsHandler(Card *card, Player *player, Enemy *targetEnemy);

    void noTargetCardsHandler(Card *card);

    void playedCardHandler(Card *card);

private:
    Ui::GamePlay *ui;
    Player *m_player;
    std::vector<Enemy *> m_enemys;
    int m_turn;

    // Temporary data structure(piles)
    std::vector<Card *> m_drawPile;
    std::vector<Card *> m_discardPile;
    std::vector<Card *> m_ExhaustPile;
    //////////////////////////////////
};

#endif // GAMEPLAY_H
