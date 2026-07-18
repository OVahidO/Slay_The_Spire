#include "gamemanager.h"
#include "encountermanager.h"

#include <QCoreApplication>
#include <QRandomGenerator>
#include <QStackedWidget>

#include "allrelics.h"
#include "campfire.h"
#include "card.h"
#include "database.h"
#include "enemy.h"
#include "event.h"
#include "gameplay.h"
#include "mainmenu.h"
#include "map.h"
#include "player.h"
#include "potion.h"
#include "relic.h"
#include "reward.h"
#include "settings.h"
#include "shop.h"

GameManager::GameManager(QStackedWidget *stack, QObject *parent)
    : QObject(parent)
    , m_stack(stack)
{}

GameManager::~GameManager() {}

void GameManager::start()
{
    Database::open("slaythespire.db");
    Database::creatPlayerTable();
    Database::creatRunStateTable();
    Database::creatRunDeckTable();
    Database::creatRunRelicsTable();
    Database::creatRunPotionsTable();

    showMainMenu();
}

// ==================== Navigation helpers ====================

void GameManager::switchTo(QWidget *page)
{
    if (!page || !m_stack)
        return;

    if (m_stack->indexOf(page) == -1)
        m_stack->addWidget(page);

    m_stack->setCurrentWidget(page);
}

void GameManager::clearTransientScreen(QWidget *screen)
{
    if (!screen || !m_stack)
        return;

    m_stack->removeWidget(screen);
    screen->deleteLater();
}

void GameManager::showMainMenu()
{
    if (!m_mainMenu) {
        m_mainMenu = new MainMenu();
        m_stack->addWidget(m_mainMenu);

        connect(m_mainMenu, &MainMenu::playerIsReady, this, &GameManager::onPlayerReady);
        connect(m_mainMenu, &MainMenu::exit, qApp, &QCoreApplication::quit);
        connect(m_mainMenu, &MainMenu::startGameClicked, this, &GameManager::onMainMenuStart);
        connect(m_mainMenu,
                &MainMenu::leaderboardClicked,
                this,
                &GameManager::onMainMenuLeaderboard);
        connect(m_mainMenu, &MainMenu::settingsClicked, this, &GameManager::onMainMenuSettings);
    }

    switchTo(m_mainMenu);
}

// ==================== Login/Register -> Run lifecycle ====================

void GameManager::onPlayerReady(Player *player)
{
    m_player = player;

    prepareGamePlayForPlayer();

    if (Database::hasActiveRun(m_player->id()))
        resumeRun();
    else
        startNewRun();
}

void GameManager::prepareGamePlayForPlayer()
{
    if (m_gamePlay)
        return;

    m_gamePlay = new GamePlay(m_player);
    m_stack->addWidget(m_gamePlay);

    connect(m_gamePlay, &GamePlay::combatWon, this, &GameManager::onCombatWon);
    connect(m_gamePlay, &GamePlay::playerDead, this, &GameManager::onPlayerDead);
}

void GameManager::grantStarterKit()
{
    if (!m_player || !m_gamePlay)
        return;

    m_gamePlay->grantRelicToPlayer(new BurningBlood());
}

void GameManager::startNewRun()
{
    m_currentAct = 1;
    m_currentFloor = 0;
    m_currentNodeIndex = 0;
    m_usedAct1EncounterTypes.clear();
    m_usedAct2EncounterTypes.clear();

    grantStarterKit();
    buildNewMap();

    autoSaveProgress();
}

void GameManager::resumeRun()
{
    int energy = 3;
    int maxEnergy = 3;
    unsigned int seed = 0;

    Database::loadRunState(m_player->id(),
                           m_currentAct,
                           m_currentFloor,
                           m_currentNodeIndex,
                           energy,
                           maxEnergy,
                           seed);

    m_player->setMaxEnergy(maxEnergy);
    m_player->setEnergy(energy);
    m_mapSeed = seed;

    for (Card *c : m_gamePlay->deck())
        delete c;
    m_gamePlay->deck().clear();

    for (Card *c : Database::loadRunDeck(m_player->id())) {
        c->setOwnerPlayer(m_player);
        m_gamePlay->deck().push_back(c);
    }

    for (const QString &relicName : Database::loadRunRelics(m_player->id())) {
        Relic *relic = createRelicFromName(relicName);
        if (relic)
            m_player->addRelic(relic);
    }

    for (const QString &tag : Database::loadRunPotions(m_player->id())) {
        Potion *potion = createPotionFromTag(tag);
        if (potion)
            m_player->addPotion(potion);
    }

    m_usedAct1EncounterTypes.clear();
    m_usedAct2EncounterTypes.clear();

    buildNewMap();
}

