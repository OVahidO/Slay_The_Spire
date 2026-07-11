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
    return {IntentType::Attack, damage, 0, false, getIntentIcon(IntentType::Attack)};
}

EnemyIntent Enemy::defendIntent(int block) const
{
    return {IntentType::Defend, block, 0, false, getIntentIcon(IntentType::Defend)};
}

EnemyIntent Enemy::buffIntent(int value) const
{
    return {IntentType::Buff, value, 0, false, getIntentIcon(IntentType::Buff)};
}

EnemyIntent Enemy::debuffIntent(int value) const
{
    return {IntentType::Debuff, value, 0, false, getIntentIcon(IntentType::Debuff)};
}

EnemyIntent Enemy::entangleIntent() const
{
    return {IntentType::Entangle, 0, 0, false, getIntentIcon(IntentType::Entangle)};
}

EnemyIntent Enemy::attackDefendIntent(int damage, int block) const
{
    return {IntentType::AttackDefend, damage, block, false, getIntentIcon(IntentType::AttackDefend)};
}

EnemyIntent Enemy::attackDebuffIntent(int damage, int value) const
{
    return {IntentType::AttackDebuff, damage, value, false, getIntentIcon(IntentType::AttackDebuff)};
}

EnemyIntent Enemy::defendBuffIntent(int block, int value) const
{
    return {IntentType::DefendBuff, block, value, false, getIntentIcon(IntentType::DefendBuff)};
}

EnemyIntent Enemy::escapeIntent() const
{
    return {IntentType::Unknown, 0, 0, true, getIntentIcon(IntentType::Unknown)};
}

EnemyIntent Enemy::unknownIntent() const
{
    return {IntentType::Unknown, 0, 0, false, getIntentIcon(IntentType::Unknown)};
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

QPixmap Enemy::getIntentIcon(IntentType type) const
{
    QString path;

    switch (type) {
    case IntentType::Attack:
        path = ":/buffdebuff-intent/Pics/Enemies/intents/Attacking/StS2_Intent_Attack5.png";
        break;
    case IntentType::Defend:
        path = ":/buffdebuff-intent/Pics/Enemies/intents/Blocking/StS2_Intent_Defend.png";
        break;
    case IntentType::Buff:
        path = ":/buffdebuff-intent/Pics/Enemies/intents/Buffing/StS2_Intent_Buff.png";
        break;
    case IntentType::Debuff:
        path = ":/buffdebuff-intent/Pics/Enemies/intents/Debuffing/StS2_Intent_Debuff.png";
        break;
    case IntentType::AttackDefend:
        path = ":/buffdebuff-intent/Pics/Enemies/intents/Blocking/Intent_DefendAttack7.png";
        break;
    case IntentType::AttackDebuff:
        path = ":/buffdebuff-intent/Pics/Enemies/intents/Debuffing/Intent_DebuffAttack7.png";
        break;
    case IntentType::DefendBuff:
        path = ":/buffdebuff-intent/Pics/Enemies/intents/Buffing/Intent_DefendBuff.png";
        break;
    case IntentType::Entangle:
        path = ":/buffdebuff-intent/Pics/Enemies/intents/Debuffing/Intent_DebuffStrong.png";
        break;
    case IntentType::Unknown:
    default:
        path = ":/buffdebuff-intent/Pics/Enemies/intents/Miscellaneous/Intent_Unknown.png";
        break;
    }

    QPixmap icon;
    icon.load(path);
    return icon;
}