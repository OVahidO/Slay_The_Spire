#ifndef CARD_H
#define CARD_H

#include <QGraphicsObject>
#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QString>
#include <QVariantAnimation>

class Enemy;
class Player;
class GamePlay;

enum class CardType { Attack, Skill, Power, Status, Curse };

class Card : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Card(QString name,
                  CardType type,
                  int energyCost,
                  QString description,
                  bool isRare = false,
                  bool isExhaust = false,
                  bool requiresTarget = true,
                  QGraphicsItem *parent = nullptr);
    virtual ~Card() = default;

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    int ID() const;
    QString name() const;
    int energyCost() const;
    CardType cardType() const;
    QString sourcePath() const;
    QString description() const;
    bool isRare() const;
    bool needTarget() const;
    bool isExhaust() const;

    bool isUpgraded() const;
    virtual void upgrade();

    virtual void applyEffect(Player *player, Enemy *targetEnemy) = 0;
    virtual bool applyEffect(GamePlay *gameplay);

signals:
    void targetCardPlayed(Card* card, Player* player, Enemy* targetEnemy);
    void noTargetCardPlayed(Card* card);
    void cardEnterrdMouse(Card* card);
    void cardLeavedMouse(Card* card);

protected:
    QPixmap m_cardPixmap;
    void loadPixmap();
    QVariantAnimation* m_hoverAnimation;
    qreal m_oldZValue;

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    int m_ID;
    QString m_name;
    int m_energyCost;
    CardType m_type;
    QString m_sourcePath;
    QString m_description;
    bool m_isRare;
    bool m_needTarget;
    bool m_isExhaust;
    bool m_isUpgraded = false;
};

#endif // CARD_H
