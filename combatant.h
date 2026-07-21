#ifndef COMBATANT_H
#define COMBATANT_H

#include <QGraphicsObject>
#include <QObject>
#include <QPainter>
#include <QString>
#include <QVector>

#include "buffdebuff.h"

class Combatant;
class GamePlay;
class HealthBarItem;
class BuffItem;

enum class PowerUseTime { StartTurn, EndTurn, OnExhaust };

struct PowerEffect
{
    QString iconPath;
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

    void setCurrentHPDirect(int hp);
    void setMaxHPDirect(int hp);

signals:
    // === Multiplayer ===
    void combatStateChanged();

protected:
    HealthBarItem *m_healthBar;
    bool m_hasBarricade = false;
    QString m_name;
    int m_maxHP;
    int m_currentHP;
    int m_block;
    int m_turnCount;
    QVector<BuffDebuff *> m_activeEffects;
    QVector<PowerEffect> m_powerEffects;

    QVector<BuffItem *> m_buffItems;
    void updateBuffUI();
};

class HealthBarItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit HealthBarItem(QGraphicsItem *parent = nullptr);

    void updateStats(int currentHp, int maxHp, int block);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    int width() const;
    int height() const;

private:
    int m_currentHp = 80;
    int m_maxHp = 80;
    int m_block = 0;

    int m_width = 120;
    int m_height = 15;
};

class BuffItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit BuffItem(BuffDebuffType type, int stacks, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setStacks(int stacks);
    BuffDebuffType Btype() const;

private:
    BuffDebuffType m_type;
    int m_stacks;
    QPixmap m_icon;

    void setupAppearance();
};

#endif // COMBATANT_H
