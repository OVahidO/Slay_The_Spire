#ifndef ENEMY_H
#define ENEMY_H

#include <QPair>
#include <QString>
#include "combatant.h"

class Player;
class GamePlay;

enum class IntentType {
    Attack,
    Defend,
    Buff,
    Debuff,
    AttackDebuff,
    DefendBuff,
    AttackDefend,
    Unknown
};

struct EnemyIntent
{
    IntentType type = IntentType::Unknown;
    int value = 0;
    int secondaryValue = 0;
    bool isHidden = false;
};

class Enemy : public Combatant
{
    Q_OBJECT
public:
    explicit Enemy(QString name,
                   int minHP,
                   int maxHP,
                   bool isMultiplayer = false,
                   QGraphicsItem *parent = nullptr);
    virtual ~Enemy() = default;

    void executeIntent(Player *player);
    void applyEnemyIntent(GamePlay *game);

    QString getName() const;
    EnemyIntent getCurrentIntent() const;
    virtual void calculateNextIntent() = 0;

protected:
    EnemyIntent pickIntent(const QList<QPair<int, EnemyIntent>> &options) const;

    QString m_name;
    EnemyIntent m_currentIntent;
};
#endif // ENEMY_H
