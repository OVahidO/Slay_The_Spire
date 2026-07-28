#ifndef ENDCOMBATPAGES_H
#define ENDCOMBATPAGES_H

#include <QWidget>
#include <QDialog>

class Player;

enum class endMod {Victory , Defate};

namespace Ui {
class endCombatPages;
}

class endCombatPages : public QDialog
{
    Q_OBJECT

public:
    explicit endCombatPages(Player* player, endMod endMod, QWidget *parent = nullptr);
    ~endCombatPages();

private slots:
    void on_endPageButton_clicked();

private:
    Ui::endCombatPages *ui;
    void paintEvent(QPaintEvent *event) override;

    Player* m_player;
};

#endif // ENDCOMBATPAGES_H
