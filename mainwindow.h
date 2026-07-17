#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    Topbar* m_topbar;
    GameManager *m_gameManager;
};
#endif // MAINWINDOW_H
