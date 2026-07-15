#include "allevents.h"

#include "allrelics.h"
#include "attackcards.h"
#include "card.h"
#include "carddialogs.h"
#include "cursecards.h"
#include "gameplay.h"
#include "player.h"
#include "potion.h"
#include "powercards.h"
#include "relic.h"
#include "skillcards.h"

#include <QRandomGenerator>
#include <QtMath>
#include <algorithm>

namespace {
Card *createRandomCurse()
{
    return (QRandomGenerator::global()->bounded(2) == 0)
               ? static_cast<Card *>(new Regret())
               : static_cast<Card *>(new CurseOfTheBell());
}

Potion *createRandomPotion()
{
    switch (QRandomGenerator::global()->bounded(4)) {
    case 0:
        return new BlockPotion();
    case 1:
        return new FirePotion();
    case 2:
        return new EnergyPotion();
    case 3:
        return new SwiftPotion();
    default:
        return new BlockPotion();
    }
}

Card *createRandomTransformCard()
{
    switch (QRandomGenerator::global()->bounded(6)) {
    case 0:
        return new Strike();
    case 1:
        return new Defend();
    case 2:
        return new Bash();
    case 3:
        return new Clash();
    case 4:
        return new Entrench();
    case 5:
        return new Inflame();
    default:
        return new Strike();
    }
}

} // namespace

OminousForge::OminousForge(Player *player, GamePlay *gamePlay, QWidget *parent)
    : Event("Ominous Forge",
            ":/events/Pics/Background/Events/Event-OminousForge.png",
            EventScope::Any,
            player,
            gamePlay,
            parent)
{
    setDescription("A strange, glowing forge hums with dark energy, waiting for an offering...");

    addOption({"Forge",
               "Upgrade a card\nfrom your deck",
               ":/icons/Pics/Icons/card-type-icon/black crossed sword-gold.png",
               [this]() { onForge(); },
               nullptr});

    addOption({"Rummage",
               "Obtain Warped Tongs\n+ a random Curse",
               ":/icons/Pics/Icons/relic/event/WarpedTongs.png",
               [this]() { onRummage(); },
               nullptr});

    addOption({"Leave", "Walk away without\ntouching the forge", "", []() {}, nullptr});
}

void OminousForge::onForge()
{
    if (!m_gamePlay)
        return;

    CardSelectionDialog dialog(m_gamePlay,
                               CardSelectionMode::Upgrade,
                               1,
                               "Forge - Choose a card to upgrade",
                               this);
    dialog.exec();
}

