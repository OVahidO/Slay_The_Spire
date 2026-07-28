#include "mainmenu.h"
#include <QEvent>
#include <QGraphicsBlurEffect>
#include <QLabel>
#include <QMediaPlayer>
#include <QPushButton>
#include <QUrl>
#include <QVideoWidget>

#include "login_signup.h"
#include "player.h"
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
    connect(m_loginSignup, &Login_Signup::back, this, [this]() { this->m_loginSignup->accept(); });
    connect(m_loginSignup, &Login_Signup::playerIsReady, this, [this](Player *player) {
        ui->MenuKeys->setCurrentIndex(1);
        setUsername(player->name());
        emit playerIsReady(player);
    });

    m_usernameLabel = new QLabel(this);
    m_usernameLabel->setStyleSheet(
        "color: white; font-weight: bold; font-size: 16px; background: transparent;");
    m_usernameLabel->move(15, 15);
    m_usernameLabel->raise();
    m_usernameLabel->hide();

    m_overlay = new QWidget(this);
    m_overlay->setGeometry(rect());
    m_overlay->setStyleSheet("background-color: rgba(0,0,0,120);");
    m_overlay->hide();

    setAutoFillBackground(true);

    QPalette palette = this->palette();
    QPixmap background(":/MainMenu/Pics/MainMenu/main_menu_bg.png");
    palette.setBrush(QPalette::Window,
                     QBrush(background.scaled(this->size(),
                                              Qt::IgnoreAspectRatio,
                                              Qt::SmoothTransformation)));
    this->setPalette(palette);

    // m_videoBackground = new QVideoWidget(this);
    // m_videoBackground->setGeometry(this->rect());
    // m_videoBackground->setAspectRatioMode(Qt::IgnoreAspectRatio);
    // m_videoBackground->lower();

    // m_mediaPlayer = new QMediaPlayer(this);
    // m_mediaPlayer->setVideoOutput(m_videoBackground);
    // m_mediaPlayer->setSource(QUrl("qrc:/MainMenu/Pics/MainMenu/main_menu_bg.mp4"));
    // m_mediaPlayer->setLoops(QMediaPlayer::Infinite);
    // m_mediaPlayer->play();

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

void MainMenu::setUsername(const QString &username)
{
    if (!m_usernameLabel)
        return;

    m_usernameLabel->setText(username);
    m_usernameLabel->adjustSize();
    m_usernameLabel->show();
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

void MainMenu::tryAutoLogin()
{
    if (Player *autoPlayer = m_loginSignup->attemptAutoLogin()) {
        ui->MenuKeys->setCurrentIndex(1);
        setUsername(autoPlayer->name());
        emit playerIsReady(autoPlayer);
    }
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
    emit leaderboardClicked();
}

void MainMenu::on_Settingbutton_clicked()
{
    emit settingsClicked();
}

void MainMenu::on_MultiplayerButton_clicked()
{
    emit multiplayerClicked();
}

void MainMenu::resetToLoginScreen()
{
    ui->MenuKeys->setCurrentIndex(0);

    if (m_usernameLabel)
        m_usernameLabel->hide();

    if (m_loginSignup)
        m_loginSignup->clearLoginFields();
}
