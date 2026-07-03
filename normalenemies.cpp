#include "normalenemies.h"

Cultist::Cultist(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Cultist", 48, 54, isMultiplayer, parent)
{
    calculateNextIntent();
}

void Cultist::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = EnemyIntent{IntentType::Buff, 3, false};
        m_hasIncantation = true;
    } else
        m_currentIntent = EnemyIntent{IntentType::Attack, 6, false};

    // if (m_hasIncantation && m_turnCount > 1) {
    //     BuffDebuff* strength = new BuffDebuff("Strength", 3);
    //     addEffect(strength);
    // }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

JawWorm::JawWorm(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Jaw Worm", 42, 46, isMultiplayer, parent)
{
    calculateNextIntent();
}

void JawWorm::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = EnemyIntent{IntentType::Attack, 11, 0, false};
        return;
    }

    QList<QPair<int, EnemyIntent>> options = {{25, EnemyIntent{IntentType::Attack, 11, 0, false}},
                                              {30,
                                               EnemyIntent{IntentType::AttackDefend, 7, 5, false}},
                                              {45,
                                               EnemyIntent{IntentType::DefendBuff, 6, 3, false}}};

    m_currentIntent = pickIntent(options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Louse::Louse(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy((rand() % 2 == 0) ? "Green Louse" : "Red Louse", 10, 15, isMultiplayer, parent)
{
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

    QList<QPair<int, EnemyIntent>> options
        = {{75, EnemyIntent{IntentType::Attack, randomBiteDamage(), 0, false}},
           {25, EnemyIntent{IntentType::Buff, 3, 0, false}}};

    m_currentIntent = pickIntent(options);
}

int Louse::takeDamage(int incomingDamage)
{
    int damage = Enemy::takeDamage(incomingDamage);

    if (damage > 0 && !m_defensiveReactionUsed) {
        m_defensiveReactionUsed = true;

        gainBlock(3 + rand() % 5);
    }

    return damage;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Slime::Slime(QString name, int minHP, int maxHP, bool isMultiplayer, QGraphicsItem *parent)
    : Enemy(name, minHP, maxHP, isMultiplayer, parent)
{}

bool Slime::shouldSplit() const
{
    return false;
}

AcidSlimeS::AcidSlimeS(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("Small Slime", 8, 12, isMultiplayer, parent)
{
    calculateNextIntent();
}

void AcidSlimeS::calculateNextIntent()
{
    m_turnCount++;

    QList<QPair<int, EnemyIntent>> options = {{50, EnemyIntent{IntentType::Attack, 3, 0, false}},
                                              {50, EnemyIntent{IntentType::Debuff, 1, 0, false}}};

    m_currentIntent = pickIntent(options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

AcidSlimeM::AcidSlimeM(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("Medium Slime", 28, 32, isMultiplayer, parent)
{
    calculateNextIntent();
}

void AcidSlimeM::calculateNextIntent()
{
    m_turnCount++;

    QList<QPair<int, EnemyIntent>> options = {{30,
                                               EnemyIntent{IntentType::AttackDebuff, 7, 1, false}},
                                              {40, EnemyIntent{IntentType::Attack, 10, 0, false}},
                                              {30, EnemyIntent{IntentType::Debuff, 1, 0, false}}};

    m_currentIntent = pickIntent(options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

AcidSlimeL::AcidSlimeL(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("Large Slime", 68, 72, isMultiplayer, parent)
{
    calculateNextIntent();
}

void AcidSlimeL::calculateNextIntent()
{
    m_turnCount++;

    if (!m_hasSplit && m_currentHP <= m_maxHP / 2) {
        m_currentIntent = {IntentType::Unknown, 0, 0, false};

        return;
    }

    QList<QPair<int, EnemyIntent>> options = {{30,
                                               EnemyIntent{IntentType::AttackDebuff, 7, 1, false}},
                                              {40, EnemyIntent{IntentType::Attack, 10, 0, false}},
                                              {30, EnemyIntent{IntentType::Debuff, 1, 0, false}}};

    m_currentIntent = pickIntent(options);
}

bool AcidSlimeL::shouldSplit() const
{
    return !m_hasSplit && m_currentHP <= m_maxHP / 2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Thief::Thief(
    QString name, int minHP, int maxHP, int mugDamage, bool isMultiplayer, QGraphicsItem *parent)
    : Enemy(name, minHP, maxHP, isMultiplayer, parent)
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
        m_currentIntent = {IntentType::AttackDebuff, m_mugDamage, 15, false};

        return;
    }

    if (m_turnCount == 3) {
        m_currentIntent = {IntentType::Defend, 6, 0, false};

        return;
    }

    m_currentIntent = {IntentType::Unknown, 0, 0, true};

    m_hasEscaped = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Looter::Looter(bool isMultiplayer, QGraphicsItem *parent)
    : Thief("Looter", 44, 48, 10, isMultiplayer, parent)
{}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Mugger::Mugger(bool isMultiplayer, QGraphicsItem *parent)
    : Thief("Mugger", 52, 56, 14, isMultiplayer, parent)
{}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

BlueSlaver::BlueSlaver(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Blue Slaver", 46, 50, isMultiplayer, parent)
{
    calculateNextIntent();
}

void BlueSlaver::calculateNextIntent()
{
    m_turnCount++;

    QList<QPair<int, EnemyIntent>> options = {{60, EnemyIntent{IntentType::Attack, 12, 0, false}},
                                              {40,
                                               EnemyIntent{IntentType::AttackDebuff, 7, 1, false}}};

    m_currentIntent = pickIntent(options);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

RedSlaver::RedSlaver(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Red Slaver", 46, 50, isMultiplayer, parent)
{
    calculateNextIntent();
}

void RedSlaver::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = {IntentType::Attack, 13, 0, false};

        return;
    }

    QList<QPair<int, EnemyIntent>> options;

    if (!m_usedEntangle)
        options.append({25, EnemyIntent{IntentType::Debuff, 1, 0, false}});

    options.append({50, EnemyIntent{IntentType::Attack, 13, 0, false}});

    options.append({50, EnemyIntent{IntentType::AttackDebuff, 8, 1, false}});

    m_currentIntent = pickIntent(options);

    if (m_currentIntent.type == IntentType::Debuff)
        m_usedEntangle = true;
}

SphericGuardian::SphericGuardian(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Spheric Guardian", 20, 20, isMultiplayer, parent)
{
    gainBlock(25);
    calculateNextIntent();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void SphericGuardian::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = EnemyIntent{IntentType::AttackDebuff, 10, 5, false};
        return;
    }

    QList<QPair<int, EnemyIntent>> options = {{50,
                                               EnemyIntent{IntentType::AttackDefend, 10, 15, false}},
                                              {50, EnemyIntent{IntentType::Attack, 10, 2, false}}};

    m_currentIntent = pickIntent(options);
}