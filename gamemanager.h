#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QMutex>
#include <QObject>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QVector>

#include "mapButton.h"
#include "reward.h"
#include "settings.h"

class QTcpServer;
class QTcpSocket;

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
class SettingsDialog;

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
    void showSettingsPage(SettingsMode mode);

    // ---- Multiplayer ----
    bool isMultiplayer() const;
    void setMultiplayerMode(bool enabled);
    bool isLeader() const;
    void setLeader(bool leader);

public slots:
    void returnToMainMenuAfterDefeat();

    void onRemoteNodeSelectionReceived(int levelIndex, int levelPosIndex, MapButtonType type);
    void reassignLeaderIfNeeded();

signals:
    void runEnded();
    void victoryPageRequested();
    void defeatPageRequested();
    void leaderboardDataReady(QVector<QPair<QString, int>> scores);

    void leaderChanged(bool isLeaderNow);
    void localNodeSelectionReady(int levelIndex, int levelPosIndex, MapButtonType type);

private slots:
    // Login/MainMenu
    void onPlayerReady(Player *player);
    void onMainMenuStart();
    void onMainMenuLeaderboard();
    void onMainMenuSettings();
    void onSettingsReturn();
    void onSettingsCloseFromMenu();
    void onSettingsSaveAndQuit();
    void onSettingsAbandonRun();
    void onSettingsVolumeChanged(int volume);
    void onSettingsMuteToggled(bool muted);
    void onSettingsCredentialsSaveRequested(const QString &username, const QString &password);

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

    // --- Setting ---
    SettingsDialog *m_settings = nullptr;
    QWidget *m_screenBeforeSettings = nullptr;

    int m_masterVolume = 80;
    bool m_isMuted = false;

    // --- Multiplayer ---
    bool m_isMultiplayer = false;
    bool m_isLeader = true;
    QTcpServer *m_networkServer = nullptr;
    QTcpSocket *m_networkSocket = nullptr;
    QMutex m_networkMutex; // for thread
};

#endif // GAMEMANAGER_H
