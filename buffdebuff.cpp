#include "buffdebuff.h"

BuffDebuff::BuffDebuff(BuffDebuffType type, int stacks)
    : m_type(type)
    , m_stacks(stacks)
{
    loadIcon();
}

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

void BuffDebuff::loadIcon()
{
    QString path;

    switch (m_type) {
    case BuffDebuffType::Strength:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_Strength.png";
        break;
    case BuffDebuffType::Dexterity:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_Dexterity.png";
        break;
    case BuffDebuffType::Vulnerable:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Debuffs/StS2_Icon_Vulnerable.png";
        break;
    case BuffDebuffType::Weak:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Debuffs/StS2_Icon_Weak.png";
        break;
    case BuffDebuffType::Frail:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Debuffs/StS2_Icon_Frail.png";
        break;
    case BuffDebuffType::Barricade:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/Icon_Barricade.png";
        break;
    case BuffDebuffType::Berserk:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/Icon_Berserk.png";
        break;
    case BuffDebuffType::Brutality:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/Icon_Brutality.png";
        break;
    case BuffDebuffType::Metallicize:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/Icon_Metallicize.png";
        break;
    case BuffDebuffType::DemonForm:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_DemonForm.png";
        break;
    case BuffDebuffType::FeelNoPain:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_FeelNoPain.png";
        break;
    case BuffDebuffType::Flex:
        path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_FlexPotion.png";
        break;
        // case BuffDebuffType::Intangible:
        //     path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_Intangible.png";
        //     break;
        // case BuffDebuffType::Rage:
        //     path = ":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_Rage.png";
        //     break;
    }

    m_icon.load(path);
}