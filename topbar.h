#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>

class Player;
class Potion;

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
    void potionClicked(Potion* potion);
    void newPotionHandler(Potion* potion);

signals:
    void potionUsed(Potion* potion);

private:
    Ui::Topbar *ui;
    Player* m_player;
    bool m_isInCombat = false;
    QVector<Potion*> m_emptyBottles{4, nullptr};
};

#endif // TOPBAR_H
