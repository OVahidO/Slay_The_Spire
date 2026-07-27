#include "topbar.h"
#include "ui_topbar.h"
#include "player.h"
#include "gameplay.h"
#include "potion.h"
#include "piledialog.h"
#include <QGraphicsBlurEffect>

Topbar::Topbar(GamePlay *gameplay, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Topbar)
{
    ui->setupUi(this);
    m_player = gameplay->player();
    m_gameplay = gameplay;
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
    connect(m_player, &Player::potionAdded, this, &Topbar::newPotionHandler);
    connect(m_gameplay, &GamePlay::deckChanged, this, &Topbar::updateDeckSizeLabel);
    ui->userNameLabel->setText(m_player->name());

    updateHpLabel();
    updateCoinLabel();
    updateDeckSizeLabel();

    ////////////////////
    m_overlay = new QWidget(this);
    m_overlay->setGeometry(rect());
    m_overlay->setStyleSheet("background-color: rgba(0,0,0,120);");
    m_overlay->hide();
    ////////////////////
}

Topbar::~Topbar()
{
    delete ui;
}

void Topbar::updateHpLabel()
{
    ui->maxHpLabel->setText("/" + QString::number(m_player->maxHP()));
    ui->hpLabel->setText(QString::number(m_player->currentHP()));
}

void Topbar::updateCoinLabel()
{
    ui->coinLabel->setText(QString::number(m_player->coin()));
}

void Topbar::updateDeckSizeLabel()
{
    ui->deckButton->setText(QString::number(m_gameplay->deck().size()));
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
void Topbar::on_deckButton_clicked()
{
    m_overlay->show();
    auto blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(8);
    this->setGraphicsEffect(blur);
    PileDialog pd(m_gameplay->deck(), this);
    pd.exec();
    this->setGraphicsEffect(nullptr);
    m_overlay->hide();
}


void Topbar::on_settingButton_clicked()
{
    emit settingButton_clicked();
}

