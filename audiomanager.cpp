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
        {MusicTrack::MainMenu,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - Slay the Spire.mp3")},
        {MusicTrack::MapAct1,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - Exordium.mp3")},
        {MusicTrack::MapAct2,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - The City.mp3")},
        {MusicTrack::CombatNormalAct1,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - Battle Trance.mp3")},
        {MusicTrack::CombatNormalAct2,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - The City.mp3")},
        {MusicTrack::CombatElite,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - Facing the Elite.mp3")},
        {MusicTrack::CombatBossKingSlime,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - Exordium Slain.mp3")},
        {MusicTrack::CombatBossHexaGhost,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - The Guardian Emerges.mp3")},
        {MusicTrack::CombatBossTheChamp,
         QUrl(
             "qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - Battle With the Champ.mp3")},
        {MusicTrack::Shop,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - Meet the Merchant.mp3")},
        {MusicTrack::Treasure,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - The Beyond.mp3")},
        {MusicTrack::Campfire,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - After Image.mp3")},
        {MusicTrack::Event,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - The Beyond.mp3")},
        {MusicTrack::Victory,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - The Spire Slain.mp3")},
        {MusicTrack::Defeat,
         QUrl("qrc:/soundtracks/Sounds-Musics/soundtracks/Clark Aboud - The Ending.mp3")},
    };

    m_sfxPaths = {
        {SfxId::UiHover,
         "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/mixkit-sword-strikes-armor-2765.wav"},
        {SfxId::UiClick,
         "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/mixkit-fast-sword-whoosh-2792.wav"},
        {SfxId::EmptySpaceClick, "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/bong_001.ogg"},
        {SfxId::InvalidClick, "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/bong_001.ogg"},
        {SfxId::CardSelect, "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/draw.wav"},
        {SfxId::CardPlay, "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/card-slide-2.ogg"},
        {SfxId::CardShuffle, "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/card-fan-1.ogg"},
        {SfxId::CardDraw, "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/card-place-3.ogg"},
        {SfxId::CheckboxHover,
         "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/mixkit-sword-strikes-armor-2765.wav"},
        {SfxId::EndTurnClick, "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/card-shove-1.ogg"},
        {SfxId::ShopBuy, "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/clinking-coins-1993.wav"},
        {SfxId::CampfireUpgrade,
         "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/sword-slice-2-393845.mp3"},
        {SfxId::CampfireRest,
         "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/fire-swoosh-burning-1328.wav"},
        {SfxId::TreasureOpen, "qrc:/soundeffects/Sounds-Musics/soundeffects/UI/chest.mp3"},
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