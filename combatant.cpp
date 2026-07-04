#include "combatant.h"
#include <QtAlgorithms>
#include <QtGlobal>

Combatant::Combatant(QString name, int maxHP, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_name(name)
    , m_maxHP(maxHP)
    , m_currentHP(maxHP)
    , m_block(0)
    , m_turnCount(0)
{}

Combatant::~Combatant()
{
    qDeleteAll(m_activeEffects);
}

int Combatant::takeDamage(int incomingDamage)
{
    int modified = incomingDamage;

    if (effectStacks(BuffDebuffType::Vulnerable) > 0)
        modified = static_cast<int>(modified * 1.5);

    int damageAfterBlock = modified - m_block;
    if (damageAfterBlock < 0)
        damageAfterBlock = 0;

    m_block -= modified;
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

void Combatant::addBlockFromCard(int amount)
{
    int modified = amount + effectStacks(BuffDebuffType::Dexterity);

    if (effectStacks(BuffDebuffType::Frail) > 0)
        modified = static_cast<int>(modified * 0.75);

    if (modified < 0)
        modified = 0;

    addBlock(modified);
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

int Combatant::effectStacks(BuffDebuffType type) const
{
    for (BuffDebuff *effect : m_activeEffects)
        if (effect->type() == type)
            return effect->stacks();

    return 0;
}

QVector<BuffDebuff *> Combatant::getActiveEffects() const
{
    return m_activeEffects;
}

void Combatant::applyBuffDebuff(BuffDebuffType type, int stacks)
{
    for (BuffDebuff *effect : m_activeEffects) {
        if (effect->type() == type) {
            effect->addStacks(stacks);
            return;
        }
    }

    m_activeEffects.append(new BuffDebuff(type, stacks));
}

void Combatant::tickDecayingBuffDebuff()
{
    for (int i = m_activeEffects.size() - 1; i >= 0; --i) {
        m_activeEffects[i]->decayOneStack();

        if (m_activeEffects[i]->isDepleted()) {
            delete m_activeEffects[i];
            m_activeEffects.removeAt(i);
        }
    }
}

int Combatant::calculateOutgoingDamage(int baseDamage) const
{
    int modified = baseDamage + effectStacks(BuffDebuffType::Strength);

    if (effectStacks(BuffDebuffType::Weak) > 0)
        modified = static_cast<int>(modified * 0.75); // floor طبق ویکی

    if (modified < 0)
        modified = 0;

    return modified;
}