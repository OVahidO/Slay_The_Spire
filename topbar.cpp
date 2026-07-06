#include "topbar.h"
#include "ui_topbar.h"
#include "player.h"
#include "potion.h"

Topbar::Topbar(Player* player, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Topbar)
{
    ui->setupUi(this);
    m_player = player;
//
    m_isInCombat = true;
//
    m_player->Potions().append(new BlockPotion);
    m_player->Potions().append(new FirePotion);
    m_player->Potions().append(new EnergyPotion);
    m_player->Potions().append(new Fairy_in_a_Bottle);

    int i = 0;
    for(Potion* potion : m_player->Potions())
    {
        ui->gridLayout->addWidget(potion, 0, i);
        potion->show();
        m_emptyBottles.append(new emptyBottle);
        m_emptyBottles[i]->hide();
        ui->gridLayout->addWidget(m_emptyBottles[i], 0, i);
        connect(potion, &Potion::potionClicked, this, &Topbar::potionClicked);
        i++;
    }

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

void Topbar::potionClicked(Potion* potion)
{
    if(m_isInCombat)
    {
        int slotIndex = m_player->Potions().indexOf(potion);
        potion->hide();
        m_emptyBottles[slotIndex]->show();
        emit potionUsed(potion);
    }
}