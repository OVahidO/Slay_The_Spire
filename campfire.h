#ifndef CAMPFIRE_H
#define CAMPFIRE_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class Player;
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
    explicit UpgradeDialog(Player *player, QWidget *parent = nullptr);
};

#endif // CAMPFIRE_H
