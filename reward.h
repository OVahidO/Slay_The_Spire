#ifndef REWARD_H
#define REWARD_H

#include <QGraphicsScene>
#include <QVector>
#include <QWidget>
#include <functional>

class Player;
class GamePlay;
class Card;
class Relic;
class EventGraphicsView;
class SelectableOptionItem;
class QGraphicsTextItem;
class QPushButton;

enum class RewardCombatType { Enemy, Elite, Boss };

class RewardScreen : public QWidget
{
    Q_OBJECT
public:
    explicit RewardScreen(Player *player,
                          GamePlay *gamePlay,
                          RewardCombatType combatType,
                          QWidget *parent = nullptr);
    ~RewardScreen();

signals:
    void rewardFinished();

private slots:
    void onSceneItemClicked(QGraphicsItem *item);
    void onSkipCardClicked();
    void onContinueClicked();

private:
    Player *m_player;
    GamePlay *m_gamePlay;
    RewardCombatType m_combatType;

    QGraphicsScene *m_scene = nullptr;
    EventGraphicsView *m_view = nullptr;

    QGraphicsTextItem *m_titleItem = nullptr;
    QGraphicsTextItem *m_goldItem = nullptr;
    QGraphicsTextItem *m_potionItem = nullptr;
    QGraphicsTextItem *m_relicGrantedItem = nullptr;
    QGraphicsTextItem *m_cardChoiceLabel = nullptr;
    QGraphicsTextItem *m_relicChoiceLabel = nullptr;

    QVector<Card *> m_cardChoices;
    bool m_cardStageResolved = false;

    QVector<SelectableOptionItem *> m_bossRelicItems;
    QVector<std::function<Relic *()>> m_bossRelicFactories;
    bool m_relicStageResolved = true;

    QPushButton *m_skipCardBtn = nullptr;
    QPushButton *m_continueBtn = nullptr;

    int m_goldAmount = 0;

    void setupScene();
    void grantGoldAndPotion();
    void setupCardChoices();
    void setupBossRelicChoices();
    void updateContinueButtonState();
    void resolveCardStage();

    Card *createRandomCard(bool rare) const;
    Relic *createRandomNormalRelicForElite() const;
};

#endif // REWARD_H
