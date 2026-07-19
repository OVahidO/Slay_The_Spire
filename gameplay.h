#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QParallelAnimationGroup>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include <algorithm>
#include <random>

#include "combatvisuals.h"

class Player;
class Enemy;
class Card;
class Potion;
class Relic;
class Combatant;
class QGraphicsPixmapItem;

namespace Ui {
class GamePlay;
}

class GamePlay : public QWidget
{
    Q_OBJECT

public:
    explicit GamePlay(Player *player, QWidget *parent = nullptr);
    ~GamePlay();

    int turn() const;
    void setTurn(int turn);
    void addTurn(int n = 1);
    Player *&player();

    // --- Enemies management ---
    std::vector<Enemy *> &enemies();
    void addEnemy(Enemy *enemy);
    void clearEnemies();
    bool allEnemiesDead() const;

    void playerReviveEnergy();
    void draw();
    void drawCards(int count);
    void fillingDrawPile();
    bool isEnoughEnergy(int cardEnergyCost);
    int takeDamageToAllEnemies(int damage);
    void addCardToDiscardPile(Card *card);

    void addCardToExhaustPile(Card *card);
    void applyBurnDamage();
    void upgradeAllBurnsInDeck();

    Card *selectedHandCard() const;
    void setSelectedHandCard(Card *card);

    void addCardToDeck(Card *card);
    std::vector<Card *> &deck();

    void addCardToDrawPile(Card *card, bool shuffleIn = true);
    void grantRelicToPlayer(Relic *relic);

    void startCombat();

    void removeTemporaryCardsFromPile(std::vector<Card *> &pile);
    void removeTemporaryCards();
    void endCombat();

    void addCardToHand(Card *card);
    void drawFromExhaustPile();
    void drawFromDrawPile();

    void discardHandToDiscardPile();
    void removeDeadEnemies();

    // ------------------------- Combat VFX -------------------------
    void setupBackground(const QString &imagePath);

    void setupEnemies();

    void showTargetingFrame(Enemy *enemy);
    void hideTargetingFrame();

    void playAttackJolt(Combatant *attacker, bool attackerIsPlayer);

    void showFloatingDamage(Combatant *target, int amount, bool isHeal = false);

    void triggerScreenShake(int intensity = 12, int durationMs = 300);

signals:
    void enemiesTurnEnded();
    void playerTurnEnded();
    void playerDead();
    void combatWon();
    void cardPlayed(Card *);
    void valueChanged();
    void gamePlayIsReady();

public slots:
    void playerTurn();
    void enemiesTurn();
    void endTurnButtonClicked();
    void targetCardsHandler(Card *card, Player *player, Enemy *targetEnemy);
    void noTargetCardsHandler(Card *card);
    void playedCardHandler(Card *card);
    void usedPotionHandler(Potion *potion);
    void updateHpLabels();
    void updateEnergyLabel();
    void updatePlayerInformLabels();
    void updateHandsCardsLayout(Card *hoveredCard = nullptr);
    void update();
    void refreshGamePlay();

private slots:
    void onCardEnemyHoverChanged(Enemy *enemy);

private:
    Ui::GamePlay *ui;
    Player *m_player;
    std::vector<Enemy *> m_enemys;
    int m_turn;
    QParallelAnimationGroup *m_animGroup = new QParallelAnimationGroup(this);

    static const int HAND_MAX_SIZE = 10;
    static const int DRAW_COUNT_PER_TURN = 5;

    /// for dual wield
    Card *m_selectedHandCard = nullptr;

    QGraphicsScene *m_scene;
    QGraphicsView *m_view;

    void creatEnergyUI();
    QGraphicsTextItem *m_energyLabel;

    // Temporary data structure(piles)
    std::vector<Card *> m_deck;
    std::vector<Card *> m_drawPile;
    std::vector<Card *> m_discardPile;
    std::vector<Card *> m_ExhaustPile;
    /////////////////////////////////

    QGraphicsPixmapItem *m_backgroundItem = nullptr;
    TargetFrame *m_targetFrame = nullptr;

    void connectCardVfxSignals(Card *card);
};

class EndTurnButton : public QGraphicsObject {

    Q_OBJECT;

public:
    explicit EndTurnButton(QGraphicsItem *parent = nullptr);

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
    QGraphicsPixmapItem *m_glow = nullptr;
    QGraphicsPixmapItem* m_buttonPicture;
    QGraphicsTextItem* m_plainText;
};

#endif // GAMEPLAY_H
