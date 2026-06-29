#include "enemy.h"

Enemy::Enemy(QString name, int minHP, int maxHP, bool isMultiplayer, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_name(name), m_maxHP(minHP + rand() % (maxHP - minHP + 1)), m_currentHP(m_maxHP), m_block(0), m_turnCount(0) {
    // if (isMultiplayer)
    // {
    //     m_maxHP *= 2;
    //     m_currentHP = m_maxHP;
    // }
}

Enemy::~Enemy() {}

int Enemy::takeDamage(int incomingDamage) {
    int finalDamage = incomingDamage;

    // for (BuffDebuff* effect : m_activeEffects)
    //     if (effect->getName() == "Vulnerable")
    //         finalDamage = qRound(finalDamage * 1.5);

    int damageAfterBlock = finalDamage - m_block;
    if (damageAfterBlock < 0)
        damageAfterBlock = 0;

    m_block -= finalDamage;
    if (m_block < 0)
        m_block = 0;

    m_currentHP -= damageAfterBlock;
    if (m_currentHP < 0)
        m_currentHP = 0;

    return damageAfterBlock;
}

void Enemy::gainBlock(int amount) { m_block += amount; }

void Enemy::resetBlock() { m_block = 0; }

void Enemy::executeIntent(Player* player) {
    Q_UNUSED(player);
    // switch (m_currentIntent.type) {
    // case IntentType::Attack:
    //     player->takeDamage(m_currentIntent.value);
    //     break;
    // case IntentType::Defend:
    //     gainBlock(m_currentIntent.value);
    //     break;
    // ادامشو یه کاری میکنیم زیاد به درد نمیخوره چون با سیگنال اسلاته
    // }
}

bool Enemy::isDead() const { return m_currentHP <= 0; }

QString Enemy::getName() const { return m_name; }

int Enemy::getCurrentHP() const { return m_currentHP; }

int Enemy::getMaxHP() const { return m_maxHP; }

EnemyIntent Enemy::getCurrentIntent() const { return m_currentIntent; }

void Enemy::addEffect(BuffDebuff* effect) { m_activeEffects.append(effect); }

void Enemy::removeEffect(BuffDebuff* effect) { m_activeEffects.removeOne(effect); }

QList<BuffDebuff*> Enemy::getActiveEffects() const { return m_activeEffects; }

//QRectF Enemy::boundingRect() const { return QRectF(0, 0, 150, 150); }

// void Enemy::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
//     Q_UNUSED(painter);
//     Q_UNUSED(option);
//     Q_UNUSED(widget);
// }

EnemyIntent Enemy::pickIntent(const QList<QPair<int, EnemyIntent>>& options) const
{
    int totalWeight = 0;
    for (const auto& option : options)
        totalWeight += option.first;

    int roll = rand() % totalWeight;
    int cumulative = 0;
    for (const auto& option : options) {
        cumulative += option.first;
        if (roll < cumulative)
            return option.second;
    }

    return EnemyIntent();
}
