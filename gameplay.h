#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsObject>
#include <QWidget>
#include <algorithm>
#include <random>
#include <QParallelAnimationGroup>

// Temporary player and Enemy;
class Player;
class Enemy;
class Card;
class Potion;
//////////////////////////////

namespace Ui {
class GamePlay;
}

class GamePlay : public QWidget
{
    Q_OBJECT

public:
    explicit GamePlay(Player* player, QWidget *parent = nullptr);
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
    ///
    void addCardToExhaustPile(Card *card);
    void applyBurnDamage();

    Card *selectedHandCard() const;
    void setSelectedHandCard(Card *card);

    void addCardToDeck(Card *card);

    void startCombat();

    void removeTemporaryCardsFromPile(std::vector<Card *> &pile);
    void removeTemporaryCards();
    void endCombat();
    ///
    void addCardToHand(Card *card);
    void drawFromExhaustPile();
    void drawFromDrawPile();

signals:
    void enemiesTurnEnded();
    void playerTurnEnded();
    void playerDead();
    void cardPlayed(Card *);
    void valueChanged();

public slots:
    void playerTurn();

    void enemiesTurn();

    void endTurnButtonClicked();

    void targetCardsHandler(Card *card, Player *player, Enemy *targetEnemy);

    void noTargetCardsHandler(Card *card);

    void playedCardHandler(Card *card);

    void usedPotionHandler(Potion* potion);

    void updateHpLabels();

    void updateEnergyLabel();

    void updatePlayerInformLabels();

    void updateHandsCardsLayout(Card* hoveredCard = nullptr);

    void update();

private:
    Ui::GamePlay *ui;
    Player *m_player;
    std::vector<Enemy *> m_enemys;
    int m_turn;
    QParallelAnimationGroup* m_animGroup = new QParallelAnimationGroup(this);

    /// for dual wield
    Card *m_selectedHandCard = nullptr;
    ///

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;

    void creatEnergyUI();
    QGraphicsTextItem* m_energyLabel;

    // Temporary data structure(piles)
    std::vector<Card *> m_deck; // added by ahoora
    std::vector<Card *> m_drawPile;
    std::vector<Card *> m_discardPile;
    std::vector<Card *> m_ExhaustPile;
    //////////////////////////////////
};

class EndTurnButton : public QGraphicsObject {

    Q_OBJECT;

public:
    explicit EndTurnButton(QGraphicsItem* parent = nullptr);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

signals:
    void onClick();

public slots:
    void activeButton();

protected:
    void loadButtonPixmap(QString pixmapPath);
    void setButtonText(QString plainText);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QGraphicsPixmapItem* m_buttonPicture;
    QGraphicsTextItem* m_plainText;
};

#endif // GAMEPLAY_H
