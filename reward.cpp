#include "reward.h"

#include <QFont>
#include <QGraphicsTextItem>
#include <QHBoxLayout>
#include <QLinearGradient>
#include <QPushButton>
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <algorithm>
#include <random>

#include "allrelics.h"
#include "attackcards.h"
#include "card.h"
#include "event.h"
#include "gameplay.h"
#include "player.h"
#include "potion.h"
#include "powercards.h"
#include "relic.h"
#include "skillcards.h"

namespace {

Potion *createRandomPotionForReward()
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

} // namespace

RewardScreen::RewardScreen(Player *player,
                           GamePlay *gamePlay,
                           RewardCombatType combatType,
                           QWidget *parent)
    : QWidget(parent)
    , m_player(player)
    , m_gamePlay(gamePlay)
    , m_combatType(combatType)
{
    setFixedSize(1280, 640);

    setupScene();
    grantGoldAndPotion();

    if (m_combatType == RewardCombatType::Elite) {
        bool hasBlackStar = false;
        for (Relic *r : m_player->relics())
            if (r->name() == "Black Star") {
                hasBlackStar = true;
                break;
            }

        int relicCount = hasBlackStar ? 2 : 1;
        QStringList grantedNames;

        for (int i = 0; i < relicCount; ++i) {
            Relic *relic = createRandomNormalRelicForElite();
            if (relic) {
                m_gamePlay->grantRelicToPlayer(relic);
                grantedNames << relic->name();
            }
        }

        m_relicGrantedItem->setPlainText("Relic obtained: " + grantedNames.join(", "));
    }

    m_relicStageResolved = (m_combatType != RewardCombatType::Boss);

    if (m_combatType == RewardCombatType::Boss)
        setupBossRelicChoices();

    setupCardChoices();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_view);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(10, 10, 10, 10);

    m_skipCardBtn = new QPushButton("Skip Card Reward", this);
    connect(m_skipCardBtn, &QPushButton::clicked, this, &RewardScreen::onSkipCardClicked);
    bottomLayout->addWidget(m_skipCardBtn);

    bottomLayout->addStretch();

    m_continueBtn = new QPushButton("Continue", this);
    m_continueBtn->setEnabled(false);
    connect(m_continueBtn, &QPushButton::clicked, this, &RewardScreen::onContinueClicked);
    bottomLayout->addWidget(m_continueBtn);

    mainLayout->addLayout(bottomLayout);

    updateContinueButtonState();
}
RewardScreen::~RewardScreen() {}

void RewardScreen::setupScene()
{
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 1280, 560);

    m_view = new EventGraphicsView(m_scene, this);
    connect(m_view, &EventGraphicsView::itemClicked, this, &RewardScreen::onSceneItemClicked);

    QLinearGradient bgGradient(0, 0, 0, 560);
    bgGradient.setColorAt(0.0, QColor(25, 20, 15));
    bgGradient.setColorAt(1.0, QColor(10, 8, 6));
    m_scene->setBackgroundBrush(bgGradient);

    QString titleText;
    switch (m_combatType) {
    case RewardCombatType::Enemy:
        titleText = "Victory! - Enemy Combat Rewards";
        break;
    case RewardCombatType::Elite:
        titleText = "Victory! - Elite Combat Rewards";
        break;
    case RewardCombatType::Boss:
        titleText = "Victory! - Boss Combat Rewards";
        break;
    }

    m_titleItem = new QGraphicsTextItem(titleText);
    QFont titleFont("Arial", 20, QFont::Bold);
    m_titleItem->setFont(titleFont);
    m_titleItem->setDefaultTextColor(QColor(250, 180, 90));
    m_titleItem->setPos(1280 / 2 - m_titleItem->boundingRect().width() / 2, 15);
    m_scene->addItem(m_titleItem);

    QFont rewardFont("Arial", 13, QFont::Bold);

    m_goldItem = new QGraphicsTextItem();
    m_goldItem->setFont(rewardFont);
    m_goldItem->setDefaultTextColor(QColor(250, 210, 60));
    m_goldItem->setPos(40, 65);
    m_scene->addItem(m_goldItem);

    m_potionItem = new QGraphicsTextItem();
    m_potionItem->setFont(rewardFont);
    m_potionItem->setDefaultTextColor(QColor(150, 220, 255));
    m_potionItem->setPos(40, 90);
    m_scene->addItem(m_potionItem);

    m_relicGrantedItem = new QGraphicsTextItem();
    m_relicGrantedItem->setFont(rewardFont);
    m_relicGrantedItem->setDefaultTextColor(QColor(210, 160, 255));
    m_relicGrantedItem->setPos(40, 115);
    m_scene->addItem(m_relicGrantedItem);

    m_relicChoiceLabel = new QGraphicsTextItem();
    m_relicChoiceLabel->setFont(rewardFont);
    m_relicChoiceLabel->setDefaultTextColor(QColor(229, 212, 179));
    m_relicChoiceLabel->setPos(1280 / 2 - 200, 165);
    m_scene->addItem(m_relicChoiceLabel);

    m_cardChoiceLabel = new QGraphicsTextItem();
    m_cardChoiceLabel->setFont(rewardFont);
    m_cardChoiceLabel->setDefaultTextColor(QColor(229, 212, 179));
    m_scene->addItem(m_cardChoiceLabel);
}

