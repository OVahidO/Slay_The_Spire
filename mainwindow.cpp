#include "mainwindow.h"
#include "mainmenu.h"
#include "gameplay.h"
#include "player.h"
#include "map.h"
#include "ui_mainwindow.h"
#include "topbar.h"
#include "shop.h"
#include "event.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1280, 720);
    Player* player = new Player("Vahid&Ahoora", 100);
    m_topbar = new Topbar(player, this);
    ui->verticalLayout->insertWidget(0, m_topbar);
    connect(player, &Player::potionAdded, m_topbar, &Topbar::newPotionHandler);
    GamePlay* g = new GamePlay(player, this);
    int gamePlay_index = ui->stackedWidget->addWidget(g);
    ui->stackedWidget->setCurrentIndex(gamePlay_index);

    // // connect(m_topbar, &Topbar::potionUsed, g, &GamePlay::usedPotionHandler);

    // // Map* m = new Map(0, this);
    // // int map_Index = ui->stackedWidget->addWidget(m);
    // // ui->stackedWidget->setCurrentIndex(map_Index);

    // MainMenu* M = new MainMenu(this);
    // M->show();
    // int MenuIndex = ui->stackedWidget->addWidget(M);
    // ui->stackedWidget->setCurrentIndex(MenuIndex);
    // connect(M, &MainMenu::exit, this, [this](){this->deleteLater();});
    // //connect(M, &MainMenu::playerIsReady, this, ?);

    // m_gameManager = new GameManager(ui->stackedWidget, this);
    // m_gameManager->start();

    player->setCoin(600);
    Shop* sko = new Shop(player, g, this);
    int shopIndex = ui->stackedWidget->addWidget(sko);
    ui->stackedWidget->setCurrentIndex(shopIndex);

    // Event* eve = createRandomEvent(1,player, g, this);
    // int eveIndex = ui->stackedWidget->addWidget(eve);
    // ui->stackedWidget->setCurrentIndex(eveIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}
