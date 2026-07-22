#include "allenemies.h"
#include "gameplay.h"
#include "player.h"
#include "statuscards.h"

Cultist::Cultist(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Cultist", 48, 54, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/normal/Cultist.png";
    loadPic();
    Cultist::calculateNextIntent();
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

            powerEffects().append(PowerEffect{":/icons/Pics/Enemies/enemy/normal/Icon_Ritual.png",
                                              3,
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
    m_soucePath = ":/enemies/Pics/Enemies/enemy/normal/JawWorm.png";
    loadPic();
    JawWorm::calculateNextIntent();
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
    : Enemy((rollBounded(2) == 0) ? "Green Louse" : "Red Louse",
            10,
            15,
            enemyType::Normal,
            isMultiplayer,
            parent)
{
    m_isRed = (m_name == "Red Louse");
    m_soucePath = (m_isRed) ? ":/enemies/Pics/Enemies/enemy/normal/RedLouse.png"
                            : ":/enemies/Pics/Enemies/enemy/normal/GreenLouse.png";
    loadPic();
    Louse::calculateNextIntent();
}

int Louse::randomBiteDamage() const
{
    return 5 + rollBounded(3);
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
        addBlock(3 + rollBounded(5));
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

bool Slime::needsToSplit() const
{
    return !m_hasSplit && shouldSplit();
}

void Slime::markSplit()
{
    m_hasSplit = true;
}

QVector<Enemy *> Slime::createSplitChildren(bool isMultiplayer) const
{
    Q_UNUSED(isMultiplayer);
    return {};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

AcidSlimeS::AcidSlimeS(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("Small Slime", 8, 12, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/normal/AcidSlimeS.png";
    loadPic();
    AcidSlimeS::calculateNextIntent();
}

void AcidSlimeS::calculateNextIntent()
{
    m_turnCount++;

    QVector<QPair<int, EnemyIntent>> options = {{50, attackIntent(3)}, {50, debuffIntent(1)}};

    m_currentIntent = pickIntent(options);
}

bool AcidSlimeS::shouldSplit() const
{
    return false;
}

void AcidSlimeS::executeIntent(GamePlay *game)
{
    if (needsToSplit())
        return;
    Enemy::executeIntent(game);
}

QVector<Enemy *> AcidSlimeS::createSplitChildren(bool isMultiplayer) const
{
    QVector<Enemy *> children;
    children.clear();
    return children;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

AcidSlimeM::AcidSlimeM(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("Medium Slime", 28, 32, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/normal/AcidSlimeM.png";
    loadPic();
    AcidSlimeM::calculateNextIntent();
}

void AcidSlimeM::calculateNextIntent()
{
    m_turnCount++;

    QVector<QPair<int, EnemyIntent>> options = {{30, attackDebuffIntent(7, 1)},
                                                {40, attackIntent(10)},
                                                {30, debuffIntent(1)}};

    m_currentIntent = pickIntent(options);
}

bool AcidSlimeM::shouldSplit() const
{
    return m_currentHP <= m_maxHP / 2;
}

void AcidSlimeM::executeIntent(GamePlay *game)
{
    if (needsToSplit())
        return;
    Enemy::executeIntent(game);
}

QVector<Enemy *> AcidSlimeM::createSplitChildren(bool isMultiplayer) const
{
    QVector<Enemy *> children;

    if (m_currentHP <= 12) {
        for (int i = 0; i < 2; ++i) {
            AcidSlimeS *child = new AcidSlimeS(isMultiplayer);
            child->overrideHP(m_currentHP);
            children.append(child);
        }
    }

    return children;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

AcidSlimeL::AcidSlimeL(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("Large Slime", 68, 72, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/normal/AcidSlimeL.png";
    loadPic();
    AcidSlimeL::calculateNextIntent();
}

void AcidSlimeL::calculateNextIntent()
{
    m_turnCount++;

    if (needsToSplit()) {
        m_currentIntent = unknownIntent();
        return;
    }

    QVector<QPair<int, EnemyIntent>> options = {{30, attackDebuffIntent(7, 1)},
                                                {40, attackIntent(10)},
                                                {30, debuffIntent(1)}};

    m_currentIntent = pickIntent(options);
}

bool AcidSlimeL::shouldSplit() const
{
    return m_currentHP <= m_maxHP / 2;
}

void AcidSlimeL::executeIntent(GamePlay *game)
{
    if (needsToSplit())
        return;
    Enemy::executeIntent(game);
}

QVector<Enemy *> AcidSlimeL::createSplitChildren(bool isMultiplayer) const
{
    QVector<Enemy *> children;

    if (m_currentHP <= 12) {
        for (int i = 0; i < 2; ++i) {
            AcidSlimeS *child = new AcidSlimeS(isMultiplayer);
            child->overrideHP(m_currentHP);
            children.append(child);
            return children;
        }
    }

    else if (m_currentHP <= 32) {
        for (int i = 0; i < 2; ++i) {
            AcidSlimeM *child = new AcidSlimeM(isMultiplayer);
            child->overrideHP(m_currentHP);
            children.append(child);
        }
    }

    return children;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Thief::Thief(
    QString name, int minHP, int maxHP, int mugDamage, bool isMultiplayer, QGraphicsItem *parent)
    : Enemy(name, minHP, maxHP, enemyType::Normal, isMultiplayer, parent)
    , m_mugDamage(mugDamage)
{
    Thief::calculateNextIntent();
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
    m_soucePath = ":/enemies/Pics/Enemies/enemy/normal/Looter.png";
    loadPic();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mugger::Mugger(bool isMultiplayer, QGraphicsItem *parent)
    : Thief("Mugger", 52, 56, 14, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/normal/Mugger.png";
    loadPic();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

BlueSlaver::BlueSlaver(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Blue Slaver", 46, 50, enemyType::Normal, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/normal/SlaverBlue.png";
    loadPic();
    BlueSlaver::calculateNextIntent();
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
    m_soucePath = ":/enemies/Pics/Enemies/enemy/normal/SlaverRed.png";
    loadPic();
    RedSlaver::calculateNextIntent();
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
    m_soucePath = ":/enemies/Pics/Enemies/enemy/normal/SphericGuardian.png";
    loadPic();
    addBlock(25);
    SphericGuardian::calculateNextIntent();
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////

GremlinKnob::GremlinKnob(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Gremlin Knob", 82, 86, enemyType::Elite, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/elite/GremlinNob.png";
    loadPic();
    GremlinKnob::calculateNextIntent();
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

void GremlinKnob::onAnyCardPlayed(CardType cardType, GamePlay *game)
{
    Q_UNUSED(game);

    if (m_enrageStacks > 0 && cardType == CardType::Skill)
        applyBuffDebuff(BuffDebuffType::Strength, 2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Sentry::Sentry(bool startsWithBeam, bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Sentry", 38, 42, enemyType::Elite, isMultiplayer, parent)
    , m_startsWithBeam(startsWithBeam)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/elite/Sentry.png";
    loadPic();
    Sentry::calculateNextIntent();
}

void Sentry::calculateNextIntent()
{
    m_turnCount++;

    bool useBeam = m_startsWithBeam ? (m_turnCount % 2 == 1) : (m_turnCount % 2 == 0);

    if (useBeam)
        m_currentIntent = attackIntent(9);
    else
        m_currentIntent = debuffIntent(2);
}

void Sentry::onIntentExecuted(GamePlay *game)
{
    if (!game)
        return;

    if (m_currentIntent.type == IntentType::Debuff) {
        game->addCardToDrawPile(new DAZE(), true);
        game->addCardToDrawPile(new DAZE(), true);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

BookOfStabbing::BookOfStabbing(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Book of Stabbing", 160, 162, enemyType::Elite, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/elite/BookOfStabbing.png";
    loadPic();
    BookOfStabbing::calculateNextIntent();
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
    m_soucePath = ":/enemies/Pics/Enemies/enemy/elite/Taskmaster.png";
    loadPic();
    Taskmaster::calculateNextIntent();
}

void Taskmaster::calculateNextIntent()
{
    m_turnCount++;
    m_currentIntent = EnemyIntent{IntentType::Attack, 7, 1, false};
}

void Taskmaster::onIntentExecuted(GamePlay *game)
{
    if (!game)
        return;

    game->addCardToDiscardPile(new WOUND());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

KingSlime::KingSlime(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("King Slime", 140, 140, enemyType::Boss, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/boss/SlimeBoss.png";
    loadPic();
    KingSlime::calculateNextIntent();
}

void KingSlime::calculateNextIntent()
{
    m_turnCount++;

    if (needsToSplit()) {
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
        m_currentIntent = unknownIntent(); // Goop Spray
        break;
    }
}

bool KingSlime::shouldSplit() const
{
    return m_currentHP <= m_maxHP / 2;
}

void KingSlime::executeIntent(GamePlay *game)
{
    if (needsToSplit())
        return;
    Enemy::executeIntent(game);
}

QVector<Enemy *> KingSlime::createSplitChildren(bool isMultiplayer) const
{
    QVector<Enemy *> children;

    for (int i = 0; i < 2; ++i) {
        AcidSlimeL *child = new AcidSlimeL(isMultiplayer);
        child->overrideHP(m_currentHP);
        children.append(child);
    }

    return children;
}

void KingSlime::onIntentExecuted(GamePlay *game)
{
    if (!game || needsToSplit())
        return;

    int phase = (m_turnCount - 1) % 3;
    if (phase == 2) {
        for (int i = 0; i < 3; ++i)
            game->addCardToDiscardPile(new SLIME());
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

HexaGhost::HexaGhost(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("HexaGhost", 250, 250, enemyType::Boss, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/boss/Hexaghost.png";
    loadPic();
    HexaGhost::calculateNextIntent();
}

void HexaGhost::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = unknownIntent();
        m_lastCycleIndex = -1;
        return;
    }

    if (m_turnCount == 2) {
        int dividerDamage = 1;
        m_currentIntent = EnemyIntent{IntentType::Attack, dividerDamage, 6, false};
        m_lastCycleIndex = -1;
        return;
    }

    int cycleIndex = (m_turnCount - 3) % 7;
    m_lastCycleIndex = cycleIndex;

    switch (cycleIndex) {
    case 0:
    case 2:
    case 5:
        m_currentIntent = EnemyIntent{IntentType::Attack, 6, 1, false};
        break;
    case 1:
    case 4:
        m_currentIntent = EnemyIntent{IntentType::Attack, 5, 2, false};
        break;
    case 3:
        m_currentIntent = defendBuffIntent(12, 2);
        break;
    case 6:
        m_currentIntent = EnemyIntent{IntentType::Attack, 2, 6, false};
        break;
    }
}

void HexaGhost::onIntentExecuted(GamePlay *game)
{
    if (!game)
        return;

    // Sear (فازهای 0، 2، 5): یک BURN به Discard اضافه می‌شود
    if (m_lastCycleIndex == 0 || m_lastCycleIndex == 2 || m_lastCycleIndex == 5) {
        game->addCardToDiscardPile(new BURN());
    }
    // Inferno (فاز 6): همه BURNهای موجود ارتقا پیدا می‌کنند + 3 BURN+ جدید اضافه می‌شود
    else if (m_lastCycleIndex == 6) {
        game->upgradeAllBurnsInDeck();

        for (int i = 0; i < 3; ++i) {
            BURN *burn = new BURN();
            burn->upgrade();
            game->addCardToDiscardPile(burn);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

TheChamp::TheChamp(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("The Champ", 420, 420, enemyType::Boss, isMultiplayer, parent)
{
    m_soucePath = ":/enemies/Pics/Enemies/enemy/boss/Champ.png";
    loadPic();
    TheChamp::calculateNextIntent();
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