void OminousForge::onRummage()
{
    if (!m_player || !m_gamePlay)
        return;

    m_gamePlay->grantRelicToPlayer(new WarpedTongs());
    m_gamePlay->addCardToDeck(createRandomCurse());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Lab::Lab(Player *player, GamePlay *gamePlay, QWidget *parent)
    : Event("Lab",
            ":/events/Pics/Background/Events/Event-Lab.png",
            EventScope::Any,
            player,
            gamePlay,
            parent)
{
    setDescription("Rows of bubbling, unlabeled potions line the shelves of this abandoned lab...");

    addOption({"Search",
               "Obtain 3 random Potions",
               ":/Potions/Pics/Potions/block_potion.png",
               [this]() { onSearch(); },
               nullptr});
}

void Lab::onSearch()
{
    if (!m_player)
        return;

    for (int i = 0; i < 3; ++i) {
        Potion *potion = createRandomPotion();
        if (!m_player->addPotion(potion))
            delete potion;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

GoldenIdolEvent::GoldenIdolEvent(Player *player, GamePlay *gamePlay, QWidget *parent)
    : Event("Golden Idol",
            ":/events/Pics/Background/Events/Event-GoldenIdol.png",
            EventScope::Act1,
            player,
            gamePlay,
            parent)
{
    setDescription("A golden idol rests atop a pedestal, untouched for centuries...");

    addOption({"Steal",
               "Obtain the Golden Idol\nLose 5 Max HP",
               ":/icons/Pics/Icons/relic/event/GoldenIdol.png",
               [this]() { onSteal(); },
               nullptr});

    addOption({"Leave", "Walk away", "", []() {}, nullptr});
}

void GoldenIdolEvent::onSteal()
{
    if (!m_player || !m_gamePlay)
        return;

    m_gamePlay->grantRelicToPlayer(new GoldenIdol());
    m_player->addMaxHp(-5);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

TheCleric::TheCleric(Player *player, GamePlay *gamePlay, QWidget *parent)
    : Event("The Cleric",
            ":/events/Pics/Background/Events/Event-Cleric.png",
            EventScope::Act1,
            player,
            gamePlay,
            parent)
{
    setDescription("A hooded cleric offers blessings, for a price...");

    addOption({"Heal",
               "Lose 35 Gold\nHeal 25% Max HP",
               ":/icons/Pics/Icons/hpIcon.png",
               [this]() { onHeal(); },
               [this]() { return m_player && m_player->coin() >= 35; }});

    addOption({"Purify",
               "Lose 50 Gold\nRemove a card from your deck",
               ":/icons/Pics/Icons/card-type-icon/virus.png",
               [this]() { onPurify(); },
               [this]() { return m_player && m_player->coin() >= 50; }});

    addOption({"Leave", "Walk away", "", []() {}, nullptr});
}

void TheCleric::onHeal()
{
    if (!m_player || m_player->coin() < 35)
        return;

    m_player->setCoin(m_player->coin() - 35);
    m_player->heal(qRound(m_player->maxHP() * 0.25));
}

void TheCleric::onPurify()
{
    if (!m_player || !m_gamePlay || m_player->coin() < 50)
        return;

    m_player->setCoin(m_player->coin() - 50);

    CardSelectionDialog dialog(m_gamePlay,
                               CardSelectionMode::Remove,
                               1,
                               "Purify - Choose a card to remove",
                               this);
    dialog.exec();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Augmenter::Augmenter(Player *player, GamePlay *gamePlay, QWidget *parent)
    : Event("Augmenter",
            ":/events/Pics/Background/Events/Event-Augmenter.png",
            EventScope::Act2,
            player,
            gamePlay,
            parent)
{
    setDescription("A twisted scientist wants to experiment on you...");

    addOption({"Test J.A.X",
               "Obtain the J.A.X card",
               ":/card-art/Pics/Cards/Curse/jax.png",
               [this]() { onTestJAX(); },
               nullptr});

    addOption({"Become Test Subject",
               "Choose 2 cards\nto Transform",
               "",
               [this]() { onBecomeTestSubject(); },
               nullptr});

    addOption({"Ingest Mutagens",
               "Obtain the Mutagenic\nStrength relic",
               ":/icons/Pics/Icons/relic/event/MutagenicStrength.png",
               [this]() { onIngestMutagens(); },
               nullptr});
}

void Augmenter::onTestJAX()
{
    if (m_gamePlay)
        m_gamePlay->addCardToDeck(new J_A_X());
}

void Augmenter::onBecomeTestSubject()
{
    if (!m_gamePlay)
        return;

    CardSelectionDialog dialog(m_gamePlay,
                               CardSelectionMode::PickAny,
                               2,
                               "Become Test Subject - Choose 2 cards to Transform",
                               this);

    if (dialog.exec() == QDialog::Accepted)
        for (Card *card : dialog.selectedCards())
            transformCard(card);
}

void Augmenter::onIngestMutagens()
{
    if (m_gamePlay)
        m_gamePlay->grantRelicToPlayer(new MutagenicStrength());
}

void Augmenter::transformCard(Card *card)
{
    if (!m_gamePlay || !card)
        return;

    auto &deck = m_gamePlay->deck();
    auto it = std::find(deck.begin(), deck.end(), card);
    if (it == deck.end())
        return;

    deck.erase(it);
    delete card;

    m_gamePlay->addCardToDeck(createRandomTransformCard());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

FaceTrader::FaceTrader(Player *player, GamePlay *gamePlay, QWidget *parent)
    : Event("Face Trader",
            ":/events/Pics/Background/Events/Event-FaceTrader.png",
            EventScope::Act2,
            player,
            gamePlay,
            parent)
{
    setDescription("A masked figure offers to trade something of yours for something of theirs...");

    addOption({"Touch", "Lose 5-10% Max HP\nGain 75 Gold", "", [this]() { onTouch(); }, nullptr});

    addOption({"Trade",
               "Obtain a random Relic\n(Face of Cleric / Gremlin Visage / Cultist Headpiece)",
               "",
               [this]() { onTrade(); },
               nullptr});

    addOption({"Leave", "Walk away", "", []() {}, nullptr});
}

void FaceTrader::onTouch()
{
    if (!m_player)
        return;

    int percent = 5 + QRandomGenerator::global()->bounded(6);
    int hpLoss = qRound(m_player->maxHP() * (percent / 100.0));

    m_player->loseHp(hpLoss);
    m_player->setCoin(m_player->coin() + 75);
}

void FaceTrader::onTrade()
{
    if (!m_player || !m_gamePlay)
        return;

    switch (QRandomGenerator::global()->bounded(3)) {
    case 0:
        m_gamePlay->grantRelicToPlayer(new FaceOfTheCleric());
        break;
    case 1:
        m_gamePlay->grantRelicToPlayer(new GremlinVisage());
        break;
    case 2:
        m_gamePlay->grantRelicToPlayer(new CultistHeadpiece());
        break;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

Colosseum::Colosseum(Player *player, GamePlay *gamePlay, QWidget *parent)
    : Event("The Colosseum",
            ":/events/Pics/Background/Events/Event-Colosseum.png",
            EventScope::Act2,
            player,
            gamePlay,
            parent)
{
    setDescription("A roaring crowd demands a spectacle. One more fight before you can leave...");

    addOption({"Fight",
               "Fight a random Elite\n+ standard Elite rewards",
               "",
               [this]() { onFight(); },
               nullptr});
}

void Colosseum::onFight()
{
    emit triggerEliteCombat();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

PleadingVagrant::PleadingVagrant(Player *player, GamePlay *gamePlay, QWidget *parent)
    : Event("Pleading Vagrant",
            ":/events/Pics/Background/Events/Event-Vagrant.png",
            EventScope::Act2,
            player,
            gamePlay,
            parent)
{
    setDescription("A ragged vagrant begs at your feet, clutching something behind their back...");

    addOption({"Give 85 Gold",
               "Lose 85 Gold\nObtain a random Relic",
               "",
               [this]() { onGive(); },
               [this]() { return m_player && m_player->coin() >= 85; }});

    addOption({"Rob", "Obtain a random Relic\n+ Curse: Regret", "", [this]() { onRob(); }, nullptr});
}

void PleadingVagrant::onGive()
{
    if (!m_player || m_player->coin() < 85)
        return;

    m_player->setCoin(m_player->coin() - 85);
    m_gamePlay->grantRelicToPlayer(Relic::createRandomNormalRelic());
}

void PleadingVagrant::onRob()
{
    if (!m_player || !m_gamePlay)
        return;

    m_gamePlay->grantRelicToPlayer(Relic::createRandomNormalRelic());
    m_gamePlay->addCardToDeck(new Regret());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

TheJoust::TheJoust(Player *player, GamePlay *gamePlay, QWidget *parent)
    : Event("The Joust",
            ":/events/Pics/Background/Events/Event-Joust.png",
            EventScope::Act2,
            player,
            gamePlay,
            parent)
{
    setDescription("Place your bet on the joust...");

    addOption({"Murderer",
               "Bet 50 Gold\n70% chance to win 100 Gold",
               "",
               [this]() { placeBet(70, 100); },
               [this]() { return m_player && m_player->coin() >= 50; }});

    addOption({"Owner",
               "Bet 50 Gold\n30% chance to win 250 Gold",
               "",
               [this]() { placeBet(30, 250); },
               [this]() { return m_player && m_player->coin() >= 50; }});
}

void TheJoust::placeBet(int winChancePercent, int winAmount)
{
    if (!m_player || m_player->coin() < 50)
        return;

    m_player->setCoin(m_player->coin() - 50);

    bool won = QRandomGenerator::global()->bounded(100) < winChancePercent;
    if (won)
        m_player->setCoin(m_player->coin() + winAmount);
}
