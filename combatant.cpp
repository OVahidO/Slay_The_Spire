#include "combatant.h"

Combatant::Combatant(QString name, int maxHP, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_name(name)
    , m_maxHP(maxHP)
    , m_currentHP(maxHP)
    , m_block(0)
    , m_turnCount(0)
{}

int Combatant::takeDamage(int incomingDamage)
{
    int damageAfterBlock = incomingDamage - m_block;
    if (damageAfterBlock < 0)
        damageAfterBlock = 0;

    m_block -= incomingDamage;
    if (m_block < 0)
        m_block = 0;

    m_currentHP -= damageAfterBlock;
    if (m_currentHP < 0)
        m_currentHP = 0;

    return damageAfterBlock;
}

void Combatant::addBlock(int amount)
{
    m_block += amount;
}

void Combatant::resetBlock()
{
    m_block = 0;
}

bool Combatant::isDead() const
{
    return m_currentHP <= 0;
}

const QString &Combatant::name() const
{
    return m_name;
}

int Combatant::currentHP() const
{
    return m_currentHP;
}

int Combatant::maxHP() const
{
    return m_maxHP;
}

int Combatant::block() const
{
    return m_block;
}

int Combatant::turnCount() const
{
    return m_turnCount;
}

void Combatant::nextTurn()
{
    m_turnCount++;
}

void Combatant::addEffect(BuffDebuff *effect)
{
    m_activeEffects.append(effect);
}

void Combatant::removeEffect(BuffDebuff *effect)
{
    m_activeEffects.removeOne(effect);
}

QList<BuffDebuff *> Combatant::getActiveEffects() const
{
    return m_activeEffects;
}