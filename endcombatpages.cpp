#include "endcombatpages.h"
#include "ui_endcombatpages.h"
#include "player.h"
#include "database.h"
#include <QPainter>

endCombatPages::endCombatPages(Player* player, endMod endMod, QWidget *parent)
    : QDialog(parent)
    , m_player(player)
    , ui(new Ui::endCombatPages)
{
    ui->setupUi(this);

    this->setGeometry(525,100,500,625);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    setAttribute(Qt::WA_TranslucentBackground);

    if(endMod == endMod::Victory)
    {
        ui->endPageLabel->setText("Victory");
    }
    else
    {
        ui->endPageLabel->setText("Defate");
    }

    int score = m_player->scoreDetails().nBoss*10000 + m_player->scoreDetails().nElit*5000 + m_player->scoreDetails().floor*100 + m_player->scoreDetails().damage;
    m_player->setScore(score);

    QLabel* nBossLabel = new QLabel("Boss .........."
                                        ".........."
                                        ".........."
                                        ".......... "
                                        + QString::number(m_player->scoreDetails().nBoss)
                                        + " x10000", this);

    QLabel* nElitLabel = new QLabel("Elite .........."
                                    ".........."
                                    ".........."
                                    ".......... "
                                    + QString::number(m_player->scoreDetails().nElit)
                                    + " x5000", this);

    QLabel* levelLabel = new QLabel("Level .........."
                                    ".........."
                                    ".........."
                                    ".......... "
                                    + QString::number(m_player->scoreDetails().floor)
                                    + " x100", this);

    QLabel* damageLabel = new QLabel("Damage .........."
                                    ".........."
                                    ".........."
                                    ".......... "
                                        + QString::number(m_player->scoreDetails().damage), this);

    QLabel* spaceLabel = new QLabel(this);

    QLabel* scoreLabel = new QLabel("score .........."
                                     ".........."
                                     ".........."
                                     ".......... "
                                         + QString::number(m_player->score()), this);

    nBossLabel->setAlignment(Qt::AlignCenter);
    ui->scoreDetailsLayout->addWidget(nBossLabel);
    nElitLabel->setAlignment(Qt::AlignCenter);
    ui->scoreDetailsLayout->addWidget(nElitLabel);
    levelLabel->setAlignment(Qt::AlignCenter);
    ui->scoreDetailsLayout->addWidget(levelLabel);
    damageLabel->setAlignment(Qt::AlignCenter);
    ui->scoreDetailsLayout->addWidget(damageLabel);
    spaceLabel->setAlignment(Qt::AlignCenter);
    ui->scoreDetailsLayout->addWidget(spaceLabel);
    scoreLabel->setAlignment(Qt::AlignCenter);
    ui->scoreDetailsLayout->addWidget(scoreLabel);

    m_player->scoreDetails().damage = 0;
    m_player->scoreDetails().floor = 0;
    m_player->scoreDetails().nBoss = 0;
    m_player->scoreDetails().nElit = 0;

    Database::saveScoreDetails(m_player->id(), m_player->scoreDetails());
    Database::updatePlayerScore(m_player->id(), m_player->score());
}

endCombatPages::~endCombatPages()
{
    delete ui;
}

void endCombatPages::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPixmap background(":/EndCombatPages/Pics/EndCombatPages/Bg2.png");
    painter.drawPixmap(this->rect(), background);
}

void endCombatPages::on_endPageButton_clicked()
{
    this->accept();
}

