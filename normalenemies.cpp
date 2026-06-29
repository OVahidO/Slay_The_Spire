#include "normalenemies.h"

Cultist::Cultist(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Cultist", 48, 54, isMultiplayer, parent) {
    calculateNextIntent();
}

void Cultist::calculateNextIntent() {
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = EnemyIntent{ IntentType::Buff, 3, false };
        m_hasIncantation = true;
    }
    else
        m_currentIntent = EnemyIntent{ IntentType::Attack, 6, false };

    // if (m_hasIncantation && m_turnCount > 1) {
    //     BuffDebuff* strength = new BuffDebuff("Strength", 3);
    //     addEffect(strength);
    // }
}

JawWorm::JawWorm(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Jaw Worm", 42, 46, isMultiplayer, parent) {
    calculateNextIntent();
}

void JawWorm::calculateNextIntent() {
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = EnemyIntent{ IntentType::Attack, 11, 0, false };
        return;
    }

    QList<QPair<int, EnemyIntent>> options = {
        { 25, EnemyIntent{ IntentType::Attack, 11, 0, false } },
        { 30, EnemyIntent{ IntentType::AttackDefend, 7, 5, false } },
        { 45, EnemyIntent{ IntentType::DefendBuff, 6, 3, false } }
    };

    m_currentIntent = pickIntent(options);
}


Louse::Louse(bool isMultiplayer, QGraphicsItem* parent)
    : Enemy(
          (rand() % 2 == 0) ? "Green Louse" : "Red Louse",
          10,
          15,
          isMultiplayer,
          parent) {
    m_isRed = (m_name == "Red Louse");

    calculateNextIntent();
}

int Louse::randomBiteDamage() const {
    return 5 + rand() % 3;
}

void Louse::calculateNextIntent() {
    m_turnCount++;

    QList<QPair<int, EnemyIntent>> options =
         { { 75, EnemyIntent{ IntentType::Attack, randomBiteDamage(), 0, false } },
         { 25, EnemyIntent{ IntentType::Buff, 3, 0, false } } };

    m_currentIntent = pickIntent(options);
}

int Louse::takeDamage(int incomingDamage) {
    int damage = Enemy::takeDamage(incomingDamage);

    if (damage > 0 && !m_defensiveReactionUsed) {
        m_defensiveReactionUsed = true;

        gainBlock(3 + rand() % 5);
    }

    return damage;
}

Slime::Slime(QString name, int minHP, int maxHP, bool isMultiplayer, QGraphicsItem* parent)
    : Enemy(name, minHP, maxHP, isMultiplayer, parent) {}

bool Slime::shouldSplit() const {
    return false;
}

AcidSlimeS::AcidSlimeS(bool isMultiplayer, QGraphicsItem* parent)
    : Slime("Small Slime",
          8,
          12,
          isMultiplayer,
          parent
          ) {
    calculateNextIntent();
}

void AcidSlimeS::calculateNextIntent() {
    m_turnCount++;

    QList<QPair<int, EnemyIntent>> options =
        {
            { 50, EnemyIntent{ IntentType::Attack, 3, 0, false } },
            { 50, EnemyIntent{ IntentType::Debuff, 1, 0, false } }
        };

    m_currentIntent = pickIntent(options);
}

AcidSlimeM::AcidSlimeM(bool isMultiplayer, QGraphicsItem* parent)
    : Slime(
          "Medium Slime",
          28,
          32,
          isMultiplayer,
          parent
          ) {
    calculateNextIntent();
}

void AcidSlimeM::calculateNextIntent()
{
    m_turnCount++;

    QList<QPair<int, EnemyIntent>> options =
        {
            { 30, EnemyIntent{ IntentType::AttackDebuff, 7, 1, false } },
            { 40, EnemyIntent{ IntentType::Attack, 10, 0, false } },
            { 30, EnemyIntent{ IntentType::Debuff, 1, 0, false } }
        };

    m_currentIntent = pickIntent(options);
}

AcidSlimeL::AcidSlimeL(bool isMultiplayer, QGraphicsItem* parent)
    : Slime(
          "Large Slime",
          68,
          72,
          isMultiplayer,
          parent
          ) {
    calculateNextIntent();
}

void AcidSlimeL::calculateNextIntent()
{
    m_turnCount++;

    if (!m_hasSplit && m_currentHP <= m_maxHP / 2) {
        m_currentIntent = { IntentType::Unknown, 0, 0, false };

        return;
    }

    QList<QPair<int, EnemyIntent>> options =
        {
            { 30, EnemyIntent{ IntentType::AttackDebuff, 7, 1, false } },
            { 40, EnemyIntent{ IntentType::Attack, 10, 0, false } },
            { 30, EnemyIntent{ IntentType::Debuff, 1, 0, false } }
        };

    m_currentIntent = pickIntent(options);
}

bool AcidSlimeL::shouldSplit() const {
    return !m_hasSplit && m_currentHP <= m_maxHP / 2;
}