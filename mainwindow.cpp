#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1280, 720);

    m_gameManager = new GameManager(ui->stackedWidget, this);
    m_gameManager->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
