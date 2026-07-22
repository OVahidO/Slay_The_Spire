#include "enemy.h"
#include "card.h"
#include "gameplay.h"
#include "player.h"

#include <QRandomGenerator>

std::mt19937 *Enemy::s_activeRng = nullptr;

Enemy::Enemy(
    QString name, int minHP, int maxHP, enemyType type, bool isMultiplayer, QGraphicsItem *parent)
    : Combatant(name, rollHP(minHP, maxHP), parent) // FIX: rand() مستقیم حذف شد
    , m_type(type)
{
    if (isMultiplayer) {
        m_maxHP *= 2;
        m_currentHP = m_maxHP;
    }
}

//temperory
QRectF Enemy::boundingRect() const
{
    // if (!m_enemyPic.isNull())
    //     return QRectF(0, 0, m_enemyPic.width(), m_enemyPic.height());

    return QRectF(0, 0, 110, 100);
}

void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    QRectF rect = boundingRect();

    if (!m_enemyPic.isNull()) {
        painter->drawPixmap(rect, m_enemyPic, m_enemyPic.rect());
    } else {
        painter->setBrush(QColor(80, 20, 20));
        painter->setPen(QPen(Qt::white, 2));
        painter->drawRoundedRect(rect, 8, 8);
        painter->setPen(Qt::white);
        painter->drawText(rect, Qt::AlignCenter, m_name);
    }

    // نوار HP
    // qreal barHeight = 10;
    // QRectF hpBg(0, rect.height() + 5, rect.width(), barHeight);
    // painter->setBrush(QColor(40, 40, 40));
    // painter->setPen(Qt::NoPen);
    // painter->drawRect(hpBg);

    // qreal hpRatio = m_maxHP > 0 ? static_cast<qreal>(m_currentHP) / m_maxHP : 0;
    // QRectF hpFill(0, rect.height() + 5, rect.width() * hpRatio, barHeight);
    // painter->setBrush(QColor(200, 40, 40));
    // painter->drawRect(hpFill);

    // painter->setPen(Qt::white);
    // QFont font("Arial", 9, QFont::Bold);
    // painter->setFont(font);
    // painter->drawText(hpBg, Qt::AlignCenter, QString("%1/%2").arg(m_currentHP).arg(m_maxHP));

    // آیکون Intent فعلی بالای دشمن
    if (!m_currentIntent.icon.isNull()) {
        QRectF intentRect(rect.width() / 2 - 20, -55, 40, 40);
        painter->drawPixmap(intentRect, m_currentIntent.icon, m_currentIntent.icon.rect());

        if (m_currentIntent.type == IntentType::Attack
            || m_currentIntent.type == IntentType::AttackDefend
            || m_currentIntent.type == IntentType::AttackDebuff) {
            painter->setPen(Qt::white);
            QFont valFont("Arial", 11, QFont::Bold);
            painter->setFont(valFont);
            painter->drawText(QRectF(rect.width() / 2 - 20, -18, 40, 20),
                              Qt::AlignCenter,
                              QString::number(m_currentIntent.value));
        }
    }
}
//

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

// void Enemy::executeIntent(Player *player)
// {
//     if (!player)
//         return;

//     switch (m_currentIntent.type) {
//     case IntentType::Attack: {
//         int hits = m_currentIntent.secondaryValue > 0 ? m_currentIntent.secondaryValue : 1;

//         for (int i = 0; i < hits; ++i)
//             player->takeDamage(calculateOutgoingDamage(m_currentIntent.value));

//         break;
//     }

//     case IntentType::Defend:
//         addBlock(m_currentIntent.value);
//         break;

//     case IntentType::AttackDefend:
//         player->takeDamage(calculateOutgoingDamage(m_currentIntent.value));
//         addBlock(m_currentIntent.secondaryValue);
//         break;

//     case IntentType::AttackDebuff:
//         player->takeDamage(calculateOutgoingDamage(m_currentIntent.value));
//         player->applyBuffDebuff(BuffDebuffType::Vulnerable, m_currentIntent.secondaryValue);
//         break;

