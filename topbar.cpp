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
    for(int i=0; i<m_emptyBottles.size(); i++)
    {
        m_emptyBottles[i] = (new emptyBottle(this));
        ui->gridLayout->addWidget(m_emptyBottles[i], 0, i);
        m_emptyBottles[i]->show();
    }

    connect(m_player, &Player::hpChanged, this, &Topbar::updateHpLabel);
    connect(m_player, &Player::coinChanged, this, &Topbar::updateCoinLabel);
    connect(m_player, &Player::takedDamage, this, &Topbar::updateHpLabel);
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

        // ui->gridLayout->removeWidget(potion);
        // potion->setParent(nullptr);

        // potion->setEnabled(false);
        potion->hide();

        m_emptyBottles[slotIndex]->show();

        emit potionUsed(potion);
    }
}

void Topbar::newPotionHandler(Potion* potion)
{
    int potionIndex = m_player->Potions().indexOf(potion);
    if(potionIndex != -1)
    {
        m_emptyBottles[potionIndex]->hide();
        potion->setParent(this);
        ui->gridLayout->addWidget(potion, 0, potionIndex);
        potion->show();

        connect(potion, &Potion::potionClicked, this, &Topbar::potionClicked);
    }
}