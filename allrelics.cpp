#include "allrelics.h"

#include <QRandomGenerator>
#include <QUrl>
#include "buffdebuff.h"
#include "card.h"
#include "cursecards.h"
#include "gameplay.h"
#include "player.h"
#include "statuscards.h"

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
        QUrl(":/sounds/Sounds-Musics/soundeffects/enemies/cultist-cacaw-slaythespire.mp3"));
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

CallingBell::CallingBell(QGraphicsItem *parent)
    : Relic("Calling Bell",
            "Obtain the Curse of the Bell and 3 random normal relics.",
            relicType::Boss,
            parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/boss/CallingBell.png";
    loadIcon();
}

void CallingBell::onEquip(GamePlay *game)
{
    if (!game || !game->player())
        return;

    // game->addCardToDeck(new CurseOfTheBell());

    for (int i = 0; i < 3; ++i) {
        Relic *r = createRandomNormalRelic();
        r->onEquip(game);
        game->player()->addRelic(r);
    }
}

MarkOfPain::MarkOfPain(QGraphicsItem *parent)
    : Relic(
          "Mark of Pain",
          "Gain 1 extra energy per turn; at the start of combat have 2 WOUNDS added to draw pile.",
          relicType::Boss,
          parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/boss/MarkofPain.png";
    loadIcon();
}

void MarkOfPain::onEquip(GamePlay *game)
{
    if (game && game->player()) {
        Player *player = game->player();
        player->setMaxEnergy(player->maxEnergy() + 1);
    }
}

void MarkOfPain::onCombatStart(GamePlay *game)
{
    if (game) {
        // game->addCardToHand(new WOUND());
        // game->addCardToHand(new WOUND());
    }
}

VelvetChoker::VelvetChoker(QGraphicsItem *parent)
    : Relic("Velvet Choker",
            "Gain 1 extra energy per turn; you can no longer play more than 6 cards per turn.",
            relicType::Boss,
            parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/boss/velvet_choker.png";
    loadIcon();
    m_counter = 0;
}

void VelvetChoker::onEquip(GamePlay *game)
{
    if (game && game->player())
        game->player()->setMaxEnergy(game->player()->maxEnergy() + 1);
}

void VelvetChoker::onTurnStart(Player *player)
{
    m_counter = 0;
    // if (player)
    //     player->setCannotPlayCards(false);
}

void VelvetChoker::onCardPlayed(Card *card, Player *player)
{
    Q_UNUSED(card);
    if (!player)
        return;

    m_counter++;

    // if (m_counter >= 6)
    //     player->setCannotPlayCards(true);
}