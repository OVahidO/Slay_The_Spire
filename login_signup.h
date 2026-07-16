#ifndef LOGIN_SIGNUP_H
#define LOGIN_SIGNUP_H

#include <QWidget>
#include <QDialog>

//////////////////
class Player;
/////////////////

namespace Ui {
class Login_Signup;
}

class Login_Signup : public QDialog
{
    Q_OBJECT

public:
    explicit Login_Signup(QWidget *parent = nullptr);
    ~Login_Signup();

public slots:
    void signupButton_clicked();

    void loginButton_clicked();

    void on_usernameInput_editingFinished();

    void on_passwordInput_editingFinished();

    void on_confirimPasswordInput_editingFinished();

    void on_SignupEnterButton_clicked();

    void on_LoginEnterButton_clicked();

signals:
    void back();

    void playerIsReady(Player* player);

private slots:
    void on_BackButton_2_clicked();

    void on_BackButton_clicked();

private:
    Ui::Login_Signup *ui;
    QVector<QPair<Player* , unsigned int>> m_players;
};

#endif // LOGIN_SIGNUP_H
