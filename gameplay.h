#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QWidget>

namespace Ui {
class GamePlay;
}

class GamePlay : public QWidget
{
    Q_OBJECT

public:
    explicit GamePlay(QWidget *parent = nullptr);
    ~GamePlay();

private:
    Ui::GamePlay *ui;
};

#endif // GAMEPLAY_H
