#include "login_signup.h"
#include "ui_login_signup.h"
#include "player.h"
#include "database.h"

#include <QCheckBox>
#include <QSettings>
Login_Signup::Login_Signup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login_Signup)
{
    ui->setupUi(this);
    this->setGeometry(525,175,500,500);
    m_players = Database::selectAllPlayers();

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    setAttribute(Qt::WA_TranslucentBackground);


    m_rememberMeCheckBox = new QCheckBox("Remember Me", this);
    m_rememberMeCheckBox->setStyleSheet("color: white; font-weight: bold;");
    ui->verticalLayout_4->insertWidget(ui->verticalLayout_4->count() - 1, m_rememberMeCheckBox);
    QSettings settings("SlayTheSpireClone", "Auth");
    if (settings.value("rememberMe", false).toBool()) {
        ui->LoginUsernameInput->setText(settings.value("rememberedUsername").toString());
        ui->LoginPasswordInput->setText(settings.value("rememberedPassword").toString());
        m_rememberMeCheckBox->setChecked(true);
    }
}

Login_Signup::~Login_Signup()
{
    delete ui;
}

void Login_Signup::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPixmap background(":/MainMenu/Pics/MainMenu/login_signup_Bg.png");
    painter.drawPixmap(this->rect(), background);
}

Player *Login_Signup::attemptAutoLogin()
{
    QSettings settings("SlayTheSpireClone", "Auth");
    if (!settings.value("rememberMe", false).toBool())
        return nullptr;

    int playerID = -1;
    if (!Database::validateLogin(settings.value("rememberedUsername").toString(),
                                 settings.value("rememberedPassword").toString(),
                                 playerID))
        return nullptr;

    return Database::loadPlayerById(playerID);
}

void Login_Signup::signupButton_clicked()
{
    ui->SignupEnterButton->setDisabled(true);
    ui->passwordInput->setDisabled(true);
    ui->confirimPasswordInput->setDisabled(true);
    ui->usernameInput->setText("");
    ui->passwordInput->setText("");
    ui->confirimPasswordInput->setText("");
    ui->usernameErrorLabel->setText("");
    ui->passwordErrorLabel->setText("");
    ui->Login_Signup_Stack->setCurrentIndex(1);
}

void Login_Signup::loginButton_clicked()
{
    ui->LoginUsernameInput->setText("");
    ui->LoginPasswordInput->setText("");
    ui->LoginErrorLabel->setText("");
    ui->Login_Signup_Stack->setCurrentIndex(0);
}

void Login_Signup::on_usernameInput_editingFinished()
{
    bool isExists = false;
    QString inputUsername = ui->usernameInput->text();
    for(auto& player : m_players)
    {
        if(inputUsername == player->name())
        {
            isExists = true;
            break;
        }
    }

    if(isExists || inputUsername.isEmpty())
    {
        if(isExists)
            ui->usernameErrorLabel->setText("Error: this username is exists!");
        if(inputUsername.isEmpty())
            ui->usernameErrorLabel->setText("Error: is empty!");
        ui->passwordInput->setDisabled(true);
        ui->confirimPasswordInput->setDisabled(true);
        ui->SignupEnterButton->setDisabled(true);
    }
    else
    {
        ui->usernameErrorLabel->setText("");
        ui->passwordInput->setDisabled(false);
        ui->confirimPasswordInput->setDisabled(false);
    }
}


void Login_Signup::on_passwordInput_editingFinished()
{
    QString inputPass = ui->passwordInput->text();
    if(inputPass.isEmpty())
    {
        ui->confirimPasswordInput->setDisabled(true);
        ui->SignupEnterButton->setDisabled(true);
        ui->passwordErrorLabel->setText("Error: is empty!");
    }
    else if(inputPass.length() < 8)
    {
        ui->confirimPasswordInput->setDisabled(true);
        ui->SignupEnterButton->setDisabled(true);
        ui->passwordErrorLabel->setText("Error: is lower than 8 charachter!");
    }
    else
    {
        ui->confirimPasswordInput->setDisabled(false);
        ui->passwordErrorLabel->setText("");
    }
}


void Login_Signup::on_confirimPasswordInput_editingFinished()
{
    QString inputCPass = ui->confirimPasswordInput->text();
    if(inputCPass.isEmpty())
    {
        ui->passwordErrorLabel->setText("Error: is empty!");
    }
    else
    {
        if(inputCPass == ui->passwordInput->text())
        {
            ui->SignupEnterButton->setEnabled(true);
            ui->passwordErrorLabel->setText("");
        }
        else
        {
            ui->passwordErrorLabel->setText("Error: is not same!");
            ui->SignupEnterButton->setDisabled(true);
        }
    }
}

void Login_Signup::on_SignupEnterButton_clicked()
{
    Player* player = new Player(ui->usernameInput->text(), 100);
    Database::insertPlayerValue(player, ui->passwordInput->text());
    // Database::close() قبلاً اینجا صدا زده می‌شد که باعث می‌شد
    // GameManager دیگر نتواند به دیتابیس دسترسی داشته باشد. حذف شد.

    // Database::close();
    emit playerIsReady(player);
    this->accept();
}

void Login_Signup::on_LoginEnterButton_clicked()
{
    if(ui->LoginUsernameInput->text().isEmpty())
    {
        ui->LoginErrorLabel->setText("Error: username is empty!");
        return;
    }
    if(ui->LoginPasswordInput->text().isEmpty())
    {
        ui->LoginErrorLabel->setText("Error: password is empty!");
        return;
    }

    int playerID = -1;
    if (Database::validateLogin(ui->LoginUsernameInput->text(),
                                ui->LoginPasswordInput->text(),
                                playerID)) {
        Player *player = Database::loadPlayerById(playerID);
        if (player) {
            QSettings settings("SlayTheSpireClone", "Auth");
            if (m_rememberMeCheckBox && m_rememberMeCheckBox->isChecked()) {
                settings.setValue("rememberMe", true);
                settings.setValue("rememberedUsername", ui->LoginUsernameInput->text());
                settings.setValue("rememberedPassword", ui->LoginPasswordInput->text());
            } else {
                settings.setValue("rememberMe", false);
                settings.remove("rememberedUsername");
                settings.remove("rememberedPassword");
            }

            emit playerIsReady(player);
            this->accept();
            return;
        }
    }
    // for(auto& player : m_players)
    // {
    //     if(ui->LoginUsernameInput->text() == player.first->name())
    //     {
    //         // if(ui->LoginPasswordInput->text() == player.first->password())
    //         // {
    //         //     emit playerIsReady(player.first);
    //         //     Database::close();
    //         //     this->accept();
    //         //     return;
    //         // }
    //         // else
    //         //     break;
    //     }
    // }
    ui->LoginErrorLabel->setText("Error: username or password isn't correct!");
}


void Login_Signup::on_BackButton_2_clicked()
{
    emit back();
}


void Login_Signup::on_BackButton_clicked()
{
    emit back();
}

