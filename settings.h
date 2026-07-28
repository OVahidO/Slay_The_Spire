#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QWidget>
#include <QDialog>

class Player;
class QSlider;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui {
class settings;
}
QT_END_NAMESPACE

enum class SettingsMode { MainMenu, InGame };

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(Player *player, SettingsMode mode, QWidget *parent = nullptr);
    ~SettingsDialog();

    SettingsMode mode() const;
    void setMode(SettingsMode mode);

    void setVolume(int volume);
    void setMuted(bool muted);
    void setPlayer(Player *player);

signals:
    // Audio
    void volumeChanged(int volume);
    void muteToggled(bool muted);

    // Account
    void credentialsSaveRequested(const QString &username, const QString &password);
    void logoutRequested();

    // Game flow (InGame mode only)
    void returnRequested();
    void saveAndQuitRequested();
    void abandonRunRequested();

    // MainMenu mode only
    void closeRequested();

private slots:
    void onReturnClicked();
    void onSaveAndQuitClicked();
    void onAbandonRunClicked();
    void onCloseClicked();
    void on_horizontalSlider_valueChanged(int value);
    void on_muteCheckBox_toggled(bool checked);
    void on_saveButton_clicked();
    // void onLogoutClicked();
    void on_pushButton_clicked();

private:
    Ui::settings *ui;
    void paintEvent(QPaintEvent *event) override;

    Player *m_player;
    SettingsMode m_mode;
    // QPushButton *m_logoutBtn;

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
