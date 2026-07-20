#ifndef PILEDIALOG_H
#define PILEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QVector>

class Card;

namespace Ui {
class PileDialog;
}

class PileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PileDialog(const std::vector<Card*>& pile, QWidget *parent = nullptr);
    ~PileDialog();

private slots:
    void on_BackButton_clicked();

private:
    Ui::PileDialog *ui;
};

#endif // PILEDIALOG_H
