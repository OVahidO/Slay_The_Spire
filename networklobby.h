#ifndef NETWORKLOBBY_H
#define NETWORKLOBBY_H

#include <QObject>
#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;

class NetworkLobby : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkLobby(QWidget *parent = nullptr);

    void setInputEnabled(bool enabled);

public slots:
    void setStatusMessage(const QString &message);

signals:
    void hostRequested(quint16 port);
    void joinRequested(const QString &address, quint16 port);

private slots:
    void onHostButtonClicked();
    void onJoinButtonClicked();

private:
    QLineEdit *m_hostPortInput;
    QLineEdit *m_joinAddressInput;
    QLineEdit *m_joinPortInput;
    QPushButton *m_hostButton;
    QPushButton *m_joinButton;
    QLabel *m_statusLabel;
};

#endif // NETWORKLOBBY_H