void RewardScreen::grantGoldAndPotion()
{
    int minGold = 0;
    int maxGold = 0;

    switch (m_combatType) {
    case RewardCombatType::Enemy:
        minGold = 15;
        maxGold = 25;
        break;
    case RewardCombatType::Elite:
        minGold = 30;
        maxGold = 40;
        break;
    case RewardCombatType::Boss:
        minGold = 80;
        maxGold = 80;
        break;
    }

    m_goldAmount = minGold + QRandomGenerator::global()->bounded(maxGold - minGold + 1);
    m_player->setCoin(m_player->coin() + m_goldAmount);
    m_goldItem->setPlainText(QString("Gold obtained: %1").arg(m_goldAmount));

    if (m_combatType == RewardCombatType::Boss) {
        m_potionItem->setPlainText("");
        return;
    }

    int potionChance = (m_combatType == RewardCombatType::Enemy) ? 40 : 60;
    bool potionDropped = QRandomGenerator::global()->bounded(100) < potionChance;

    if (!potionDropped) {
        m_potionItem->setPlainText("");
        return;
    }

    Potion *potion = createRandomPotionForReward();
    if (m_player->addPotion(potion))
        m_potionItem->setPlainText("Potion obtained!");
    else {
        delete potion;
        m_potionItem->setPlainText("A Potion dropped, but your bag is full!");
    }
}

Card *RewardScreen::createRandomCard(bool rare) const
{
    if (!rare) {
        switch (QRandomGenerator::global()->bounded(10)) {
        case 0:
            return new class Strike();
        case 1:
            return new class Bash();
        case 2:
            return new class Clash();
        case 3:
            return new class Hemokinesis();
        case 4:
            return new class BloodForBlood();
        case 5:
            return new class Whirlwind();
        case 6:
            return new class Defend();
        case 7:
            return new class Entrench();
        case 8:
            return new class Bloodletting();
        case 9:
            return new class Inflame();
        default:
            return new class Strike();
        }
    }

    switch (QRandomGenerator::global()->bounded(8)) {
    case 0:
        return new class Bludgeon();
    case 1:
        return new class Reaper();
    case 2:
        return new class Feed();
    case 3:
        return new class Immolate();
    case 4:
        return new class Exhume();
    case 5:
        return new class Limit_Break();
    case 6:
        return new class Offering();
    case 7:
        return new class Impervious();
    default:
        return new class Bludgeon();
    }
}

Relic *RewardScreen::createRandomNormalRelicForElite() const
{
    switch (QRandomGenerator::global()->bounded(5)) {
    case 0:
        return new Anchor();
    case 1:
        return new Girya();
    case 2:
        return new IceCream();
    case 3:
        return new Kunai();
    case 4:
        return new Shuriken();
    default:
        return new Anchor();
    }
}

void RewardScreen::setupCardChoices()
{
    m_cardChoiceLabel->setPlainText("Choose a card to add to your deck (or Skip)");
    qreal labelY = (m_combatType == RewardCombatType::Boss) ? 340 : 200;
    m_cardChoiceLabel->setPos(1280 / 2 - m_cardChoiceLabel->boundingRect().width() / 2, labelY);

    const qreal spacing = 220;
    const qreal cardY = labelY + 40;
    qreal totalWidth = spacing * 3;
    qreal startX = 1280 / 2 - totalWidth / 2 + (spacing - 170) / 2;

    for (int i = 0; i < 3; ++i) {
        bool rare = QRandomGenerator::global()->bounded(100) < 10;
        Card *card = createRandomCard(rare);

        card->setFlag(QGraphicsItem::ItemIsMovable, false);
        card->setFlag(QGraphicsItem::ItemIsSelectable, false);
        card->setPos(startX + i * spacing, cardY);

        m_scene->addItem(card);
        m_cardChoices.append(card);
    }
}

