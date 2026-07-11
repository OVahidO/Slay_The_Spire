#ifndef SHOP_H
#define SHOP_H

#include <QDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHash>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

class Player;
class GamePlay;
class Card;
class Potion;

class ShopGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ShopGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);

signals:
    void itemClicked(QGraphicsItem *item);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

struct ShopCardEntry
{
    Card *card = nullptr;
    QGraphicsTextItem *priceTag = nullptr;
    int price = 0;
    bool isOnSale = false;
    bool isRare = false;
    bool purchased = false;
};

struct ShopPotionEntry
{
    Potion *potion = nullptr;
    QGraphicsPixmapItem *icon = nullptr;
    QGraphicsTextItem *priceTag = nullptr;
    int price = 0;
    bool purchased = false;
};

class Shop : public QWidget
{
    Q_OBJECT
public:
    explicit Shop(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);
    ~Shop();

signals:
    void shopFinished();

private slots:
    void onSceneItemClicked(QGraphicsItem *item);
    void onRemoveCardClicked();
    void onLeaveClicked();

private:
    Player *m_player;
    GamePlay *m_gamePlay;

    QGraphicsScene *m_scene;
    ShopGraphicsView *m_view;

    QVector<ShopCardEntry> m_cardStock;
    QVector<ShopPotionEntry> m_potionStock;

    QGraphicsTextItem *m_goldItem;
    QPushButton *m_removeCardBtn;
    QPushButton *m_leaveBtn;

    static int s_cardRemovalUseCount;
    static const int s_baseRemovalCost = 50;
    static const int s_removalCostStep = 25;

    void setupScene();
    void generateStock();
    void layoutStockOnScene();
    void updateGoldDisplay();
    void updateAffordability();

    void buyCard(int index);
    void buyPotion(int index);

    Card *createRandomCard(bool rare) const;

    struct PotionSpawn
    {
        Potion *potion;
        QString iconPath;
    };
    PotionSpawn createRandomPotion() const;

    int rollNormalCardPrice() const;
    int rollRareCardPrice() const;
    int rollPotionPrice(Potion *potion) const;
    int currentCardRemovalCost() const;
};

class ShopRemoveCardDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShopRemoveCardDialog(GamePlay *gamePlay, QWidget *parent = nullptr);
    bool cardWasRemoved() const;

private:
    bool m_cardWasRemoved = false;
};

#endif // SHOP_H
