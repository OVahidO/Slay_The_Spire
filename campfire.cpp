#include "campfire.h"
#include <QMessageBox>
#include <QTimer>
#include "card.h"
#include "gameplay.h"
#include "player.h"
#include "relic.h"

Campfire::Campfire(Player *player, QWidget *parent)
    : QWidget(parent)
    , m_player(player)
{
    setupUI();
    checkAvailableOptions();
}

Campfire::~Campfire() {}

void Campfire::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_restBtn = new QPushButton("Rest (Heal 20% Max HP)", this);
    m_smithBtn = new QPushButton("Smith (Upgrade a Card)", this);
    m_liftBtn = new QPushButton("Lift (Gain 1 Permanent Strength)", this);

    layout->addWidget(m_restBtn);
    layout->addWidget(m_smithBtn);
    layout->addWidget(m_liftBtn);

    connect(m_restBtn, &QPushButton::clicked, this, &Campfire::onRestClicked);
    connect(m_smithBtn, &QPushButton::clicked, this, &Campfire::onSmithClicked);
    connect(m_liftBtn, &QPushButton::clicked, this, &Campfire::onLiftClicked);
}

void Campfire::checkAvailableOptions()
{
    bool hasCoffeeDripper = false;
    bool hasGirya = false;
    Relic *giryaRelic = nullptr;

    for (Relic *r : m_player->relics()) {
        // if (r->name() == "Coffee Dripper") {
        //     hasCoffeeDripper = true;
        // } else
        if (r->name() == "Girya") {
            hasGirya = true;
            giryaRelic = r;
        }
    }

    // if (hasCoffeeDripper) {
    //     m_restBtn->setEnabled(false);
    //     m_restBtn->setText("Rest (Disabled by Coffee Dripper)");
    // }

    if (m_player->currentHP() >= m_player->maxHP()) {
        m_restBtn->setEnabled(false);
        m_restBtn->setText("Rest (Already Full HP)");
    }

    if (!hasGirya || (giryaRelic && giryaRelic->counter() <= 0))
        m_liftBtn->setVisible(false);
}

void Campfire::onRestClicked()
{
    if (!m_player)
        return;

    int healAmount = qRound(m_player->maxHP() * 0.20);
    m_player->heal(healAmount);

    emit campfireFinished();
}

void Campfire::onSmithClicked()
{
    if (!m_gamePlay)
        return;

    UpgradeDialog dialog(m_gamePlay, this);

    if (dialog.exec() == QDialog::Accepted && dialog.cardWasUpgraded())
        emit campfireFinished();
}

void Campfire::onLiftClicked()
{
    if (!m_player)
        return;

    for (Relic *r : m_player->relics()) {
        if (r->name() == "Girya" && r->counter() > 0) {
            r->setCounter(r->counter() - 1);
            // اعمال 1 Strength دائمی روی بازیکن (will be implemented)
            break;
        }
    }

    emit campfireFinished();
}

UpgradeDialog::UpgradeDialog(GamePlay *gamePlay, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Select a Card to Upgrade (Smith)");

    QGridLayout *layout = new QGridLayout(this);

    int row = 0;
    int col = 0;
    bool hasUpgradableCards = false;

    if (gamePlay) {
        for (Card *card : gamePlay->deck()) {
            if (card && !card->isUpgraded() && card->cardType() != CardType::Status
                && card->cardType() != CardType::Curse) {
                hasUpgradableCards = true;

                QPushButton *cardBtn = new QPushButton(card->name(), this);
                cardBtn->setFixedSize(120, 160);

                connect(cardBtn, &QPushButton::clicked, this, [this, card]() {
                    card->upgrade();
                    card->update();

                    m_cardWasUpgraded = true;

                    QMessageBox::information(this,
                                             "Smith Successful",
                                             card->name() + " has been upgraded!");
                    this->accept();
                });

                layout->addWidget(cardBtn, row, col);
                col++;
                if (col > 3) {
                    col = 0;
                    row++;
                }
            }
        }
    }

    if (!hasUpgradableCards) {
        QMessageBox::warning(this, "No Cards", "You don't have any upgradable cards!");
        QTimer::singleShot(0, this, &QDialog::reject);
    }
}

bool UpgradeDialog::cardWasUpgraded() const
{
    return m_cardWasUpgraded;
}