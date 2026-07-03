#ifndef COMBATANT_H
#define COMBATANT_H

#include <QGraphicsObject>
#include <QList>
#include <QObject>

class BuffDebuff;

class Combatant : public QGraphicsObject
{
    Q_OBJECT
public:
    Combatant(QString name, int maxHP, QGraphicsItem *parent = nullptr);
    virtual ~Combatant() = default;

    virtual int takeDamage(int incomingDamage);
    void addBlock(int amount);
    void resetBlock();
    bool isDead() const;

    const QString &name() const;
    int currentHP() const;
    int maxHP() const;
    int block() const;
    int turnCount() const;
    void nextTurn();

    void addEffect(BuffDebuff *effect);
    void removeEffect(BuffDebuff *effect);
    QList<BuffDebuff *> getActiveEffects() const;

protected:
    QString m_name;
    int m_maxHP;
    int m_currentHP;
    int m_block;
    int m_turnCount;
    QList<BuffDebuff *> m_activeEffects;
};

#endif // COMBATANT_H
