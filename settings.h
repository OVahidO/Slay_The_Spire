#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QWidget>

class Player;
class QSlider;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QLabel;

enum class SettingsMode { MainMenu, InGame };

class SettingsDialog : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsDialog(Player *player, SettingsMode mode, QWidget *parent = nullptr);
    ~SettingsDialog();

    SettingsMode mode() const;
    void setMode(SettingsMode mode);

    void setVolume(int volume);
    void setMuted(bool muted);

signals:
    // Audio
    void volumeChanged(int volume);
    void muteToggled(bool muted);

    // Account
    void credentialsSaveRequested(const QString &username, const QString &password);

    // Game flow (InGame mode only)
    void returnRequested();
    void saveAndQuitRequested();
    void abandonRunRequested();

    // MainMenu mode only
    void closeRequested();

private slots:
    void onVolumeSliderChanged(int value);
    void onMuteToggled(bool checked);
    void onSaveAccountClicked();
    void onReturnClicked();
    void onSaveAndQuitClicked();
    void onAbandonRunClicked();
    void onCloseClicked();

private:
    Player *m_player;
    SettingsMode m_mode;

    // Audio
    QSlider *m_volumeSlider;
    QCheckBox *m_muteCheckBox;

    // Account
    QLineEdit *m_usernameInput;
    QLineEdit *m_passwordInput;
    QPushButton *m_saveAccountBtn;
    QLabel *m_accountStatusLabel;

    // Game flow (InGame only)
    QPushButton *m_returnBtn;
    QPushButton *m_saveAndQuitBtn;
    QPushButton *m_abandonRunBtn;

    // MainMenu only
    QPushButton *m_closeBtn;

    void setupUi();
    void applyModeVisibility();
};

#endif // SETTINGS_H
