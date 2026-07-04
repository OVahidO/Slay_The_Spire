#ifndef BUFFDEBUFF_H
#define BUFFDEBUFF_H

enum class BuffDebuffType { Strength, Dexterity, Vulnerable, Weak, Frail };

class BuffDebuff
{
public:
    explicit BuffDebuff(BuffDebuffType type, int stacks);

    BuffDebuffType type() const;
    int stacks() const;

    void addStacks(int amount);
    void decayOneStack();
    bool isDepleted() const;

private:
    BuffDebuffType m_type;
    int m_stacks;
};

#endif // BUFFDEBUFF_H