void GameManager::buildNewMap()
{
    if (m_map) {
        m_stack->removeWidget(m_map);
        m_map->deleteLater();
        m_map = nullptr;
    }

    if (m_mapSeed == 0)
        m_mapSeed = QRandomGenerator::global()->generate();

    m_map = new Map(m_mapSeed);
    m_stack->addWidget(m_map);
    connectMapSignals();

    m_map->enableLevel(m_currentFloor);
}

void GameManager::connectMapSignals()
{
    connect(m_map, &Map::nodeSelected, this, &GameManager::onMapNodeSelected, Qt::UniqueConnection);
}

// ==================== MainMenu ====================

void GameManager::onMainMenuStart()
{
    if (!m_map)
        buildNewMap();

    switchTo(m_map);
}

void GameManager::onMainMenuLeaderboard()
{
    showLeaderboard();
}

void GameManager::onMainMenuSettings()
{
    showSettingsPage(SettingsMode::MainMenu);
}

// ==================== Map ====================

void GameManager::onMapNodeSelected(MapButton *button)
{
    if (!button)
        return;

    m_currentFloor = button->levelIndex();
    m_currentNodeIndex = button->levelPosIndex();

    switch (button->buttonType()) {
    case MapButtonType::ENEMY:
        startBattle(MapButtonType::ENEMY);
        break;
    case MapButtonType::ELITE:
        startBattle(MapButtonType::ELITE);
        break;
    case MapButtonType::BOSS:
        startBattle(MapButtonType::BOSS);
        break;
    case MapButtonType::EVENT:
        showEventScreen();
        break;
    case MapButtonType::SHOP:
        showShopScreen();
        break;
    case MapButtonType::CAMPFIRE:
        showCampfireScreen();
        break;
    case MapButtonType::TREASURE:
        handleTreasureNode();
        break;
    }
}

// ==================== Battle ====================

void GameManager::startBattle(MapButtonType type)
{
    if (!m_gamePlay || !m_player)
        return;

    m_gamePlay->clearEnemies();

    bool isElite = (type == MapButtonType::ELITE);
    bool isBoss = (type == MapButtonType::BOSS);

    QVector<int> &usedPool = (m_currentAct == 1) ? m_usedAct1EncounterTypes
                                                 : m_usedAct2EncounterTypes;

    QVector<Enemy *> enemies = EncounterManager::generateEncounter(m_currentAct,
                                                                   m_currentFloor,
                                                                   isElite,
                                                                   isBoss,
                                                                   false,
                                                                   usedPool);

    for (Enemy *e : enemies)
        m_gamePlay->addEnemy(e);

    m_pendingRewardType = isBoss    ? RewardCombatType::Boss
                          : isElite ? RewardCombatType::Elite
                                    : RewardCombatType::Enemy;

    m_gamePlay->startCombat();
    switchTo(m_gamePlay);
}

void GameManager::onCombatWon()
{
    if (!m_gamePlay)
        return;

    m_gamePlay->endCombat();

    m_reward = new RewardScreen(m_player, m_gamePlay, m_pendingRewardType);
    connect(m_reward, &RewardScreen::rewardFinished, this, &GameManager::onRewardFinished);

    m_stack->addWidget(m_reward);
    switchTo(m_reward);
}

void GameManager::onPlayerDead()
{
    if (m_gamePlay)
        m_gamePlay->endCombat();

    showDefeatPage();
}

// ==================== Reward ====================

void GameManager::onRewardFinished()
{
    clearTransientScreen(m_reward);
    m_reward = nullptr;

    bool bossDefeated = (m_currentFloor == 10);

    autoSaveProgress();

    if (bossDefeated) {
        if (m_currentAct >= 2) {
            finishRunAsVictory();
        } else {
            m_currentAct++;
            m_currentFloor = 0;
            m_currentNodeIndex = 0;
            m_usedAct1EncounterTypes.clear();
            m_usedAct2EncounterTypes.clear();
            m_mapSeed = QRandomGenerator::global()->generate();

            buildNewMap();
            autoSaveProgress();
            switchTo(m_map);
        }
    } else {
        switchTo(m_map);
    }
}

// ==================== Campfire / Shop / Event ====================

void GameManager::showEventScreen()
{
    if (m_event)
        clearTransientScreen(m_event);

    m_event = createRandomEvent(m_currentAct, m_player, m_gamePlay, nullptr);

    if (!m_event) {
        //
        autoSaveProgress();
        switchTo(m_map);
        return;
    }

    connect(m_event, &Event::eventFinished, this, &GameManager::onEventFinished);
    connect(m_event, &Event::triggerEliteCombat, this, &GameManager::onEventTriggersEliteCombat);

    m_stack->addWidget(m_event);
    switchTo(m_event);
}

void GameManager::onEventTriggersEliteCombat()
{
    m_pendingIsColosseumElite = true;
}

