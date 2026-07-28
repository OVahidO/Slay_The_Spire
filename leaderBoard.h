#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class LeaderBoard;
}

class LeaderBoard : public QDialog
{
    Q_OBJECT

public:
    explicit LeaderBoard(QWidget *parent = nullptr);
    ~LeaderBoard();

    void refreshLeaderBoard();

private slots:
    void on_BackButton_clicked();

private:
    Ui::LeaderBoard *ui;
    void paintEvent(QPaintEvent *event) override;
};

#endif // LEADERBOARD_H
