#include "allenemies.h"

Cultist::Cultist(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Cultist", 48, 54, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void Cultist::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = buffIntent(3);
    } else {
        m_currentIntent = attackIntent(6);

        if (!m_hasIncantation) {
            m_hasIncantation = true;

            powerEffects().append(PowerEffect{3,
                                              [](Combatant *self, int value, GamePlay *) {
                                                  self->applyBuffDebuff(BuffDebuffType::Strength,
                                                                        value);
                                              },
                                              PowerUseTime::StartTurn});
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

JawWorm::JawWorm(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Jaw Worm", 42, 46, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void JawWorm::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = attackIntent(11);
        return;
    }

    QVector<QPair<int, EnemyIntent>> options = {{25, attackIntent(11)},
                                                {30, attackDefendIntent(7, 5)},
                                                {45, defendBuffIntent(6, 3)}};

    m_currentIntent = pickIntent(options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Louse::Louse(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy((rand() % 2 == 0) ? "Green Louse" : "Red Louse",
            10,
            15,
            enemyType::Normal,
            isMultiplayer,
            parent)
{
    m_soucePath = "";
    loadPic();
    m_isRed = (m_name == "Red Louse");
    calculateNextIntent();
}

int Louse::randomBiteDamage() const
{
    return 5 + rand() % 3;
}

void Louse::calculateNextIntent()
{
    m_turnCount++;

    QVector<QPair<int, EnemyIntent>> options = {{75, attackIntent(randomBiteDamage())},
                                                {25, buffIntent(3)}};

    m_currentIntent = pickIntent(options);
}

int Louse::takeDamage(int incomingDamage, bool isAttackDamage)
{
    int damage = Combatant::takeDamage(incomingDamage, isAttackDamage);

    if (damage > 0 && !m_defensiveReactionUsed) {
        m_defensiveReactionUsed = true;
        addBlock(3 + rand() % 5);
    }

    return damage;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Slime::Slime(
    QString name, int minHP, int maxHP, enemyType type, bool isMultiplayer, QGraphicsItem *parent)
    : Enemy(name, minHP, maxHP, type, isMultiplayer, parent)
{}

bool Slime::shouldSplit() const
{
    return false;
}

AcidSlimeS::AcidSlimeS(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("Small Slime", 8, 12, enemyType::Normal, isMultiplayer, parent)
{
    calculateNextIntent();
}

void AcidSlimeS::calculateNextIntent()
{
    m_turnCount++;

    QVector<QPair<int, EnemyIntent>> options = {{50, attackIntent(3)}, {50, debuffIntent(1)}};

    m_currentIntent = pickIntent(options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

AcidSlimeM::AcidSlimeM(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("Medium Slime", 28, 32, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void AcidSlimeM::calculateNextIntent()
{
    m_turnCount++;

    QVector<QPair<int, EnemyIntent>> options = {{30, attackDebuffIntent(7, 1)},
                                                {40, attackIntent(10)},
                                                {30, debuffIntent(1)}};

    m_currentIntent = pickIntent(options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

AcidSlimeL::AcidSlimeL(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("Large Slime", 68, 72, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void AcidSlimeL::calculateNextIntent()
{
    m_turnCount++;

    if (!m_hasSplit && m_currentHP <= m_maxHP / 2) {
        m_currentIntent = {IntentType::Unknown, 0, 0, false};

        return;
    }

    QVector<QPair<int, EnemyIntent>> options = {{30, attackDebuffIntent(7, 1)},
                                                {40, attackIntent(10)},
                                                {30, debuffIntent(1)}};

    m_currentIntent = pickIntent(options);
}

bool AcidSlimeL::shouldSplit() const
{
    return !m_hasSplit && m_currentHP <= m_maxHP / 2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Thief::Thief(
    QString name, int minHP, int maxHP, int mugDamage, bool isMultiplayer, QGraphicsItem *parent)
    : Enemy(name, minHP, maxHP, enemyType::Normal, isMultiplayer, parent)
    , m_mugDamage(mugDamage)
{
    calculateNextIntent();
}

bool Thief::hasEscaped() const
{
    return m_hasEscaped;
}

int Thief::stolenGold() const
{
    return m_stolenGold;
}

void Thief::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount <= 2) {
        m_currentIntent = attackDebuffIntent(m_mugDamage, 15);

        return;
    }

    if (m_turnCount == 3) {
        m_currentIntent = defendIntent(6);

        return;
    }

    m_currentIntent = escapeIntent();

    m_hasEscaped = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Looter::Looter(bool isMultiplayer, QGraphicsItem *parent)
    : Thief("Looter", 44, 48, 10, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mugger::Mugger(bool isMultiplayer, QGraphicsItem *parent)
    : Thief("Mugger", 52, 56, 14, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

BlueSlaver::BlueSlaver(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Blue Slaver", 46, 50, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void BlueSlaver::calculateNextIntent()
{
    m_turnCount++;

    QVector<QPair<int, EnemyIntent>> options
        = {{60, attackIntent(12)}, {40, EnemyIntent{IntentType::AttackDebuff, 7, 1, false}}};

    m_currentIntent = pickIntent(options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

RedSlaver::RedSlaver(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Red Slaver", 46, 50, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void RedSlaver::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = attackIntent(13);
        return;
    }

    QVector<QPair<int, EnemyIntent>> options;

    if (!m_usedEntangle)
        options.append({25, entangleIntent()});

    options.append({50, attackIntent(13)});
    options.append({50, attackDebuffIntent(8, 1)});

    m_currentIntent = pickIntent(options);

    if (m_currentIntent.type == IntentType::Entangle)
        m_usedEntangle = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

SphericGuardian::SphericGuardian(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Spheric Guardian", 20, 20, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    addBlock(25);
    calculateNextIntent();
}

void SphericGuardian::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = attackDebuffIntent(10, 5);
        return;
    }

    QVector<QPair<int, EnemyIntent>> options = {{50, attackDefendIntent(10, 15)},
                                                {50, EnemyIntent{IntentType::Attack, 10, 2, false}}};

    m_currentIntent = pickIntent(options);
}

GremlinKnob::GremlinKnob(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Gremlin Knob", 82, 86, enemyType::Elite, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void GremlinKnob::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = buffIntent(99);
        m_enrageStacks = 99;
        return;
    }

    QVector<QPair<int, EnemyIntent>> options = {{67, attackIntent(14)},
                                                {33, attackDebuffIntent(6, 2)}};
    m_currentIntent = pickIntent(options);
}

void GremlinKnob::onPlayerSkillPlayed()
{
    m_soucePath = "";
    loadPic();
    if (m_enrageStacks > 0)
        applyBuffDebuff(BuffDebuffType::Strength, 2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Sentry::Sentry(bool startsWithBeam, bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Sentry", 38, 42, enemyType::Elite, isMultiplayer, parent)
    , m_startsWithBeam(startsWithBeam)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void Sentry::calculateNextIntent()
{
    m_turnCount++;

    bool useBeam = m_startsWithBeam ? (m_turnCount % 2 == 1) : (m_turnCount % 2 == 0);

    if (useBeam)
        m_currentIntent = attackIntent(9);
    else
        m_currentIntent = debuffIntent(2);

    // if (!useBeam) {
    //     player->shuffleIntoDiscard(new DAZE());
    //     player->shuffleIntoDiscard(new DAZE());
    // }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

BookOfStabbing::BookOfStabbing(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Book of Stabbing", 160, 162, enemyType::Elite, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void BookOfStabbing::calculateNextIntent()
{
    m_turnCount++;

    QVector<QPair<int, EnemyIntent>> options
        = {{85, EnemyIntent{IntentType::Attack, 6, m_multiStabUsedCount + 2, true}},
           {15, EnemyIntent{IntentType::Attack, 21, 1, false}}};

    m_currentIntent = pickIntent(options);

    if (m_currentIntent.secondaryValue != 1 || m_currentIntent.value != 21)
        m_multiStabUsedCount++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Taskmaster::Taskmaster(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Taskmaster", 54, 60, enemyType::Elite, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void Taskmaster::calculateNextIntent()
{
    m_turnCount++;
    m_currentIntent = EnemyIntent{IntentType::Attack, 7, 1, false};

    // player->addCardToDiscard(new WOUND());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

KingSlime::KingSlime(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("King Slime", 140, 140, enemyType::Boss, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void KingSlime::calculateNextIntent()
{
    m_turnCount++;

    if (!m_hasSplit && shouldSplit()) {
        m_currentIntent = unknownIntent();
        return;
    }

    int phase = (m_turnCount - 1) % 3;

    switch (phase) {
    case 0:
        m_currentIntent = unknownIntent();
        break;
    case 1:
        m_currentIntent = attackIntent(35);
        break;
    case 2:
        m_currentIntent = debuffIntent(3);
        break;
    }

    // if (phase == 2) {
    //     for (int i = 0; i < 3; ++i)
    //         player->addCardToDiscard(new SLIME());
    // }
}

bool KingSlime::shouldSplit() const
{
    return !m_hasSplit && m_currentHP <= m_maxHP / 2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

HexaGhost::HexaGhost(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("HexaGhost", 250, 250, enemyType::Boss, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void HexaGhost::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = unknownIntent();
        return;
    }

    if (m_turnCount == 2) {
        int dividerDamage = 1; // placeholder
        m_currentIntent = EnemyIntent{IntentType::Attack, dividerDamage, 6, false};
        return;
    }

    int cycleIndex = (m_turnCount - 3) % 7;

    switch (cycleIndex) {
    case 0: // Sear
    case 2:
    case 5:
        m_currentIntent = EnemyIntent{IntentType::Attack, 6, 1, false};
        break;
    case 1: // Tackle
    case 4:
        m_currentIntent = EnemyIntent{IntentType::Attack, 5, 2, false};
        break;
    case 3: // Inflame
        m_currentIntent = defendBuffIntent(12, 2);
        break;
    case 6: // Inferno
        m_currentIntent = EnemyIntent{IntentType::Attack, 2, 6, false};
        break;
    }

    // if (cycleIndex == 0 || cycleIndex == 2 || cycleIndex == 5)
    //     player->addCardToDiscard(new Burn());
    // if (cycleIndex == 6) {
    //     for (int i = 0; i < 3; ++i)
    //         player->addCardToDiscard(new Burn());
    //     player->upgradeAllBurnsInDeck();
    // }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

TheChamp::TheChamp(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("The Champ", 420, 420, enemyType::Boss, isMultiplayer, parent)
{
    m_soucePath = "";
    loadPic();
    calculateNextIntent();
}

void TheChamp::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount % 2 == 1) {
        m_currentIntent = EnemyIntent{IntentType::Debuff, 2, 2, false};
        return;
    }

    QVector<QPair<int, EnemyIntent>> options = {{15, defendBuffIntent(15, 5)},
                                                {15, buffIntent(2)},
                                                {25, attackDebuffIntent(12, 2)},
                                                {45, EnemyIntent{IntentType::Attack, 8, 2, false}}};

    m_currentIntent = pickIntent(options);
}