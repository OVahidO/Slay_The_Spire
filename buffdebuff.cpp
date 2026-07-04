#include "buffdebuff.h"

BuffDebuff::BuffDebuff(BuffDebuffType type, int stacks)
    : m_type(type)
    , m_stacks(stacks)
{}

BuffDebuffType BuffDebuff::type() const
{
    return m_type;
}

int BuffDebuff::stacks() const
{
    return m_stacks;
}

void BuffDebuff::addStacks(int amount)
{
    m_stacks += amount;
}

void BuffDebuff::decayOneStack()
{
    if (m_type == BuffDebuffType::Vulnerable || m_type == BuffDebuffType::Weak
        || m_type == BuffDebuffType::Frail)
        if (m_stacks > 0)
            m_stacks--;
}

bool BuffDebuff::isDepleted() const
{
    if (m_type == BuffDebuffType::Vulnerable || m_type == BuffDebuffType::Weak
        || m_type == BuffDebuffType::Frail)
        return m_stacks <= 0;

    return false;
}