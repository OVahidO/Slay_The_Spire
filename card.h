#ifndef CARD_H
#define CARD_H

#include <QGraphicsObject>
#include <QObject>
#include <QString>

class Enemy;
class Player;

enum class CardType {Attack, Skill, Power, Status, Curse};

class Card : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Card(QString name, CardType type, int energyCost, QString path, bool isRare = false, bool requiresTarget = true, QGraphicsItem *parent = nullptr);
    virtual ~Card();

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual void applyEffect(Enemy* target, Player* player) = 0;

signals:

protected:
    int m_ID;
    QString m_name;
    int m_energyCost;
    CardType m_type;
    QString m_sourcePath;
    bool m_isRare;
    bool m_needTarget;

};

#endif // CARD_H
