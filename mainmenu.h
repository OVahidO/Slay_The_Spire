#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

/////////////
class Player;
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

private slots:
    void on_SignupButton_clicked();

    void on_BackButton_clicked();

    void on_usernameInput_editingFinished();

    void on_passwordInput_editingFinished();

    void on_confirimPasswordInput_editingFinished();

    void on_SignupEnterButton_clicked();

    void on_Loginbutton_clicked();

    void on_LoginEnterButton_clicked();

private:
    Ui::MainMenu *ui;
    QVector<QPair<Player* , unsigned int>> m_players;
};

#endif // MAINMENU_H
