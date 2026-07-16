#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "database.h"
#include "player.h"

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    Database::open("");
    m_players = Database::selectAllPlayers();
    ui->MenuKeys->setCurrentIndex(0);
    ui->Login_Signup_Stack->setCurrentIndex(2);
}

MainMenu::~MainMenu()
{
    Database::close();
    delete ui;
}

void MainMenu::on_SignupButton_clicked()
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


void MainMenu::on_BackButton_clicked()
{
    ui->Login_Signup_Stack->setCurrentIndex(2);
}


void MainMenu::on_usernameInput_editingFinished()
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
        ui->SignupButton->setDisabled(false);
    }
}


void MainMenu::on_passwordInput_editingFinished()
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


void MainMenu::on_confirimPasswordInput_editingFinished()
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


void MainMenu::on_SignupEnterButton_clicked()
{
    Player* player = new Player(ui->usernameInput->text(), 100);
    Database::insertPlayerValue(player);
    Database::close();
    emit playerIsReady(player);
    emit on_BackButton_clicked();
}


void MainMenu::on_Loginbutton_clicked()
{
    ui->LoginUsernameInput->setText("");
    ui->LoginPasswordInput->setText("");
    ui->LoginErrorLabel->setText("");
    ui->Login_Signup_Stack->setCurrentIndex(0);
}


void MainMenu::on_LoginEnterButton_clicked()
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
        // if(ui->LoginUsernameInput->text() == player.first->name())
        // {
        //     if(ui->LoginPasswordInput->text() == player.first->password())
        //     {
        //         emit playerIsReady(player.first);
        //         emit on_BackButton_clicked();
        //         Database::close();
        //         return;
        //     }
        //     else
        //         break;
        // }
    }
    ui->LoginErrorLabel->setText("Error: username or password isn't correct!");
}

