#include "settings.h"
#include "ui_settings.h"
#include "player.h"

#include <QCheckBox>
#include <QFont>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(Player *player, SettingsMode mode, QWidget *parent)
    : QDialog(parent)
    , m_player(player)
    , m_mode(mode)
    , ui(new Ui::settings)
{
    ui->setupUi(this);
    setupUi();
    this->setGeometry(525,125,500,600);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    setAttribute(Qt::WA_TranslucentBackground);

    applyModeVisibility();

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPixmap background(":/Settings/Pics/Settings/settingsBg.png");
    painter.drawPixmap(this->rect(), background);
}

SettingsMode SettingsDialog::mode() const
{
    return m_mode;
}

void SettingsDialog::setMode(SettingsMode mode)
{
    m_mode = mode;
    applyModeVisibility();
}

void SettingsDialog::setVolume(int volume)
{
    ui->horizontalSlider->blockSignals(true);
    ui->horizontalSlider->setValue(qBound(0, volume, 100));
    ui->horizontalSlider->blockSignals(false);
}

void SettingsDialog::setMuted(bool muted)
{
    ui->muteCheckBox->blockSignals(true);
    ui->muteCheckBox->setChecked(muted);
    ui->horizontalSlider->setEnabled(!muted);
    ui->muteCheckBox->blockSignals(false);
}

void SettingsDialog::setPlayer(Player *player)
{
    m_player = player;

    if (m_player)
        ui->usernameInput->setText(m_player->name());
    else
        ui->usernameInput->clear();
}

void SettingsDialog::setupUi()
{
        QVBoxLayout *mainLayout = ui->mainLayout;

        QLabel *title = ui->title;
        QFont titleFont("Kreon", 22, QFont::Bold);
        title->setFont(titleFont);

        // ---------------- Account settings ----------------
        if (m_player)
            ui->usernameInput->setText(m_player->name());

        ui->accountStatusLabel->setStyleSheet("color: #2ecc71;");

        // ---------------- Bottom buttons ----------------
        QHBoxLayout *bottomLayout = new QHBoxLayout();

        m_closeBtn = new QPushButton("Close", this);
        connect(m_closeBtn, &QPushButton::clicked, this, &SettingsDialog::onCloseClicked);

        m_logoutBtn = new QPushButton("Logout", this);
        connect(m_logoutBtn, &QPushButton::clicked, this, &SettingsDialog::onLogoutClicked);

        m_returnBtn = new QPushButton("Return", this);
        connect(m_returnBtn, &QPushButton::clicked, this, &SettingsDialog::onReturnClicked);

        m_saveAndQuitBtn = new QPushButton("Save & Quit", this);
        connect(m_saveAndQuitBtn, &QPushButton::clicked, this, &SettingsDialog::onSaveAndQuitClicked);

        m_abandonRunBtn = new QPushButton("Abandon Run", this);
        connect(m_abandonRunBtn, &QPushButton::clicked, this, &SettingsDialog::onAbandonRunClicked);

        bottomLayout->addWidget(m_closeBtn);
        bottomLayout->addWidget(m_logoutBtn);
        bottomLayout->addStretch();
        bottomLayout->addWidget(m_returnBtn);
        bottomLayout->addWidget(m_saveAndQuitBtn);
        bottomLayout->addWidget(m_abandonRunBtn);

        mainLayout->addLayout(bottomLayout);
}

void SettingsDialog::applyModeVisibility()
{
    const bool inGame = (m_mode == SettingsMode::InGame);

    m_returnBtn->setVisible(inGame);
    m_saveAndQuitBtn->setVisible(inGame);
    m_abandonRunBtn->setVisible(inGame);

    m_closeBtn->setVisible(!inGame);
    m_logoutBtn->setVisible(!inGame);
}

void SettingsDialog::onReturnClicked()
{
    emit returnRequested();
}

void SettingsDialog::onSaveAndQuitClicked()
{
    QMessageBox::StandardButton confirm
        = QMessageBox::question(this,
                                "Save & Quit",
                                "Save your progress and return to the Main Menu?");

    if (confirm == QMessageBox::Yes)
        emit saveAndQuitRequested();
}

void SettingsDialog::onAbandonRunClicked()
{
    QMessageBox::StandardButton confirm
        = QMessageBox::warning(this,
                               "Abandon Run",
                               "This will permanently end your current run. Are you sure?",
                               QMessageBox::Yes | QMessageBox::No);

    if (confirm == QMessageBox::Yes)
        emit abandonRunRequested();
}

void SettingsDialog::onCloseClicked()
{
    emit closeRequested();
}

void SettingsDialog::on_horizontalSlider_valueChanged(int value)
{
    emit volumeChanged(value);
}

void SettingsDialog::on_muteCheckBox_toggled(bool checked)
{
    ui->horizontalSlider->setEnabled(!checked);
    emit muteToggled(checked);
}


void SettingsDialog::on_saveButton_clicked()
{
    const QString username = ui->usernameInput->text().trimmed();
    const QString password = ui->passwordInput->text();

    if (username.isEmpty()) {
        ui->accountStatusLabel->setStyleSheet("color: #e74c3c;");
        ui->accountStatusLabel->setText("Username cannot be empty.");
        return;
    }

    emit credentialsSaveRequested(username, password);

    ui->accountStatusLabel->setStyleSheet("color: #2ecc71;");
    ui->accountStatusLabel->setText("Account changes saved.");
    ui->passwordInput->clear();
}

void SettingsDialog::onLogoutClicked()
{
    QMessageBox::StandardButton confirm
        = QMessageBox::question(this, "Logout", "Are you sure you want to log out?");

    if (confirm == QMessageBox::Yes)
        emit logoutRequested();
}