void GameManager::onEventFinished()
{
    clearTransientScreen(m_event);
    m_event = nullptr;

    if (m_pendingIsColosseumElite) {
        m_pendingIsColosseumElite = false;
        startBattle(MapButtonType::ELITE);
        return;
    }

    autoSaveProgress();
    switchTo(m_map);
}

void GameManager::showShopScreen()
{
    if (m_shop)
        clearTransientScreen(m_shop);

    m_shop = new Shop(m_player, m_gamePlay);
    connect(m_shop, &Shop::shopFinished, this, &GameManager::onShopFinished);

    m_stack->addWidget(m_shop);
    switchTo(m_shop);
}

void GameManager::onShopFinished()
{
    clearTransientScreen(m_shop);
    m_shop = nullptr;

    autoSaveProgress();
    switchTo(m_map);
}

void GameManager::showCampfireScreen()
{
    if (m_campfire)
        clearTransientScreen(m_campfire);

    m_campfire = new Campfire(m_player, m_gamePlay);
    connect(m_campfire, &Campfire::campfireFinished, this, &GameManager::onCampfireFinished);

    m_stack->addWidget(m_campfire);
    switchTo(m_campfire);
}

void GameManager::onCampfireFinished()
{
    clearTransientScreen(m_campfire);
    m_campfire = nullptr;

    autoSaveProgress();
    switchTo(m_map);
}

void GameManager::handleTreasureNode()
{
    // UI Treasure
    if (m_player && m_gamePlay) {
        Relic *relic = Relic::createRandomNormalRelic();
        m_gamePlay->grantRelicToPlayer(relic);

        int gold = 30 + QRandomGenerator::global()->bounded(21); // 30-50
        m_player->setCoin(m_player->coin() + gold);
    }

    autoSaveProgress();
    switchTo(m_map);
}

// ==================== Victory / Defeat / Leaderboard hooks ====================

void GameManager::finishRunAsVictory()
{
    if (m_player) {
        int score = m_player->coin() + m_currentAct * 500 + m_currentFloor * 50;

        Database::updatePlayerScore(m_player->id(), score);
        Database::deleteRunState(m_player->id());
    }

    showVictoryPage();
}

void GameManager::showVictoryPage()
{
    // Victory UI
    emit victoryPageRequested();
    emit runEnded();
}

void GameManager::showDefeatPage()
{
    // Defeat UI
    // GameManager::returnToMainMenuAfterDefeat()
    emit defeatPageRequested();
}

void GameManager::returnToMainMenuAfterDefeat()
{
    if (m_player) {
        Database::deleteRunState(m_player->id());
        Database::saveRunDeck(m_player->id(), {});
        Database::saveRunRelics(m_player->id(), {});
        Database::saveRunPotions(m_player->id(), {});
    }

    resetRunState();
    resetPlayerAndGamePlayForNewRun();

    showMainMenu();
}

void GameManager::showLeaderboard()
{
    // Leaderboard UI
    // leaderboardDataReady UI
    emit leaderboardDataReady(Database::topScores());
}

void GameManager::resetRunState()
{
    m_currentAct = 1;
    m_currentFloor = 0;
    m_currentNodeIndex = 0;
    m_mapSeed = 0;
    m_usedAct1EncounterTypes.clear();
    m_usedAct2EncounterTypes.clear();

    if (m_map) {
        m_stack->removeWidget(m_map);
        m_map->deleteLater();
        m_map = nullptr;
    }
}

void GameManager::resetPlayerAndGamePlayForNewRun()
{
    if (m_gamePlay) {
        m_stack->removeWidget(m_gamePlay);
        m_gamePlay->deleteLater();
        m_gamePlay = nullptr;
    }

    if (m_player) {
        m_player->setCoin(0);
        m_player->setCurrentHPDirect(m_player->maxHP());

        for (Relic *r : m_player->relics())
            delete r;
        m_player->relics().clear();

        QVector<Potion *> &potions = m_player->Potions();
        for (int i = 0; i < potions.size(); ++i) {
            delete potions[i];
            potions[i] = nullptr;
        }
    }
}

// ==================== Persistence ====================

void GameManager::autoSaveProgress()
{
    if (!m_player || !m_gamePlay)
        return;

    Database::updatePlayerValue(m_player);

    Database::saveRunState(m_player->id(),
                           m_currentAct,
                           m_currentFloor,
                           m_currentNodeIndex,
                           m_player->energy(),
                           m_player->maxEnergy(),
                           m_mapSeed);

    Database::saveRunDeck(m_player->id(), m_gamePlay->deck());

    QStringList relicNames;
    for (Relic *r : m_player->relics())
        relicNames << r->name();
    Database::saveRunRelics(m_player->id(), relicNames);

    QStringList potionTags;
    for (Potion *p : m_player->Potions())
        if (p)
            potionTags << potionTypeTag(p);
    Database::saveRunPotions(m_player->id(), potionTags);
}

