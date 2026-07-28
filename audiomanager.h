#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QAudioOutput>
#include <QHash>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QUrl>
#include <QVector>

enum class MusicTrack {
    MainMenu,
    MapAct1,
    MapAct2,
    CombatNormalAct1,
    CombatNormalAct2,
    CombatElite,
    CombatBossKingSlime,
    CombatBossHexaGhost,
    CombatBossTheChamp,
    Shop,
    Treasure,
    Campfire,
    Event,
    Victory,
    Defeat
};

enum class SfxId {
    UiHover,
    UiClick,
    EmptySpaceClick,
    InvalidClick,
    CardSelect,
    CardPlay,
    CardShuffle,
    CardDraw,
    CheckboxHover,
    EndTurnClick,
    ShopBuy,
    CampfireUpgrade,
    CampfireRest,
    TreasureOpen
};

class AudioManager
{
public:
    static void playMusic(MusicTrack track);
    static void stopMusic();
    static void playSfx(SfxId id);

    static void setMasterVolume(int volume); // 0-100
    static void setMuted(bool muted);

private:
    static AudioManager &instance();
    AudioManager();
    ~AudioManager();

    QSoundEffect *acquireSfxPlayer(SfxId id);

    // BGM
    QMediaPlayer m_musicPlayer;
    QAudioOutput m_musicOutput;
    QHash<MusicTrack, QUrl> m_musicUrls;
    MusicTrack m_currentTrack = MusicTrack::MainMenu;
    bool m_hasCurrentTrack = false;

    // SFX
    QHash<SfxId, QString> m_sfxPaths;
    QHash<SfxId, QVector<QSoundEffect *>> m_sfxPool;
    static const int kPoolSizePerSfx = 4;

    qreal m_volume = 0.8;
    bool m_muted = false;
};

#endif // AUDIOMANAGER_H