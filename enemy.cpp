#include "enemy.h"
#include "gameplay.h"
#include "player.h"

Enemy::Enemy(
    QString name, int minHP, int maxHP, enemyType type, bool isMultiplayer, QGraphicsItem *parent)
    : Combatant(name, minHP + rand() % (maxHP - minHP + 1), parent)
    , m_type(type)
{
    if (isMultiplayer) {
        m_maxHP *= 2;
        m_currentHP = m_maxHP;
    }
}

EnemyIntent Enemy::attackIntent(int damage) const
{
    return {IntentType::Attack, damage, 0, false};
}

EnemyIntent Enemy::defendIntent(int block) const
{
    return {IntentType::Defend, block, 0, false};
}

EnemyIntent Enemy::buffIntent(int value) const
{
    return {IntentType::Buff, value, 0, false};
}

EnemyIntent Enemy::debuffIntent(int value) const
{
    return {IntentType::Debuff, value, 0, false};
}

EnemyIntent Enemy::entangleIntent() const
{
    return {IntentType::Entangle, 0, 0, false};
}

EnemyIntent Enemy::attackDefendIntent(int damage, int block) const
{
    return {IntentType::AttackDefend, damage, block, false};
}

EnemyIntent Enemy::attackDebuffIntent(int damage, int value) const
{
    return {IntentType::AttackDebuff, damage, value, false};
}

EnemyIntent Enemy::defendBuffIntent(int block, int value) const
{
    return {IntentType::DefendBuff, block, value, false};
}

EnemyIntent Enemy::escapeIntent() const
{
    return {IntentType::Unknown, 0, 0, true};
}

EnemyIntent Enemy::unknownIntent() const
{
    return {IntentType::Unknown, 0, 0, false};
}

void Enemy::executeIntent(Player *player)
{
    if (!player)
        return;

    switch (m_currentIntent.type) {
    case IntentType::Attack: {
        int hits = m_currentIntent.secondaryValue > 0 ? m_currentIntent.secondaryValue : 1;

        for (int i = 0; i < hits; ++i)
            player->takeDamage(calculateOutgoingDamage(m_currentIntent.value));

        break;
    }

    case IntentType::Defend:
        addBlock(m_currentIntent.value);
        break;

    case IntentType::AttackDefend:
        player->takeDamage(calculateOutgoingDamage(m_currentIntent.value));
        addBlock(m_currentIntent.secondaryValue);
        break;

    case IntentType::AttackDebuff:
        player->takeDamage(calculateOutgoingDamage(m_currentIntent.value));
        player->applyBuffDebuff(BuffDebuffType::Vulnerable, m_currentIntent.secondaryValue);
        break;

    case IntentType::DefendBuff:
        addBlock(m_currentIntent.value);
        applyBuffDebuff(BuffDebuffType::Strength, m_currentIntent.secondaryValue);
        break;

    case IntentType::Buff:
        applyBuffDebuff(BuffDebuffType::Strength, m_currentIntent.value);
        break;

    case IntentType::Debuff:
        player->applyBuffDebuff(BuffDebuffType::Weak, m_currentIntent.value);
        break;

    case IntentType::Entangle:
        player->setCannotPlayAttacks(true);
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

    triggerPowerEffects(PowerUseTime::StartTurn, game);
    executeIntent(player);
    calculateNextIntent();
    triggerPowerEffects(PowerUseTime::EndTurn, game);
}

EnemyIntent Enemy::getCurrentIntent() const
{
    return m_currentIntent;
}

EnemyIntent Enemy::pickIntent(const QVector<QPair<int, EnemyIntent>> &options) const
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

void Enemy::loadPic()
{
    m_enemyPic.load(m_soucePath);
}