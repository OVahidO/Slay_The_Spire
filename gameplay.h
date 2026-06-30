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
    Player* m_player;
    std::vector<Enemy*> m_enemys;
    int m_turn;

    // Temporary data structure(piles)
    std::vector<Card*> m_drawPile;
    std::vector<Card*> m_discardPile;
    std::vector<Card*> m_ExhaustPile;
    //////////////////////////////////
};

#endif // GAMEPLAY_H
