#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

/////////////
class Player;
class Login_Signup;
/////////////

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

signals:
    void playerIsReady(Player* player);
    void exit();

    void startGameClicked();
    void leaderboardClicked();
    void settingsClicked();

private slots:
    void on_SignupButton_clicked();

    void on_Loginbutton_clicked();

    void on_ExitButton_clicked();

    void on_ExitButton_2_clicked();

    void on_StartButton_clicked();

    void on_LeaderBoardButton_clicked();

    void on_Settingbutton_clicked();

private:
    Ui::MainMenu *ui;
    Login_Signup* m_loginSignup;
    QWidget* m_overlay;
};

#endif // MAINMENU_H
