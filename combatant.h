#ifndef COMBATANT_H
#define COMBATANT_H

#include <QGraphicsObject>
#include <QObject>
#include <QString>
#include <QVector>

#include "buffdebuff.h"

class Combatant : public QGraphicsObject
{
    Q_OBJECT
public:
    Combatant(QString name, int maxHP, QGraphicsItem *parent = nullptr);
    virtual ~Combatant();

    virtual int takeDamage(int incomingDamage);
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

    int effectStacks(BuffDebuffType type) const;
    QVector<BuffDebuff *> getActiveEffects() const;
    void applyBuffDebuff(BuffDebuffType type, int stacks);
    void tickDecayingBuffDebuff();

    int calculateOutgoingDamage(int baseDamage) const;

protected:
    QString m_name;
    int m_maxHP;
    int m_currentHP;
    int m_block;
    int m_turnCount;
    QVector<BuffDebuff *> m_activeEffects;
};

#endif // COMBATANT_H
