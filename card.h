#ifndef CARD_H
#define CARD_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsObject>
#include <QLinearGradient>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QRegularExpression>
#include <QString>
#include <QHash>
#include <QTextDocument>
#include <QVariantAnimation>
#include <functional>
#include "cardIDeas.h"

class Enemy;
class Player;
class GamePlay;

enum class CardType { Attack, Skill, Power, Status, Curse };

enum class CardLifetime { Permanent, EndOfCombat };

class Card : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Card(CardID ID,
                  QString name,
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

    virtual bool isAoeEnemyEffect() const;

    virtual Card *clone() const = 0;

    CardLifetime lifetime() const;
    void setLifetime(CardLifetime lifetime);

    bool isInnate() const;
    void setInnate(bool value);

    virtual QString getDynamicDescription(Player *player = nullptr, Enemy *target = nullptr) const;
    void setHoveredEnemy(Enemy *enemy);
    void loadTypeIcon();

    void setOwnerPlayer(Player *player);

    ///////////////////////////////////

    using CreatorFunc = std::function<Card*()>;

    static QHash<CardID, CreatorFunc>& creators();

    static Card* Creat(CardID cardID);

    ///////////////////////////////////

signals:
    void targetCardPlayed(Card *card, Player *player, Enemy *targetEnemy);
    void noTargetCardPlayed(Card* card);
    void cardEnteredMouse(Card *card);
    void cardLeavedMouse(Card* card);
    void enemyHoverChanged(Enemy *enemy);

protected:
    QPixmap m_cardPixmap;
    QRectF m_targetPixmapRect;
    QPixmap m_energyPixmap;
    void loadPixmap();
    QVariantAnimation* m_hoverAnimation;
    qreal m_oldZValue;

    Enemy *m_hoveredEnemy = nullptr;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    int m_ID;
    QString m_name;
    int m_energyCost;
    QString m_sourcePath;
    QString m_description;
    bool m_isRare;
    bool m_needTarget;
    bool m_isExhaust;
    bool m_isUpgraded = false;

    CardLifetime m_lifetime = CardLifetime::Permanent;

    bool m_isInnate = false;
    QColor colorForCardType(CardType type) const;
    QString highlightKeywords(const QString &text) const;
    void drawTypeGem(QPainter *painter, const QRectF &rect) const;
    CardType m_type;
    QPixmap m_typeIcon;

    int m_baseEnergyCost;

    Player *m_ownerPlayer = nullptr;
};

#endif // CARD_H
