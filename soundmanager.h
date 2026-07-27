#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QAudioOutput>
#include <QHash>
#include <QMediaPlayer>
#include <QUrl>

enum class MusicTrack { MainMenu, Map, Combat, Boss, Campfire, Shop };

class SoundManager
{
public:
    static void play(MusicTrack track);
    static void stop();
    static void setVolume(int volume); // 0-100

private:
    static SoundManager &instance();
    SoundManager();

    QMediaPlayer m_player;
    QAudioOutput m_audioOutput;
    QHash<MusicTrack, QUrl> m_trackUrls;
    MusicTrack m_currentTrack = MusicTrack::MainMenu;
    bool m_hasCurrent = false;
};

#endif // SOUNDMANAGER_H
