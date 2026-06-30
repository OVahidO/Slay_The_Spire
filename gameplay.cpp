#include "gameplay.h"
#include "ui_gameplay.h"

GamePlay::GamePlay(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePlay)
{
    ui->setupUi(this);
}

GamePlay::~GamePlay()
{
    delete ui;
}
