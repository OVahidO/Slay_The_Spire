#include "allrelics.h"

#include <QRandomGenerator>
#include <QUrl>
#include "buffdebuff.h"
#include "card.h"
#include "cursecards.h"
#include "gameplay.h"
#include "player.h"
#include "statuscards.h"

FaceOfTheCleric::FaceOfTheCleric(QWidget *parent)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////

CultistHeadpiece::CultistHeadpiece(QWidget *parent)
    : Relic("Cultist Headpiece", "CAW CAWW!", relicType::Event, parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/event/CultistMask.png";
    loadIcon();
    m_cawSound.setSource(QUrl(
        "qrc:/soundeffects/Sounds-Musics/soundeffects/enemies/cultist-cacaw-slaythespire.mp3"));
    m_cawSound.setVolume(0.8f);
}

void CultistHeadpiece::onCombatStart(GamePlay *game)
{
    Q_UNUSED(game);

    if (m_cawSound.isLoaded())
        m_cawSound.play();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

MutagenicStrength::MutagenicStrength(QWidget *parent)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////

WarpedTongs::WarpedTongs(QWidget *parent)
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////

CallingBell::CallingBell(QWidget *parent)
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

    game->addCardToDeck(new CurseOfTheBell());

    for (int i = 0; i < 3; ++i) {
        Relic *r = createRandomNormalRelic();
        game->grantRelicToPlayer(r);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

MarkOfPain::MarkOfPain(QWidget *parent)
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
        game->addCardToDrawPile(new WOUND(), true);
        game->addCardToDrawPile(new WOUND(), true);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

VelvetChoker::VelvetChoker(QWidget *parent)
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
    if (player)
        player->setCannotPlayCards(false);
}

void VelvetChoker::onCardPlayed(Card *card, Player *player)
{
    Q_UNUSED(card);
    if (!player)
        return;

    m_counter++;

    if (m_counter >= 6)
        player->setCannotPlayCards(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

BlackStar::BlackStar(QWidget *parent)
    : Relic("Black Star", "Elites now drop 2 relics when defeated.", relicType::Boss, parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/boss/black_star.png";
    loadIcon();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Girya::Girya(QWidget *parent)
    : Relic("Girya", "You may now lift at campsites.", relicType::Normal, parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/normal/girya.png";
    loadIcon();
    m_counter = 3;
}

void Girya::onCombatStart(GamePlay *game)
{
    if (game && game->player()) {
        int liftedTimes = 3 - m_counter;

        if (liftedTimes > 0)
            game->player()->applyBuffDebuff(BuffDebuffType::Strength, liftedTimes);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Kunai::Kunai(QWidget *parent)
    : Relic("Kunai",
            "Every time you play 3 attacks in a turn gain 1 Dexterity.",
            relicType::Normal,
            parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/normal/kunai.png";
    loadIcon();
    m_counter = 0;
}

void Kunai::onTurnStart(Player *player)
{
    Q_UNUSED(player);
    m_counter = 0;
}

void Kunai::onCardPlayed(Card *card, Player *player)
{
    if (!card || !player)
        return;

    if (card->cardType() == CardType::Attack) {
        m_counter++;

        if (m_counter == 3) {
            player->applyBuffDebuff(BuffDebuffType::Dexterity, 1);
            m_counter = 0;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Shuriken::Shuriken(QWidget *parent)
    : Relic("Shuriken",
            "Every time you play 3 attacks in a turn gain 1 Strength.",
            relicType::Normal,
            parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/normal/shuriken.png";
    loadIcon();
    m_counter = 0;
}

void Shuriken::onTurnStart(Player *player)
{
    Q_UNUSED(player);
    m_counter = 0;
}

void Shuriken::onCardPlayed(Card *card, Player *player)
{
    if (!card || !player)
        return;

    if (card->cardType() == CardType::Attack) {
        m_counter++;

        if (m_counter == 3) {
            player->applyBuffDebuff(BuffDebuffType::Strength, 1);
            m_counter = 0;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

IceCream::IceCream(QWidget *parent)
    : Relic("Ice Cream", "Energy is now conserved between turns.", relicType::Normal, parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/normal/ice_cream.png";
    loadIcon();
    m_savedEnergy = 0;
}

void IceCream::onTurnEnd(Player *player)
{
    if (player)
        m_savedEnergy = player->energy();
}

void IceCream::onTurnStart(Player *player)
{
    if (player && m_savedEnergy > 0) {
        player->addEnergy(m_savedEnergy);
        m_savedEnergy = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Anchor::Anchor(QWidget *parent)
    : Relic("Anchor", "Start each combat with 10 block.", relicType::Normal, parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/normal/anchor.png";
    loadIcon();
}

void Anchor::onCombatStart(GamePlay *game)
{
    if (game && game->player())
        game->player()->addBlock(10);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

GoldenIdol::GoldenIdol(QWidget *parent)
    : Relic("Golden Idol", "A stolen relic. Its origin remains a mystery.", relicType::Event, parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/event/GoldenIdol.png";
    loadIcon();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

GremlinVisage::GremlinVisage(QWidget *parent)
    : Relic("Gremlin Visage", "At the start of combat gain 1 Weak.", relicType::Event, parent)
{
    m_soucePath = ":/icons/Pics/Icons/relic/event/GremlinMask.png";
    loadIcon();
}

void GremlinVisage::onCombatStart(GamePlay *game)
{
    if (game && game->player())
        game->player()->applyBuffDebuff(BuffDebuffType::Weak, 1);
}
