#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>

class Player;

namespace Ui {
class Topbar;
}

class Topbar : public QWidget
{
    Q_OBJECT

public:
    explicit Topbar(Player* player, QWidget *parent = nullptr);
    ~Topbar();

public slots:
    void updateHpLabel();
    void updateCoinLabel();

private:
    Ui::Topbar *ui;
    Player* m_player;
};

#endif // TOPBAR_H
