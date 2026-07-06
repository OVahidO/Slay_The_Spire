#include "topbar.h"
#include "ui_topbar.h"
#include "player.h"

Topbar::Topbar(Player* player, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Topbar)
{
    ui->setupUi(this);
    m_player = player;
    connect(m_player, &Player::hpChanged, this, &Topbar::updateHpLabel);
    connect(m_player, &Player::coinChanged, this, &Topbar::updateCoinLabel);
    updateHpLabel();
    updateCoinLabel();
}

Topbar::~Topbar()
{
    delete ui;
}

void Topbar::updateHpLabel()
{
    ui->maxHpLabel->setText("/" + QString::number(m_player->maxHP()));
    ui->hpLabel->setText(QString::number(m_player->maxHP()));
}

void Topbar::updateCoinLabel()
{
    ui->coinLabel->setText(QString::number(m_player->coin()));
}
