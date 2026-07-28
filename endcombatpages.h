#ifndef ENDCOMBATPAGES_H
#define ENDCOMBATPAGES_H

#include <QWidget>
#include <QDialog>

enum class endMod {Victory , Defate};

namespace Ui {
class endCombatPages;
}

class endCombatPages : public QDialog
{
    Q_OBJECT

public:
    explicit endCombatPages(endMod endMod, QWidget *parent = nullptr);
    ~endCombatPages();

private slots:
    void on_endPageButton_clicked();

private:
    Ui::endCombatPages *ui;
    void paintEvent(QPaintEvent *event) override;
};

#endif // ENDCOMBATPAGES_H
