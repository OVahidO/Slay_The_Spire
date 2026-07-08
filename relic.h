#ifndef RELIC_H
#define RELIC_H

#include <QGraphicsObject>

class GamePlay;
class Player;
class Card;

class Relic : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Relic(QGraphicsItem *parent = nullptr);
    virtual ~Relic() = default;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual void onCombatStart(GamePlay *game) {}
    virtual void onCombatEnd(Player *player) {}
    virtual void onTurnStart(Player *player) {}
    virtual void onTurnEnd(Player *player) {}
    virtual void onCardPlayed(Card *card, Player *player) {}
    virtual void onTakeDamage(int &damage, Player *player) {}

    QString name() const;
    QString description() const;
    int counter() const;

    void setCounter(int value);

signals:

protected:
    QString m_name;
    QString m_description;
    QString m_soucePath;
    QPixmap m_icon;
    int m_counter = -1; // like for kunai

    void loadIcon(const QString &path);
};

#endif // RELIC_H
