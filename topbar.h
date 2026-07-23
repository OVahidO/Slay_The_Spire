#ifndef TOPBAR_H
#define TOPBAR_H

#include <QWidget>

class Player;
class GamePlay;
class Card;
class Potion;

namespace Ui {
class Topbar;
}

class Topbar : public QWidget
{
    Q_OBJECT

public:
    explicit Topbar(GamePlay *gameplay, QWidget *parent = nullptr);
    ~Topbar();

public slots:
    void updateHpLabel();
    void updateCoinLabel();
    void updateDeckSizeLabel();
    void potionClicked(Potion* potion);
    void newPotionHandler(Potion* potion);

signals:
    void potionUsed(Potion* potion);

private slots:
    void on_deckButton_clicked();

private:
    Ui::Topbar *ui;
    Player* m_player;
    bool m_isInCombat = false;
    QVector<Potion*> m_emptyBottles{4, nullptr};

    QWidget* m_overlay;
    GamePlay* m_gameplay;
};

#endif // TOPBAR_H
