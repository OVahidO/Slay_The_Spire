#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QVector>

#include "mapButton.h"
#include "reward.h"

class Player;
class GamePlay;
class Map;
class MapButton;
class MainMenu;
class Shop;
class Campfire;
class Event;
class RewardScreen;
class Enemy;
class Relic;
class Potion;
class Card;

class QStackedWidget;
class QWidget;

class GameManager : public QObject
{
    Q_OBJECT
public:
    explicit GameManager(QStackedWidget *stack, QObject *parent = nullptr);
    ~GameManager();

    void start();

    // ---- Hook (Victory/Defeat/Leaderboard UI) ----
    void showVictoryPage();
    void showDefeatPage();
    void showLeaderboard();

public slots:
    void returnToMainMenuAfterDefeat();

signals:
    void runEnded();
    void victoryPageRequested();
    void defeatPageRequested();
    void leaderboardDataReady(QVector<QPair<QString, int>> scores);

private slots:
    // Login/MainMenu
    void onPlayerReady(Player *player);
    void onMainMenuStart();
    void onMainMenuLeaderboard();
    void onMainMenuSettings();

    // Map
    void onMapNodeSelected(MapButton *button);

    // Combat
    void onCombatWon();
    void onPlayerDead();

    // Reward / Campfire / Shop
    void onRewardFinished();
    void onCampfireFinished();
    void onShopFinished();

    // Event
    void onEventFinished();
    void onEventTriggersEliteCombat();

private:
    QStackedWidget *m_stack;

    // Persistent
    Player *m_player = nullptr;
    GamePlay *m_gamePlay = nullptr;
    Map *m_map = nullptr;
    MainMenu *m_mainMenu = nullptr;

    // Transient
    Shop *m_shop = nullptr;
    Campfire *m_campfire = nullptr;
    Event *m_event = nullptr;
    RewardScreen *m_reward = nullptr;

    int m_currentAct = 1;
    int m_currentFloor = 0;
    int m_currentNodeIndex = 0;
    unsigned int m_mapSeed = 0;

    RewardCombatType m_pendingRewardType = RewardCombatType::Enemy;
    bool m_pendingIsColosseumElite = false;

    QVector<int> m_usedAct1EncounterTypes;
    QVector<int> m_usedAct2EncounterTypes;

    // --- Navigation helpers ---
    void switchTo(QWidget *page);
    void clearTransientScreen(QWidget *screen);

    void showMainMenu();

    // --- Run lifecycle ---
    void prepareGamePlayForPlayer();
    void startNewRun();
    void resumeRun();
    void buildNewMap();
    void connectMapSignals();
    void grantStarterKit();
    void resetRunState();
    void resetPlayerAndGamePlayForNewRun();
    void finishRunAsVictory();

    // --- Node handling ---
    void startBattle(MapButtonType type);
    void showEventScreen();
    void showShopScreen();
    void showCampfireScreen();
    void handleTreasureNode();

    // --- Persistence ---
    void autoSaveProgress();

    // --- Factories for restoring saved state ---
    Relic *createRelicFromName(const QString &name) const;
    QString potionTypeTag(Potion *potion) const;
    Potion *createPotionFromTag(const QString &tag) const;
};

#endif // GAMEMANAGER_H
