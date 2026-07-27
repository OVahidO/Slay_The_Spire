#include "leaderBoard.h"
#include "ui_leaderboard.h"
#include "database.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLayoutItem>
#include <QPainter>

LeaderBoard::LeaderBoard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LeaderBoard)
{
    ui->setupUi(this);
    this->setGeometry(525,100,500,500);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    setAttribute(Qt::WA_TranslucentBackground);
}

LeaderBoard::~LeaderBoard()
{
    delete ui;
}

void LeaderBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPixmap background(":/MainMenu/Pics/MainMenu/LeaderBoardBg.png");
    painter.drawPixmap(this->rect(), background);
}


void LeaderBoard::refreshLeaderBoard()
{
    QLayoutItem *item;
    while ((item = ui->PlayersVLayout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    auto topScorePlayers = Database::topScores();
    for(auto& topScorePlayer : topScorePlayers)
    {
        QLabel* Player_Score_Label = new QLabel(topScorePlayer.first + " .........."
                                                                       ".........."
                                                                       ".........."
                                                                       ".........."
                                                                       ".......... "
                                                                    + QString::number(topScorePlayer.second), this);
        Player_Score_Label->setAlignment(Qt::AlignCenter);
        ui->PlayersVLayout->addWidget(Player_Score_Label);
    }
}

void LeaderBoard::on_BackButton_clicked()
{
    this->accept();
}

