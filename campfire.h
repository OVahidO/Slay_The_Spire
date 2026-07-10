#ifndef CAMPFIRE_H
#define CAMPFIRE_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class Player;
class GamePlay;
class Relic;

class Campfire : public QWidget
{
    Q_OBJECT
public:
    explicit Campfire(Player *player, QWidget *parent = nullptr);
    ~Campfire();

signals:
    void campfireFinished();
    void requestCardUpgrade();

private slots:
    void onRestClicked();
    void onSmithClicked();
    void onLiftClicked();

private:
    GamePlay *m_gamePlay;
    Player *m_player;

    QPushButton *m_restBtn;
    QPushButton *m_smithBtn;
    QPushButton *m_liftBtn;

    void setupUI();
    void checkAvailableOptions();
};

class UpgradeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UpgradeDialog(GamePlay *gamePlay, QWidget *parent = nullptr);

private:
    bool m_cardWasUpgraded = false;

public:
    bool cardWasUpgraded() const;
};

#endif // CAMPFIRE_H
