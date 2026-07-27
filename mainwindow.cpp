#include "mainwindow.h"
#include "audiomanager.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1280, 720);

    QPixmap normalPix(":/icons/Pics/Icons/cursor_default.png");
    QPixmap clickedPix(":/icons/Pics/Icons/cursor_tilted.png");

    m_normalCursor = QCursor(normalPix.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation),
                             0,
                             0);
    m_clickedCursor
        = QCursor(clickedPix.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation), 0, 0);

    qApp->setOverrideCursor(m_normalCursor);

    qApp->installEventFilter(this);

    m_gameManager = new GameManager(ui->stackedWidget, ui->verticalLayout, this);
    m_gameManager->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        qApp->changeOverrideCursor(m_clickedCursor);
        if (qobject_cast<QPushButton *>(obj))
            AudioManager::playSfx(SfxId::UiClick);
    } else if (event->type() == QEvent::MouseButtonRelease) {
        qApp->changeOverrideCursor(m_normalCursor);
    } else if (event->type() == QEvent::Enter) {
        if (qobject_cast<QPushButton *>(obj))
            AudioManager::playSfx(SfxId::UiHover);
    }

    return QMainWindow::eventFilter(obj, event);
}