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