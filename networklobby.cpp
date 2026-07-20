#include "networklobby.h"

#include <QGroupBox>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

NetworkLobby::NetworkLobby(QWidget *parent)
    : QWidget(parent)
{
    // setFixedSize(1280, 720);
    // setStyleSheet("background-color: #1b1b1f; color: #e5d4b3;");

    // QVBoxLayout *mainLayout = new QVBoxLayout(this);
    // mainLayout->setContentsMargins(300, 120, 300, 120);
    // mainLayout->setSpacing(20);

    // QLabel *title = new QLabel("Multiplayer (Co-op) - 2 Players Only", this);
    // title->setStyleSheet("font-size: 22px; font-weight: bold;");
    // title->setAlignment(Qt::AlignCenter);
    // mainLayout->addWidget(title);

    // QGroupBox *hostBox = new QGroupBox("Host a Game", this);
    // QVBoxLayout *hostLayout = new QVBoxLayout(hostBox);
    // m_hostPortInput = new QLineEdit("7777", hostBox);
    // m_hostPortInput->setValidator(new QIntValidator(1024, 65535, m_hostPortInput));
    // hostLayout->addWidget(new QLabel("Port:", hostBox));
    // hostLayout->addWidget(m_hostPortInput);
    // m_hostButton = new QPushButton("Start Hosting", hostBox);
    // connect(m_hostButton, &QPushButton::clicked, this, &NetworkLobby::onHostButtonClicked);
    // hostLayout->addWidget(m_hostButton);
    // mainLayout->addWidget(hostBox);

    // QGroupBox *joinBox = new QGroupBox("Join a Game", this);
    // QVBoxLayout *joinLayout = new QVBoxLayout(joinBox);
    // m_joinAddressInput = new QLineEdit("127.0.0.1", joinBox);
    // joinLayout->addWidget(new QLabel("Host IP Address:", joinBox));
    // joinLayout->addWidget(m_joinAddressInput);
    // m_joinPortInput = new QLineEdit("7777", joinBox);
    // m_joinPortInput->setValidator(new QIntValidator(1024, 65535, m_joinPortInput));
    // joinLayout->addWidget(new QLabel("Port:", joinBox));
    // joinLayout->addWidget(m_joinPortInput);
    // m_joinButton = new QPushButton("Connect", joinBox);
    // connect(m_joinButton, &QPushButton::clicked, this, &NetworkLobby::onJoinButtonClicked);
    // joinLayout->addWidget(m_joinButton);
    // mainLayout->addWidget(joinBox);

    // m_statusLabel = new QLabel("", this);
    // m_statusLabel->setAlignment(Qt::AlignCenter);
    // m_statusLabel->setStyleSheet("color: #f2c94c;");
    // mainLayout->addWidget(m_statusLabel);
}

void NetworkLobby::onHostButtonClicked()
{
    bool ok = false;
    quint16 port = static_cast<quint16>(m_hostPortInput->text().toUInt(&ok));

    if (!ok) {
        setStatusMessage("Invalid port number.");
        return;
    }

    m_hostButton->setEnabled(false);
    m_joinButton->setEnabled(false);
    emit hostRequested(port);
}

void NetworkLobby::onJoinButtonClicked()
{
    bool ok = false;
    quint16 port = static_cast<quint16>(m_joinPortInput->text().toUInt(&ok));

    if (!ok || m_joinAddressInput->text().isEmpty()) {
        setStatusMessage("Invalid address or port.");
        return;
    }

    m_hostButton->setEnabled(false);
    m_joinButton->setEnabled(false);
    emit joinRequested(m_joinAddressInput->text(), port);
}

void NetworkLobby::setStatusMessage(const QString &message)
{
    m_statusLabel->setText(message);
}

void NetworkLobby::setInputEnabled(bool enabled)
{
    m_hostButton->setEnabled(enabled);
    m_joinButton->setEnabled(enabled);
}