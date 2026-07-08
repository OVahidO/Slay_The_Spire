#ifndef ENEMY_H
#define ENEMY_H

#include <QPair>
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
    Entangle,
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

    EnemyIntent getCurrentIntent() const;
    virtual void calculateNextIntent() = 0;

protected:
    EnemyIntent pickIntent(const QVector<QPair<int, EnemyIntent>> &options) const;

    EnemyIntent m_currentIntent;

    EnemyIntent attackIntent(int damage) const;
    EnemyIntent defendIntent(int block) const;
    EnemyIntent buffIntent(int value) const;
    EnemyIntent debuffIntent(int value) const;
    EnemyIntent entangleIntent() const;
    EnemyIntent attackDefendIntent(int damage, int block) const;
    EnemyIntent attackBuffIntent(int damage, int value) const;
    EnemyIntent attackDebuffIntent(int damage, int value) const;
    EnemyIntent defendBuffIntent(int block, int value) const;
    EnemyIntent escapeIntent() const;
    EnemyIntent unknownIntent() const;
};
#endif // ENEMY_H