void RewardScreen::setupBossRelicChoices()
{
    m_relicChoiceLabel->setPlainText("Choose a Boss Relic");
    m_relicChoiceLabel->setPos(1280 / 2 - m_relicChoiceLabel->boundingRect().width() / 2, 165);

    struct BossRelicInfo
    {
        std::function<Relic *()> factory;
        QString title;
        QString subtitle;
        QString iconPath;
    };

    QVector<BossRelicInfo> pool = {{[]() -> Relic * { return new CallingBell(); },
                                    "Calling Bell",
                                    "Obtain the Curse of the Bell\n+ 3 random normal relics",
                                    ":/icons/Pics/Icons/relic/boss/calling_bell.png"},
                                   {[]() -> Relic * { return new MarkOfPain(); },
                                    "Mark of Pain",
                                    "+1 Energy per turn\n2 WOUNDS added to draw pile",
                                    ":/icons/Pics/Icons/relic/boss/MarkofPain.png"},
                                   {[]() -> Relic * { return new VelvetChoker(); },
                                    "Velvet Choker",
                                    "+1 Energy per turn\nMax 6 cards played per turn",
                                    ":/icons/Pics/Icons/relic/boss/velvet_choker.png"},
                                   {[]() -> Relic * { return new BlackStar(); },
                                    "Black Star",
                                    "Elites now drop\n2 relics when defeated",
                                    ":/icons/Pics/Icons/relic/boss/black_star.png"}};

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(pool.begin(), pool.end(), g);

    const qreal spacing = 260;
    qreal totalWidth = spacing * 3;
    qreal startX = 1280 / 2 - totalWidth / 2 + (spacing - 220) / 2;
    qreal optionY = 195;

    for (int i = 0; i < 3; ++i) {
        const BossRelicInfo &info = pool[i];

        auto *item = new SelectableOptionItem(info.title, info.subtitle, info.iconPath);
        item->setPos(startX + spacing * i, optionY);
        m_scene->addItem(item);
        m_bossRelicItems.append(item);
        m_bossRelicFactories.append(info.factory);

        connect(item, &SelectableOptionItem::clicked, this, [this, i]() {
            if (m_relicStageResolved)
                return;

            Relic *relic = m_bossRelicFactories[i]();
            m_gamePlay->grantRelicToPlayer(relic);
            m_relicGrantedItem->setPlainText("Relic obtained: " + relic->name());

            for (SelectableOptionItem *option : m_bossRelicItems) {
                m_scene->removeItem(option);
                delete option;
            }
            m_bossRelicItems.clear();

            m_relicStageResolved = true;
            updateContinueButtonState();
        });
    }
}

void RewardScreen::onSceneItemClicked(QGraphicsItem *item)
{
    if (m_cardStageResolved)
        return;

    Card *card = qgraphicsitem_cast<Card *>(item);
    if (!card && item->parentItem())
        card = qgraphicsitem_cast<Card *>(item->parentItem());

    if (!card || !m_cardChoices.contains(card))
        return;

    m_gamePlay->addCardToDeck(card);
    m_cardChoices.removeAll(card);

    for (Card *other : m_cardChoices) {
        m_scene->removeItem(other);
        delete other;
    }
    m_cardChoices.clear();

    resolveCardStage();
}

void RewardScreen::onSkipCardClicked()
{
    if (m_cardStageResolved)
        return;

    for (Card *card : m_cardChoices) {
        m_scene->removeItem(card);
        delete card;
    }
    m_cardChoices.clear();

    resolveCardStage();
}

void RewardScreen::resolveCardStage()
{
    m_cardChoiceLabel->setPlainText("");
    m_cardStageResolved = true;
    m_skipCardBtn->setEnabled(false);
    updateContinueButtonState();
}

void RewardScreen::updateContinueButtonState()
{
    m_continueBtn->setEnabled(m_cardStageResolved && m_relicStageResolved);
}

void RewardScreen::onContinueClicked()
{
    emit rewardFinished();
}
