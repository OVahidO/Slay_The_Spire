#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

/////////////
class Player;
class Login_Signup;

class QLabel;

class QMediaPlayer;
class QVideoWidget;
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

    void resetToLoginScreen();
    void setUsername(const QString &username);
    void tryAutoLogin();

signals:
    void playerIsReady(Player* player);
    void exit();

    void startGameClicked();
    void leaderboardClicked();
    void settingsClicked();

    void multiplayerClicked();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_SignupButton_clicked();

    void on_Loginbutton_clicked();

    void on_ExitButton_clicked();

    void on_ExitButton_2_clicked();

    void on_StartButton_clicked();

    void on_LeaderBoardButton_clicked();

    void on_Settingbutton_clicked();

    void on_MultiplayerButton_clicked();

private:
    Ui::MainMenu *ui;
    Login_Signup* m_loginSignup;
    QWidget* m_overlay;
    QLabel *m_usernameLabel = nullptr;

    QMediaPlayer *m_mediaPlayer = nullptr;
    QVideoWidget *m_videoBackground = nullptr;
};

#endif // MAINMENU_H
