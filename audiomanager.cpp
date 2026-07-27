#include "audiomanager.h"

AudioManager &AudioManager::instance()
{
    static AudioManager manager;
    return manager;
}

AudioManager::AudioManager()
{
    m_musicPlayer.setAudioOutput(&m_musicOutput);
    m_musicOutput.setVolume(m_volume);
    m_musicPlayer.setLoops(QMediaPlayer::Infinite);

    m_musicUrls = {
        // {MusicTrack::MainMenu, QUrl("qrc:/soundeffects/Sounds-Musics/music/main_menu_theme.mp3")},
        // {MusicTrack::MapAct1, QUrl("qrc:/soundeffects/Sounds-Musics/music/map_act1_theme.mp3")},
        // {MusicTrack::MapAct2, QUrl("qrc:/soundeffects/Sounds-Musics/music/map_act2_theme.mp3")},
        // {MusicTrack::CombatNormalAct1,
        //  QUrl("qrc:/soundeffects/Sounds-Musics/music/combat_act1_theme.mp3")},
        // {MusicTrack::CombatNormalAct2,
        //  QUrl("qrc:/soundeffects/Sounds-Musics/music/combat_act2_theme.mp3")},
        // {MusicTrack::CombatElite,
        //  QUrl("qrc:/soundeffects/Sounds-Musics/music/combat_elite_theme.mp3")},
        // {MusicTrack::CombatBoss,
        //  QUrl("qrc:/soundeffects/Sounds-Musics/music/combat_boss_theme.mp3")},
        // {MusicTrack::Shop, QUrl("qrc:/soundeffects/Sounds-Musics/music/shop_theme.mp3")},
        // {MusicTrack::Treasure, QUrl("qrc:/soundeffects/Sounds-Musics/music/treasure_theme.mp3")},
        // {MusicTrack::Campfire, QUrl("qrc:/soundeffects/Sounds-Musics/music/campfire_theme.mp3")},
        // {MusicTrack::Event, QUrl("qrc:/soundeffects/Sounds-Musics/music/event_theme.mp3")},
        // {MusicTrack::Victory, QUrl("qrc:/soundeffects/Sounds-Musics/music/victory_theme.mp3")},
        // {MusicTrack::Defeat, QUrl("qrc:/soundeffects/Sounds-Musics/music/defeat_theme.mp3")},
    };

    m_sfxPaths = {
        // {SfxId::UiHover, "qrc:/soundeffects/Sounds-Musics/soundeffects/ui/hover.wav"},
        // {SfxId::UiClick, "qrc:/soundeffects/Sounds-Musics/soundeffects/ui/click.wav"},
        // {SfxId::EndTurnClick, "qrc:/soundeffects/Sounds-Musics/soundeffects/ui/end_turn.wav"},
        // {SfxId::ShopBuy, "qrc:/soundeffects/Sounds-Musics/soundeffects/shop/buy.wav"},
        // {SfxId::CampfireUpgrade,
        //  "qrc:/soundeffects/Sounds-Musics/soundeffects/campfire/upgrade.wav"},
        // {SfxId::CampfireRest, "qrc:/soundeffects/Sounds-Musics/soundeffects/campfire/rest.wav"},
        // {SfxId::TreasureOpen, "qrc:/soundeffects/Sounds-Musics/soundeffects/treasure/open.wav"},
    };
}

AudioManager::~AudioManager()
{
    for (auto &pool : m_sfxPool)
        for (QSoundEffect *effect : pool)
            delete effect;
}

void AudioManager::playMusic(MusicTrack track)
{
    AudioManager &mgr = instance();

    if (mgr.m_hasCurrentTrack && mgr.m_currentTrack == track
        && mgr.m_musicPlayer.playbackState() == QMediaPlayer::PlayingState)
        return;

    mgr.m_musicPlayer.stop();
    mgr.m_musicPlayer.setSource(mgr.m_musicUrls.value(track));
    mgr.m_musicPlayer.setLoops(QMediaPlayer::Infinite);
    mgr.m_musicPlayer.play();

    mgr.m_currentTrack = track;
    mgr.m_hasCurrentTrack = true;
}

void AudioManager::stopMusic()
{
    instance().m_musicPlayer.stop();
    instance().m_hasCurrentTrack = false;
}

QSoundEffect *AudioManager::acquireSfxPlayer(SfxId id)
{
    QVector<QSoundEffect *> &pool = m_sfxPool[id];

    for (QSoundEffect *effect : pool)
        if (!effect->isPlaying())
            return effect;

    if (pool.size() < kPoolSizePerSfx) {
        QSoundEffect *effect = new QSoundEffect();
        effect->setSource(QUrl(m_sfxPaths.value(id)));
        effect->setVolume(m_muted ? 0.0 : m_volume);
        pool.append(effect);
        return effect;
    }

    return pool.first();
}

void AudioManager::playSfx(SfxId id)
{
    AudioManager &mgr = instance();
    if (!mgr.m_sfxPaths.contains(id))
        return;

    QSoundEffect *effect = mgr.acquireSfxPlayer(id);
    effect->setVolume(mgr.m_muted ? 0.0 : mgr.m_volume);
    effect->play();
}

void AudioManager::setMasterVolume(int volume)
{
    AudioManager &mgr = instance();
    mgr.m_volume = qBound(0, volume, 100) / 100.0;

    mgr.m_musicOutput.setVolume(mgr.m_muted ? 0.0 : mgr.m_volume);
    for (auto &pool : mgr.m_sfxPool)
        for (QSoundEffect *effect : pool)
            effect->setVolume(mgr.m_muted ? 0.0 : mgr.m_volume);
}

void AudioManager::setMuted(bool muted)
{
    AudioManager &mgr = instance();
    mgr.m_muted = muted;

    mgr.m_musicOutput.setVolume(muted ? 0.0 : mgr.m_volume);
    for (auto &pool : mgr.m_sfxPool)
        for (QSoundEffect *effect : pool)
            effect->setVolume(muted ? 0.0 : mgr.m_volume);
}