//     case IntentType::DefendBuff:
//         addBlock(m_currentIntent.value);
//         applyBuffDebuff(BuffDebuffType::Strength, m_currentIntent.secondaryValue);
//         break;

//     case IntentType::Buff:
//         applyBuffDebuff(BuffDebuffType::Strength, m_currentIntent.value);
//         break;

//     case IntentType::Debuff:
//         player->applyBuffDebuff(BuffDebuffType::Weak, m_currentIntent.value);
//         break;

//     case IntentType::Entangle:
//         player->setCannotPlayAttacks(true);
//         break;

//     case IntentType::Unknown:
//     default:
//         break;
//     }
// }

Player *Enemy::chooseSingleTarget(GamePlay *game) const
{
    if (!game)
        return nullptr;

    QVector<Player *> candidates;
    for (Player *p : game->allPlayers())
        if (p && p->currentHP() > 0)
            candidates.append(p);

    if (candidates.isEmpty())
        return game->player(); // fallback ایمن

    if (candidates.size() == 1)
        return candidates.first();

    int index = QRandomGenerator::global()->bounded(candidates.size());
    return candidates.at(index);
}

void Enemy::executeIntent(GamePlay *game)
{
    if (!game)
        return;

    Player *target = chooseSingleTarget(game);
    if (!target)
        return;

    switch (m_currentIntent.type) {
    case IntentType::Attack: {
        int hits = m_currentIntent.secondaryValue > 0 ? m_currentIntent.secondaryValue : 1;
        for (int i = 0; i < hits; ++i)
            target->takeDamage(calculateOutgoingDamage(m_currentIntent.value));
        emit attacked(this);

        break;
    }

    case IntentType::Defend:
        addBlock(m_currentIntent.value);
        break;

    case IntentType::AttackDefend:
        target->takeDamage(calculateOutgoingDamage(m_currentIntent.value));
        addBlock(m_currentIntent.secondaryValue);

        emit attacked(this);

        break;

    case IntentType::AttackDebuff:

        target->takeDamage(calculateOutgoingDamage(m_currentIntent.value));
        target->applyBuffDebuff(BuffDebuffType::Vulnerable, m_currentIntent.secondaryValue);
        emit attacked(this);

        break;

    case IntentType::DefendBuff:
        addBlock(m_currentIntent.value);
        applyBuffDebuff(BuffDebuffType::Strength, m_currentIntent.secondaryValue);
        break;

    case IntentType::Buff:
        applyBuffDebuff(BuffDebuffType::Strength, m_currentIntent.value);
        break;

    case IntentType::Debuff:
        for (Player *p : game->allPlayers())
            if (p && p->currentHP() > 0)
                p->applyBuffDebuff(BuffDebuffType::Weak, m_currentIntent.value);
        break;

    case IntentType::Entangle:
        target->setCannotPlayAttacks(true);
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
    // executeIntent(player);
    executeIntent(game);
    onIntentExecuted(game);
    calculateNextIntent();
    triggerPowerEffects(PowerUseTime::EndTurn, game);
    update();
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

    int roll = rollBounded(totalWeight);
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

void Enemy::setActiveRng(std::mt19937 *rng)
{
    s_activeRng = rng;
}

int Enemy::rollHP(int minHP, int maxHP)
{
    if (maxHP <= minHP)
        return minHP;

    if (s_activeRng)
        return minHP
               + static_cast<int>((*s_activeRng)() % static_cast<unsigned int>(maxHP - minHP + 1));

    return minHP + rand() % (maxHP - minHP + 1);
}

int Enemy::rollBounded(int exclusiveMax)
{
    if (exclusiveMax <= 1)
        return 0;

    if (s_activeRng)
        return static_cast<int>((*s_activeRng)() % static_cast<unsigned int>(exclusiveMax));

    return rand() % exclusiveMax;
}

void Enemy::previewNextIntent()
{
    calculateNextIntent();
}

int Enemy::networkEntityId() const
{
    return m_networkEntityId;
}

void Enemy::setNetworkEntityId(int id)
{
    m_networkEntityId = id;
}