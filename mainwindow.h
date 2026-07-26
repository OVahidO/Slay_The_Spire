#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCursor>
#include <QEvent>
#include <QMainWindow>
#include <QPixmap>

#include "gamemanager.h"

class Topbar;
class GameManager;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;
    Topbar* m_topbar;
    GameManager *m_gameManager;

    QCursor m_normalCursor;
    QCursor m_clickedCursor;
};
#endif // MAINWINDOW_H
