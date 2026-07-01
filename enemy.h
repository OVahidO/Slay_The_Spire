#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsObject>
#include <QObject>
#include <QString>
#include <QList>
#include <QPair>

class Player;

class BuffDebuff;

enum class IntentType { Attack, Defend, Buff, Debuff, AttackDebuff, DefendBuff, AttackDefend, Unknown };

struct EnemyIntent {
    IntentType type = IntentType::Unknown;
    int value = 0;
    int secondaryValue = 0;
    bool isHidden = false;
};

class Enemy : public QGraphicsObject {
    Q_OBJECT
public:
    explicit Enemy(QString name, int minHP, int maxHP, bool isMultiplayer = false, QGraphicsItem *parent = nullptr);
    virtual ~Enemy() = default;

    int takeDamage(int incomingDamage);
    void gainBlock(int amount);
    void resetBlock();
    void executeIntent(Player* player);
    bool isDead() const;

    QString getName() const;
    int getCurrentHP() const;
    int getMaxHP() const;
    EnemyIntent getCurrentIntent() const;

    void addEffect(BuffDebuff* effect);
    void removeEffect(BuffDebuff* effect);
    QList<BuffDebuff*> getActiveEffects() const;

    virtual void calculateNextIntent() = 0;

    // QRectF boundingRect() const override;
    // void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    EnemyIntent pickIntent(const QList<QPair<int, EnemyIntent>>& options) const;

    QString m_name;
    int m_maxHP;
    int m_currentHP;
    int m_block;
    int m_turnCount;
    EnemyIntent m_currentIntent;
    QList<BuffDebuff*> m_activeEffects;
};

#endif // ENEMY_H
