#include "login_signup.h"
#include "ui_login_signup.h"
#include "player.h"
#include "database.h"

Login_Signup::Login_Signup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login_Signup)
{
    ui->setupUi(this);
    this->setGeometry(525,300,500,500);
    Database::open("");
    m_players = Database::selectAllPlayers();

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    setAttribute(Qt::WA_TranslucentBackground);
}

Login_Signup::~Login_Signup()
{
    Database::close();
    delete ui;
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
        if(inputUsername == player.first->name())
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
    Database::insertPlayerValue(player);
    Database::close();
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
    for(auto& player : m_players)
    {
        if(ui->LoginUsernameInput->text() == player.first->name())
        {
            // if(ui->LoginPasswordInput->text() == player.first->password())
            // {
            //     emit playerIsReady(player.first);
            //     Database::close();
            //     this->accept();
            //     return;
            // }
            // else
            //     break;
        }
    }
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

