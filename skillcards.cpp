#include "skillcards.h"
#include "enemy.h"
#include "gameplay.h"
#include "player.h"
#include "statuscards.h"

SkillCard::SkillCard(CardID ID,
                     QString name,
                     int energyCost,
                     QString description,
                     bool isRare,
                     bool isExhaust,
                     bool requiresTarget,
                     QGraphicsItem *parent)
    : Card(ID, name, CardType::Skill, energyCost, description, isRare, isExhaust, requiresTarget, parent)
{}

SkillCard::~SkillCard() {}

QString SkillCard::getDynamicDescription(Player *player, Enemy *target) const
{
    return m_description;
}

Defend::Defend(QGraphicsItem *parent)
    : SkillCard(CardID::Defend, "Defend", 1, "Gain %1 block", false, false, true, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Skill/defend_ironclad.png";
    loadPixmap();
}

void Defend::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr)
        player->addBlockFromCard(m_blockAmount);
}

void Defend::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_blockAmount += 3;

    m_name = "Defend+";
}

Card *Defend::clone() const
{
    Defend *copy = new Defend();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

QString Defend::getDynamicDescription(Player *player, Enemy *target) const
{
    int finalBlock = m_blockAmount;

    if (player)
        finalBlock = player->calculateBlock(m_blockAmount);

    return m_description.arg(finalBlock);
}

static inline bool DefendRegistered = []()
{
    Card::creators()[CardID::Defend] = [](){return new class Defend;};
    return true;
}();

Exhume::Exhume(QGraphicsItem *parent)
    : SkillCard(CardID::Exhume, "Exhume", 1, "Put a card from exhaust pile into hand", true, true, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Skill/Red-Exhume-Art.png";
    loadPixmap();
}

void Exhume::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);
    Q_UNUSED(player);
}
bool Exhume::applyEffect(GamePlay *gameplay)
{
    gameplay->drawFromExhaustPile();
    return true;
}

void Exhume::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_energyCost -= 1;
    m_name = "Exhume+";
}

Card *Exhume::clone() const
{
    Exhume *copy = new Exhume();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool ExhumeRegistered = []()
{
    Card::creators()[CardID::Exhume] = [](){return new class Exhume;};
    return true;
}();

Limit_Break::Limit_Break(QGraphicsItem *parent)
    : SkillCard(CardID::LimitBreak, "Limit Break", 1, "Double your Strength\nExhaust", true, true, true, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Skill/Red-LimitBreak-Art.png";
    loadPixmap();
}

void Limit_Break::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr) {
        int currentStrength = player->effectStacks(BuffDebuffType::Strength);
        player->applyBuffDebuff(BuffDebuffType::Strength, currentStrength);
    }
}

void Limit_Break::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_isExhaust = false;
    m_description = "Double your Strength";
    m_name = "Limit Break+";
}

Card *Limit_Break::clone() const
{
    Limit_Break *copy = new Limit_Break();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool Limit_BreakRegistered = []()
{
    Card::creators()[CardID::LimitBreak] = [](){return new class Limit_Break;};
    return true;
}();

Offering::Offering(QGraphicsItem *parent)
    : SkillCard(CardID::Offering, "Offering", 0, "Lose 6 HP\nGain 2 Energy\nDraw 3 cards", true, true, true, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Skill/offering.png";
    loadPixmap();
}

void Offering::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr) {
        player->loseHp(6);
        player->addEnergy(2);
    }
}

bool Offering::applyEffect(GamePlay *gameplay)
{
    for (int i = 0; i < m_drawCount; i++)
        gameplay->drawFromDrawPile();

    return true;
}

void Offering::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_drawCount += 2;
    m_name = "Offering+";
    m_description = "Lose 6 HP\nGain 2 Energy\nDraw 5 cards";
}

Card *Offering::clone() const
{
    Offering *copy = new Offering();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool OfferingRegistered = []()
{
    Card::creators()[CardID::Offering] = [](){return new class Offering;};
    return true;
}();

Impervious::Impervious(QGraphicsItem *parent)
    : SkillCard(CardID::Impervious, "Impervious", 2, "Gain %1 block", true, true, true, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Skill/impervious.png";
    loadPixmap();
}

void Impervious::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr)
        player->addBlockFromCard(m_blockAmount);
}

void Impervious::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();
    m_blockAmount += 10;
    m_name = "Impervios+";
}

