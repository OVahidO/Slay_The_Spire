#include "enemy.h"
#include "gameplay.h"
#include "player.h"

Enemy::Enemy(QString name, int minHP, int maxHP, bool isMultiplayer, QGraphicsItem *parent)
    : Combatant(name, minHP + rand() % (maxHP - minHP + 1), parent)
{
    if (isMultiplayer) {
        m_maxHP *= 2;
        m_currentHP = m_maxHP;
    }
}

void Enemy::executeIntent(Player *player)
{
    if (!player)
        return;

    switch (m_currentIntent.type) {
    case IntentType::Attack:
        player->takeDamage(m_currentIntent.value);
        break;

    case IntentType::Defend:
        addBlock(m_currentIntent.value);
        break;

    case IntentType::AttackDefend:
        player->takeDamage(m_currentIntent.value);
        addBlock(m_currentIntent.secondaryValue);
        break;

    case IntentType::AttackDebuff:
        player->takeDamage(m_currentIntent.value);
        // player->addEffect(new BuffDebuff("Vulnerable", m_currentIntent.secondaryValue));
        break;

    case IntentType::DefendBuff:
        addBlock(m_currentIntent.value);
        // addEffect(new BuffDebuff("Strength", m_currentIntent.secondaryValue));
        break;

    case IntentType::Buff:
        // addEffect(new BuffDebuff("Strength", m_currentIntent.value));
        break;

    case IntentType::Debuff:
        // player->addEffect(new BuffDebuff("Weak", m_currentIntent.value));
        break;

    case IntentType::Unknown:
    default:
        break;
    }
}

void Enemy::applyEnemyIntent(GamePlay *game)
{
    if (!game)
        return;

    Player *player = game->player();
    executeIntent(player);
    calculateNextIntent();
}

EnemyIntent Enemy::getCurrentIntent() const
{
    return m_currentIntent;
}

EnemyIntent Enemy::pickIntent(const QList<QPair<int, EnemyIntent>> &options) const
{
    int totalWeight = 0;
    for (const auto &option : options)
        totalWeight += option.first;

    if (totalWeight == 0)
        return EnemyIntent();

    int roll = rand() % totalWeight;
    int cumulative = 0;
    for (const auto &option : options) {
        cumulative += option.first;
        if (roll < cumulative)
            return option.second;
    }

    return EnemyIntent();
}