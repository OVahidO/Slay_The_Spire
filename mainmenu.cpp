#include "mainmenu.h"
#include <QEvent>
#include <QGraphicsBlurEffect>
#include <QPushButton>
#include "login_signup.h"
#include "leaderBoard.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    // ui->verticalLayout_3->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    // ui->verticalLayout_3->setContentsMargins(80, 0, 0, 50);
    // ui->verticalLayout_3->setSpacing(6);
    // for (QPushButton *btn :
    //      {ui->StartButton, ui->LeaderBoardButton, ui->Settingbutton, ui->ExitButton_2}) {
    //     btn->setMaximumWidth(240);
    //     btn->setStyleSheet("text-align: left;");
    // }
    ui->MenuKeys->setCurrentIndex(0);
    m_loginSignup = new Login_Signup(this);
    connect(m_loginSignup, &Login_Signup::back, this, [this](){this->m_loginSignup->accept();});
    connect(m_loginSignup, &Login_Signup::playerIsReady, this, [this](Player* player)
    {
        ui->MenuKeys->setCurrentIndex(1);
        this->playerIsReady(player);
    });

    m_leaderBoard = new LeaderBoard(this);

    m_overlay = new QWidget(this);
    m_overlay->setGeometry(rect());
    m_overlay->setStyleSheet("background-color: rgba(0,0,0,120);");
    m_overlay->hide();

    setAutoFillBackground(true);

    QPalette palette = this->palette();
    QPixmap background(":/MainMenu/Pics/MainMenu/main_menu_bg.png");
    palette.setBrush(QPalette::Window, QBrush(background.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);

    QPixmap logo(":/MainMenu/Pics/MainMenu/eng.png");
    ui->Logo->setPixmap(logo.scaled(ui->Logo->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    //
    const QList<QPushButton *> menuButtons = {ui->SignupButton,
                                              ui->Loginbutton,
                                              ui->ExitButton,
                                              ui->StartButton,
                                              ui->LeaderBoardButton,
                                              ui->Settingbutton,
                                              ui->ExitButton_2};

    for (QPushButton *btn : menuButtons) {
        if (!btn)
            continue;
        btn->setProperty("originalText", btn->text());
        btn->installEventFilter(this);
    }
    //
}

MainMenu::~MainMenu()
{
    delete ui;
}

bool MainMenu::eventFilter(QObject *watched, QEvent *event)
{
    QPushButton *btn = qobject_cast<QPushButton *>(watched);
    if (btn) {
        if (event->type() == QEvent::Enter) {
            btn->setText("> " + btn->property("originalText").toString() + " <");
        } else if (event->type() == QEvent::Leave) {
            btn->setText(btn->property("originalText").toString());
        }
    }

    return QWidget::eventFilter(watched, event);
}

void MainMenu::on_SignupButton_clicked()
{
    m_loginSignup->signupButton_clicked();

    m_overlay->show();
    auto blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(8);
    this->setGraphicsEffect(blur);
    m_loginSignup->exec();
    this->setGraphicsEffect(nullptr);
    m_overlay->hide();
}

void MainMenu::on_Loginbutton_clicked()
{
    m_loginSignup->loginButton_clicked();

    m_overlay->show();
    auto blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(8);
    this->setGraphicsEffect(blur);
    m_loginSignup->exec();
    this->setGraphicsEffect(nullptr);
    m_overlay->hide();
}

void MainMenu::on_ExitButton_clicked()
{
    emit exit();
}


void MainMenu::on_ExitButton_2_clicked()
{
    emit exit();
}

void MainMenu::on_StartButton_clicked()
{
    emit startGameClicked();
}

void MainMenu::on_LeaderBoardButton_clicked()
{
    m_overlay->show();
    auto blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(8);
    this->setGraphicsEffect(blur);
    m_leaderBoard->refreshLeaderBoard();
    m_leaderBoard->exec();
    this->setGraphicsEffect(nullptr);
    m_overlay->hide();
    // emit leaderboardClicked();
}

void MainMenu::on_Settingbutton_clicked()
{
    emit settingsClicked();
}

void MainMenu::on_MultiplayerButton_clicked()
{
    emit multiplayerClicked();
}
