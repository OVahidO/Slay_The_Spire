#include "soundmanager.h"

SoundManager &SoundManager::instance()
{
    static SoundManager manager;
    return manager;
}

SoundManager::SoundManager()
{
    m_player.setAudioOutput(&m_audioOutput);
    m_audioOutput.setVolume(0.8);

    m_trackUrls = {
        // {MusicTrack::MainMenu, QUrl("qrc:/soundeffects/Sounds-Musics/music/main_menu_theme.mp3")},
        // {MusicTrack::Map, QUrl("qrc:/soundeffects/Sounds-Musics/music/map_theme.mp3")},
        // {MusicTrack::Combat, QUrl("qrc:/soundeffects/Sounds-Musics/music/combat_theme.mp3")},
        // {MusicTrack::Boss, QUrl("qrc:/soundeffects/Sounds-Musics/music/boss_theme.mp3")},
        // {MusicTrack::Campfire, QUrl("qrc:/soundeffects/Sounds-Musics/music/campfire_theme.mp3")},
        // {MusicTrack::Shop, QUrl("qrc:/soundeffects/Sounds-Musics/music/shop_theme.mp3")},
    };

    m_player.setLoops(QMediaPlayer::Infinite);
}

void SoundManager::play(MusicTrack track)
{
    SoundManager &mgr = instance();

    if (mgr.m_hasCurrent && mgr.m_currentTrack == track
        && mgr.m_player.playbackState() == QMediaPlayer::PlayingState)
        return;

    mgr.m_player.stop();
    mgr.m_player.setSource(mgr.m_trackUrls.value(track));
    mgr.m_player.setLoops(QMediaPlayer::Infinite);
    mgr.m_player.play();

    mgr.m_currentTrack = track;
    mgr.m_hasCurrent = true;
}

void SoundManager::stop()
{
    instance().m_player.stop();
    instance().m_hasCurrent = false;
}

void SoundManager::setVolume(int volume)
{
    instance().m_audioOutput.setVolume(qBound(0, volume, 100) / 100.0);
}