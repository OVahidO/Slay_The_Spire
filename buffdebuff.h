#ifndef BUFFDEBUFF_H
#define BUFFDEBUFF_H

#include <QPixmap>

enum class BuffDebuffType {
    Strength,
    Dexterity,
    Vulnerable,
    Weak,
    Frail,
    Metallicize,
    FeelNoPain,
    Barricade,
    DemonForm,
    Flex,
    Intangible,
    Rage,
    Brutality,
    Berserk
};

class BuffDebuff
{
public:
    explicit BuffDebuff(BuffDebuffType type, int stacks);

    BuffDebuffType type() const;
    int stacks() const;

    void addStacks(int amount);
    void decayOneStack();
    bool isDepleted() const;

    QPixmap icon() const;

private:
    QPixmap m_icon;
    BuffDebuffType m_type;
    int m_stacks;

    void loadIcon();
};

#endif // BUFFDEBUFF_H
