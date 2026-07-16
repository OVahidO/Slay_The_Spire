#include "mainwindow.h"
#include "mainmenu.h"
#include "gameplay.h"
#include "player.h"
#include "map.h"
#include "ui_mainwindow.h"
#include "topbar.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1280, 720);
    Player* player = new Player("Vahid&Ahoora", 100);
    // m_topbar = new Topbar(player, this);
    // ui->verticalLayout->insertWidget(0, m_topbar);
    // connect(player, &Player::potionAdded, m_topbar, &Topbar::newPotionHandler);
    GamePlay* g = new GamePlay(player, this);
    int gamePlay_index = ui->stackedWidget->addWidget(g);
    ui->stackedWidget->setCurrentIndex(gamePlay_index);

    // connect(m_topbar, &Topbar::potionUsed, g, &GamePlay::usedPotionHandler);

    // Map* m = new Map(0, this);
    // int map_Index = ui->stackedWidget->addWidget(m);
    // ui->stackedWidget->setCurrentIndex(map_Index);

    MainMenu* M = new MainMenu(this);
    M->show();
    int MenuIndex = ui->stackedWidget->addWidget(M);
    ui->stackedWidget->setCurrentIndex(MenuIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}
