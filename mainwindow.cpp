#include "mainwindow.h"
#include "gameplay.h"
#include "player.h"
#include "ui_mainwindow.h"
#include "topbar.h"

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

    connect(m_topbar, &Topbar::potionUsed, g, &GamePlay::usedPotionHandler);
}

MainWindow::~MainWindow()
{
    delete ui;
}
