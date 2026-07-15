#ifndef COMBATANT_H
#define COMBATANT_H

#include <QGraphicsObject>
#include <QObject>
#include <QString>
#include <QVector>

#include "buffdebuff.h"

class Combatant;
class GamePlay;

enum class PowerUseTime { StartTurn, EndTurn, OnExhaust };

struct PowerEffect
{
    int value;
    void (*func)(Combatant *self, int value, GamePlay *game);
    PowerUseTime useTime;
};

class Combatant : public QGraphicsObject
{
    Q_OBJECT
public:
    Combatant(QString name, int maxHP, QGraphicsItem *parent = nullptr);
    virtual ~Combatant();

    virtual int takeDamage(int incomingDamage, bool isAttackDamage = true);
    void setBlock(int amount);
    void addBlock(int amount);
    void addBlockFromCard(int amount);
    void resetBlock();
    bool isDead() const;

    const QString &name() const;
    int currentHP() const;
    int maxHP() const;
    int block() const;
    int turnCount() const;
    void nextTurn();

    void setBarricade(bool status);
    bool hasBarricade() const;

    int effectStacks(BuffDebuffType type) const;
    QVector<BuffDebuff *> getActiveEffects() const;
    void applyBuffDebuff(BuffDebuffType type, int stacks);
    void tickDecayingBuffDebuff();

    int calculateOutgoingDamage(int baseDamage) const;
    int calculateBlock(int baseAmount) const;

    QVector<PowerEffect> &powerEffects();
    void triggerPowerEffects(PowerUseTime time, GamePlay *game = nullptr);

    void overrideHP(int hp);

protected:
    bool m_hasBarricade = false;
    QString m_name;
    int m_maxHP;
    int m_currentHP;
    int m_block;
    int m_turnCount;
    QVector<BuffDebuff *> m_activeEffects;
    QVector<PowerEffect> m_powerEffects;
};

#endif // COMBATANT_H
