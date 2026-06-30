#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QWidget>

// Temporary player and Enemy;
class Player{};
class Enemy{};
class Card{};
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

private:
    Ui::GamePlay *ui;
    Player* player;
    std::vector<Enemy*> enemys;
    int turn;

    // Temporary data structure(piles)
    std::vector<Card*> drawPile;
    std::vector<Card*> discardPile;
    std::vector<Card*> ExhaustPile;
    //////////////////////////////////
};

#endif // GAMEPLAY_H