// ==================== Factories ====================

Relic *GameManager::createRelicFromName(const QString &name) const
{
    if (name == "Face of the Cleric")
        return new FaceOfTheCleric();
    if (name == "Cultist Headpiece")
        return new CultistHeadpiece();
    if (name == "Mutagenic Strength")
        return new MutagenicStrength();
    if (name == "Warped Tongs")
        return new WarpedTongs();
    if (name == "Calling Bell")
        return new CallingBell();
    if (name == "Mark of Pain")
        return new MarkOfPain();
    if (name == "Velvet Choker")
        return new VelvetChoker();
    if (name == "Black Star")
        return new BlackStar();
    if (name == "Girya")
        return new Girya();
    if (name == "Kunai")
        return new Kunai();
    if (name == "Shuriken")
        return new Shuriken();
    if (name == "Ice Cream")
        return new IceCream();
    if (name == "Anchor")
        return new Anchor();
    if (name == "Golden Idol")
        return new GoldenIdol();
    if (name == "Gremlin Visage")
        return new GremlinVisage();
    if (name == "Burning Blood")
        return new BurningBlood();

    return nullptr;
}

QString GameManager::potionTypeTag(Potion *potion) const
{
    if (dynamic_cast<BlockPotion *>(potion))
        return "Block";
    if (dynamic_cast<FirePotion *>(potion))
        return "Fire";
    if (dynamic_cast<EnergyPotion *>(potion))
        return "Energy";
    if (dynamic_cast<SwiftPotion *>(potion))
        return "Swift";
    if (dynamic_cast<Fairy_in_a_Bottle *>(potion))
        return "Fairy";

    return QString();
}

Potion *GameManager::createPotionFromTag(const QString &tag) const
{
    if (tag == "Block")
        return new BlockPotion();
    if (tag == "Fire")
        return new FirePotion();
    if (tag == "Energy")
        return new EnergyPotion();
    if (tag == "Swift")
        return new SwiftPotion();
    if (tag == "Fairy")
        return new Fairy_in_a_Bottle();

    return nullptr;
}

// ==================== Settings ====================

void GameManager::showSettingsPage(SettingsMode mode)
{
    if (!m_settings) {
        m_settings = new SettingsDialog(m_player, mode);
        m_stack->addWidget(m_settings);

        m_settings->setVolume(m_masterVolume);
        m_settings->setMuted(m_isMuted);

        connect(m_settings,
                &SettingsDialog::volumeChanged,
                this,
                &GameManager::onSettingsVolumeChanged);
        connect(m_settings, &SettingsDialog::muteToggled, this, &GameManager::onSettingsMuteToggled);
        connect(m_settings,
                &SettingsDialog::credentialsSaveRequested,
                this,
                &GameManager::onSettingsCredentialsSaveRequested);
        connect(m_settings, &SettingsDialog::returnRequested, this, &GameManager::onSettingsReturn);
        connect(m_settings,
                &SettingsDialog::closeRequested,
                this,
                &GameManager::onSettingsCloseFromMenu);
        connect(m_settings,
                &SettingsDialog::saveAndQuitRequested,
                this,
                &GameManager::onSettingsSaveAndQuit);
        connect(m_settings,
                &SettingsDialog::abandonRunRequested,
                this,
                &GameManager::onSettingsAbandonRun);
    } else {
        m_settings->setMode(mode);
    }

    m_screenBeforeSettings = m_stack->currentWidget();

    switchTo(m_settings);
}

void GameManager::onSettingsReturn()
{
    if (m_screenBeforeSettings)
        switchTo(m_screenBeforeSettings);
    else
        switchTo(m_gamePlay);

    m_screenBeforeSettings = nullptr;
}

void GameManager::onSettingsCloseFromMenu()
{
    if (m_screenBeforeSettings)
        switchTo(m_screenBeforeSettings);
    else
        showMainMenu();

    m_screenBeforeSettings = nullptr;
}

void GameManager::onSettingsSaveAndQuit()
{
    autoSaveProgress();

    m_screenBeforeSettings = nullptr;
    showMainMenu();
}

void GameManager::onSettingsAbandonRun()
{
    returnToMainMenuAfterDefeat();

    m_screenBeforeSettings = nullptr;
}

void GameManager::onSettingsVolumeChanged(int volume)
{
    m_masterVolume = volume;
}

void GameManager::onSettingsMuteToggled(bool muted)
{
    m_isMuted = muted;
}

void GameManager::onSettingsCredentialsSaveRequested(const QString &username,
                                                     const QString &password)
{
    if (!m_player)
        return;

    Database::updateCredentials(m_player->id(), username, password);
}
