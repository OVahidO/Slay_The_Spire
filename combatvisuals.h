#ifndef COMBATVISUALS_H
#define COMBATVISUALS_H

#include <QGraphicsObject>
#include <QGraphicsOpacityEffect>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QString>

class TargetFrame : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit TargetFrame(QGraphicsItem *parent = nullptr);

    void setCornerPixmaps(const QString &topLeftPath,
                          const QString &topRightPath,
                          const QString &bottomLeftPath,
                          const QString &bottomRightPath);

    void attachToTarget(QGraphicsItem *target);
    void hideFrame();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QPixmap m_topLeft;
    QPixmap m_topRight;
    QPixmap m_bottomLeft;
    QPixmap m_bottomRight;

    QRectF m_targetRect;
    static constexpr qreal m_cornerSize = 32.0;
};

class FloatingDamageText : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit FloatingDamageText(int amount, bool isHeal = false, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void play();

signals:
    void finished();

private:
    int m_amount;
    bool m_isHeal;
    QGraphicsOpacityEffect *m_opacityEffect;
};

#endif // COMBATVISUALS_H
