#include "allrelics.h"

#include <QRandomGenerator>
#include <QUrl>
#include "buffdebuff.h"
#include "card.h"
#include "gameplay.h"
#include "player.h"

FaceOfTheCleric::FaceOfTheCleric(QGraphicsItem *parent)
    : Relic("Face of the Cleric",
            "Increase your max HP by 1 after every combat.",
            relicType::Event,
            parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/event/FaceOfCleric.png";
    loadIcon();
}

void FaceOfTheCleric::onCombatEnd(Player *player)
{
    if (player)
        player->addMaxHp(1);
}

CultistHeadpiece::CultistHeadpiece(QGraphicsItem *parent)
    : Relic("Cultist Headpiece", "CAW CAWW!", relicType::Event, parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/event/CultistMask.png";
    loadIcon();
    m_cawSound.setSource(
        QUrl(":/sounds/Sounds & Musics/soundeffects/enemies/cultist-cacaw-slaythespire.mp3"));
    m_cawSound.setVolume(0.8f);
}

void CultistHeadpiece::onCombatStart(GamePlay *game)
{
    Q_UNUSED(game);

    if (m_cawSound.isLoaded())
        m_cawSound.play();
}

MutagenicStrength::MutagenicStrength(QGraphicsItem *parent)
    : Relic("Mutagenic Strength",
            "At the start of combat gain 3 Strength and lose it after that turn.",
            relicType::Event,
            parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/event/MutagenicStrength.png";
    loadIcon();
}

void MutagenicStrength::onCombatStart(GamePlay *game)
{
    if (game && game->player()) {
        game->player()->applyBuffDebuff(BuffDebuffType::Strength, 3);
        m_isActive = true;
    }
}

void MutagenicStrength::onTurnEnd(Player *player)
{
    if (m_isActive && player) {
        player->applyBuffDebuff(BuffDebuffType::Strength, -3);
        m_isActive = false;
    }
}

WarpedTongs::WarpedTongs(QGraphicsItem *parent)
    : Relic("Warped Tongs",
            "At the start of combat upgrade a random card for the rest of the combat.",
            relicType::Event,
            parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/event/WarpedTongs.png";
    loadIcon();
}

void WarpedTongs::onCombatStart(GamePlay *game)
{
    if (!game || !game->player())
        return;

    QVector<Card *> &hand = game->player()->HandsCards();

    if (hand.isEmpty())
        return;

    QVector<Card *> unupgradedCards;
    for (Card *card : hand)
        if (card && !card->isUpgraded())
            unupgradedCards.append(card);

    if (!unupgradedCards.isEmpty()) {
        int randomIndex = QRandomGenerator::global()->bounded(unupgradedCards.size());
        Card *targetCard = unupgradedCards[randomIndex];

        targetCard->upgrade();

        targetCard->setLifetime(CardLifetime::EndOfCombat);
    }
}