Card *Impervious::clone() const
{
    Impervious *copy = new Impervious();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

QString Impervious::getDynamicDescription(Player *player, Enemy *target) const
{
    int finalBlock = m_blockAmount;

    if (player)
        finalBlock = player->calculateBlock(m_blockAmount);

    return m_description.arg(finalBlock);
}

static inline bool ImperviousRegistered = []()
{
    Card::creators()[CardID::Impervious] = [](){return new class Impervious;};
    return true;
}();

Power_Through::Power_Through(QGraphicsItem *parent)
    : SkillCard(CardID::PowerThrough, "Power Through",
                1,
                "Gain %1 block\nAdd 2 WOUNDs into hand",
                false,
                false,
                true,
                parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Skill/Red-PowerThrough-Art.png";
    loadPixmap();
}

void Power_Through::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr)
        player->addBlockFromCard(m_blockAmount);
}

bool Power_Through::applyEffect(GamePlay* gameplay)
{
    // gameplay->addCardToHand(new WOUND());
    // gameplay->addCardToHand(new WOUND());
    return true;
}

void Power_Through::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_blockAmount += 5;
    m_name = "Power Through+";
}

Card *Power_Through::clone() const
{
    Power_Through *copy = new Power_Through();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

QString Power_Through::getDynamicDescription(Player *player, Enemy *target) const
{
    int finalBlock = m_blockAmount;

    if (player)
        finalBlock = player->calculateBlock(m_blockAmount);

    return m_description.arg(finalBlock);
}

static inline bool Power_ThroughRegistered = []()
{
    Card::creators()[CardID::PowerThrough] = [](){return new class Power_Through;};
    return true;
}();

Bloodletting::Bloodletting(QGraphicsItem *parent)
    : SkillCard(CardID::Bloodletting, "Bloodletting", 0, "Lose 3 HP\nGain 2 Energy", false, false, true, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Skill/bloodletting.png";
    loadPixmap();
}

void Bloodletting::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr) {
        player->loseHp(3);
        player->addEnergy(m_energyGain);
    }
}

void Bloodletting::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_energyGain += 1;
    m_name = "Bloodletting+";
    m_description = "Lose 3 HP\nGain 3 Energy";
}

Card *Bloodletting::clone() const
{
    Bloodletting *copy = new Bloodletting();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool BloodlettingRegistered = []()
{
    Card::creators()[CardID::Bloodletting] = [](){return new class Bloodletting;};
    return true;
}();

Entrench::Entrench(QGraphicsItem *parent)
    : SkillCard(CardID::Entrench, "Entrench", 2, "Double your Block", false, false, false, parent)
{
    m_sourcePath = "";
    loadPixmap();
}

void Entrench::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(targetEnemy);

    if (player != nullptr) {
        int currentBlock = player->block();
        player->addBlockFromCard(currentBlock);
    }
}

void Entrench::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_energyCost -= 1;
    m_name = "Entrench+";
}

Card *Entrench::clone() const
{
    Entrench *copy = new Entrench();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool EntrenchRegistered = []()
{
    Card::creators()[CardID::Entrench] = [](){return new class Entrench;};
    return true;
}();

Dual_Wield::Dual_Wield(QGraphicsItem *parent)
    : SkillCard(CardID::DualWield,
                "Dual Wield",
                1,
                "Choose a card in hand\nAdd 2 copies into hand",
                false,
                false,
                false,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Skill/DualWield.png";
    loadPixmap();
}

void Dual_Wield::applyEffect(Player *player, Enemy *targetEnemy)
{
    Q_UNUSED(player);
    Q_UNUSED(targetEnemy);
}

bool Dual_Wield::applyEffect(GamePlay *gameplay)
{
    if (!gameplay)
        return false;

    Card *selected = gameplay->selectedHandCard();

    if (!selected)
        return false;

    for (int i = 0; i < m_copyCount; ++i) {
        Card *copy = selected->clone();
        copy->setLifetime(CardLifetime::EndOfCombat);
        gameplay->addCardToHand(copy);
    }

    return true;
}

void Dual_Wield::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_copyCount++;
    m_name = "Dual Wield+";
    m_description = "Choose a card in hand\nAdd 3 copies into hand";
}

Card *Dual_Wield::clone() const
{
    Dual_Wield *copy = new Dual_Wield();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool Dual_WieldRegistered = []()
{
    Card::creators()[CardID::DualWield] = [](){return new class Dual_Wield;};
    return true;
}();