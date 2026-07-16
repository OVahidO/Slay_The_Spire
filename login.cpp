#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_LoginEnterButton_clicked()
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
    // for(auto& player : m_players)
    // {
    //     if(ui->LoginUsernameInput->text() == player.first->name())
    //     {
    //         if(ui->LoginPasswordInput->text() == player.first->password())
    //         {
    //             emit playerIsReady(player.first);
    //             emit on_BackButton_clicked();
    //             Database::close();
    //             return;
    //         }
    //         else
    //             break;
    //     }
    // }
    ui->LoginErrorLabel->setText("Error: username or password isn't correct!");
}
