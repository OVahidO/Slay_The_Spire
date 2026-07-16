#ifndef ENEMY_H
#define ENEMY_H

#include <QPair>
#include <QPixmap>
#include "combatant.h"

class Player;
class GamePlay;
enum class CardType;

enum class enemyType { Normal, Elite, Boss };

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
    QPixmap icon;
};

class Enemy : public Combatant
{
    Q_OBJECT
public:
    explicit Enemy(QString name,
                   int minHP,
                   int maxHP,
                   enemyType type,
                   bool isMultiplayer = false,
                   QGraphicsItem *parent = nullptr);
    virtual ~Enemy() = default;

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void executeIntent(Player *player);
    void applyEnemyIntent(GamePlay *game);

    EnemyIntent getCurrentIntent() const;
    virtual void calculateNextIntent() = 0;

    QPixmap getIntentIcon(IntentType type) const;
    QPixmap picture() const;

    virtual void onAnyCardPlayed(CardType cardType, GamePlay *game) {}

protected:
    EnemyIntent pickIntent(const QVector<QPair<int, EnemyIntent>> &options) const;
    EnemyIntent m_currentIntent;

    QString m_soucePath;
    QPixmap m_enemyPic;
    enemyType m_type;

    void loadPic();

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

    virtual void onIntentExecuted(GamePlay *game) {}
};

/*
int startX = 0;
int startY = 80;

for (BuffDebuff* effect : m_activeEffects) {
    if (effect->stacks() == 0) continue;

    QRectF iconRect(startX, startY, 32, 32); 

    painter->drawPixmap(iconRect.toRect(), effect->icon());

    painter->setPen(Qt::white);
    QFont font("Arial", 10, QFont::Bold);
    painter->setFont(font);
    painter->drawText(iconRect, Qt::AlignBottom | Qt::AlignRight, QString::number(effect->stacks()));

    startX += 36; 
}
*/

#endif // ENEMY_H
