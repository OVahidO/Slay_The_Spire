#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "login_signup.h"
#include <QGraphicsBlurEffect>

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    ui->MenuKeys->setCurrentIndex(0);
    m_loginSignup = new Login_Signup(this);
    connect(m_loginSignup, &Login_Signup::back, this, [this](){this->m_loginSignup->accept();});
    connect(m_loginSignup, &Login_Signup::playerIsReady, this, [this](Player* player)
    {
        ui->MenuKeys->setCurrentIndex(1);
        this->playerIsReady(player);
    });

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
}

MainMenu::~MainMenu()
{
    delete ui;
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

