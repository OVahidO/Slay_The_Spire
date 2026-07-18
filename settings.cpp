#include "settings.h"
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
    : QWidget(parent)
    , m_player(player)
    , m_mode(mode)
{
    setFixedSize(1280, 640);
    setupUi();
    applyModeVisibility();
}

SettingsDialog::~SettingsDialog() {}

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
    m_volumeSlider->blockSignals(true);
    m_volumeSlider->setValue(qBound(0, volume, 100));
    m_volumeSlider->blockSignals(false);
}

void SettingsDialog::setMuted(bool muted)
{
    m_muteCheckBox->blockSignals(true);
    m_muteCheckBox->setChecked(muted);
    m_volumeSlider->setEnabled(!muted);
    m_muteCheckBox->blockSignals(false);
}

// void SettingsDialog::setupUi()
// {
//     QVBoxLayout *mainLayout = new QVBoxLayout(this);
//     mainLayout->setContentsMargins(40, 30, 40, 30);
//     mainLayout->setSpacing(20);

//     QLabel *title = new QLabel("Settings", this);
//     QFont titleFont("Arial", 22, QFont::Bold);
//     title->setFont(titleFont);
//     mainLayout->addWidget(title);

//     // ---------------- Audio settings ----------------
//     QGroupBox *audioBox = new QGroupBox("Audio", this);
//     QVBoxLayout *audioLayout = new QVBoxLayout(audioBox);

//     QHBoxLayout *volumeRow = new QHBoxLayout();
//     QLabel *volumeLabel = new QLabel("Volume", audioBox);
//     m_volumeSlider = new QSlider(Qt::Horizontal, audioBox);
//     m_volumeSlider->setRange(0, 100);
//     m_volumeSlider->setValue(80);
//     volumeRow->addWidget(volumeLabel);
//     volumeRow->addWidget(m_volumeSlider);
//     audioLayout->addLayout(volumeRow);

//     m_muteCheckBox = new QCheckBox("Mute", audioBox);
//     audioLayout->addWidget(m_muteCheckBox);

//     connect(m_volumeSlider, &QSlider::valueChanged, this, &SettingsDialog::onVolumeSliderChanged);
//     connect(m_muteCheckBox, &QCheckBox::toggled, this, &SettingsDialog::onMuteToggled);

//     mainLayout->addWidget(audioBox);

//     // ---------------- Account settings ----------------
//     QGroupBox *accountBox = new QGroupBox("Account", this);
//     QVBoxLayout *accountLayout = new QVBoxLayout(accountBox);

//     m_usernameInput = new QLineEdit(accountBox);
//     m_usernameInput->setPlaceholderText("New username");
//     if (m_player)
//         m_usernameInput->setText(m_player->name());

//     m_passwordInput = new QLineEdit(accountBox);
//     m_passwordInput->setPlaceholderText("New password");
//     m_passwordInput->setEchoMode(QLineEdit::Password);

//     m_saveAccountBtn = new QPushButton("Save Account Changes", accountBox);
//     connect(m_saveAccountBtn, &QPushButton::clicked, this, &SettingsDialog::onSaveAccountClicked);

//     m_accountStatusLabel = new QLabel(accountBox);
//     m_accountStatusLabel->setStyleSheet("color: #2ecc71;");

//     accountLayout->addWidget(new QLabel("Username", accountBox));
//     accountLayout->addWidget(m_usernameInput);
//     accountLayout->addWidget(new QLabel("Password", accountBox));
//     accountLayout->addWidget(m_passwordInput);
//     accountLayout->addWidget(m_saveAccountBtn);
//     accountLayout->addWidget(m_accountStatusLabel);

//     mainLayout->addWidget(accountBox);

//     mainLayout->addStretch();

//     // ---------------- Bottom buttons ----------------
//     QHBoxLayout *bottomLayout = new QHBoxLayout();

//     m_closeBtn = new QPushButton("Close", this);
//     connect(m_closeBtn, &QPushButton::clicked, this, &SettingsDialog::onCloseClicked);

//     m_returnBtn = new QPushButton("Return", this);
//     connect(m_returnBtn, &QPushButton::clicked, this, &SettingsDialog::onReturnClicked);

//     m_saveAndQuitBtn = new QPushButton("Save & Quit", this);
//     connect(m_saveAndQuitBtn, &QPushButton::clicked, this, &SettingsDialog::onSaveAndQuitClicked);

//     m_abandonRunBtn = new QPushButton("Abandon Run", this);
//     connect(m_abandonRunBtn, &QPushButton::clicked, this, &SettingsDialog::onAbandonRunClicked);

//     bottomLayout->addWidget(m_closeBtn);
//     bottomLayout->addStretch();
//     bottomLayout->addWidget(m_returnBtn);
//     bottomLayout->addWidget(m_saveAndQuitBtn);
//     bottomLayout->addWidget(m_abandonRunBtn);

//     mainLayout->addLayout(bottomLayout);
// }

void SettingsDialog::applyModeVisibility()
{
    const bool inGame = (m_mode == SettingsMode::InGame);

    m_returnBtn->setVisible(inGame);
    m_saveAndQuitBtn->setVisible(inGame);
    m_abandonRunBtn->setVisible(inGame);

    m_closeBtn->setVisible(!inGame);
}

void SettingsDialog::onVolumeSliderChanged(int value)
{
    emit volumeChanged(value);
}

void SettingsDialog::onMuteToggled(bool checked)
{
    m_volumeSlider->setEnabled(!checked);
    emit muteToggled(checked);
}

void SettingsDialog::onSaveAccountClicked()
{
    const QString username = m_usernameInput->text().trimmed();
    const QString password = m_passwordInput->text();

    if (username.isEmpty()) {
        m_accountStatusLabel->setStyleSheet("color: #e74c3c;");
        m_accountStatusLabel->setText("Username cannot be empty.");
        return;
    }

    emit credentialsSaveRequested(username, password);

    m_accountStatusLabel->setStyleSheet("color: #2ecc71;");
    m_accountStatusLabel->setText("Account changes saved.");
    m_passwordInput->clear();
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
