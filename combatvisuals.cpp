#include "combatvisuals.h"

#include <QElapsedTimer>
#include <QFont>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QSequentialAnimationGroup>
#include <QTimer>

// =============================== TargetFrame ===============================

TargetFrame::TargetFrame(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setZValue(500);
    hide();
}

void TargetFrame::setCornerPixmaps(const QString &topLeftPath,
                                   const QString &topRightPath,
                                   const QString &bottomLeftPath,
                                   const QString &bottomRightPath)
{
    m_topLeft.load(topLeftPath);
    m_topRight.load(topRightPath);
    m_bottomLeft.load(bottomLeftPath);
    m_bottomRight.load(bottomRightPath);
}

void TargetFrame::attachToTarget(QGraphicsItem *target)
{
    if (!target)
        return;

    QRectF sceneRect = target->sceneBoundingRect();

    prepareGeometryChange();
    setPos(sceneRect.topLeft());
    m_targetRect = QRectF(0, 0, sceneRect.width(), sceneRect.height());

    show();
    update();
}

void TargetFrame::hideFrame()
{
    hide();
}

QRectF TargetFrame::boundingRect() const
{
    return m_targetRect.adjusted(-m_cornerSize, -m_cornerSize, m_cornerSize, m_cornerSize);
}

void TargetFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (m_targetRect.isEmpty())
        return;

    painter->setRenderHint(QPainter::Antialiasing);

    QRectF topLeftRect(m_targetRect.left() - m_cornerSize / 2,
                       m_targetRect.top() - m_cornerSize / 2,
                       m_cornerSize,
                       m_cornerSize);
    QRectF topRightRect(m_targetRect.right() - m_cornerSize / 2,
                        m_targetRect.top() - m_cornerSize / 2,
                        m_cornerSize,
                        m_cornerSize);
    QRectF bottomLeftRect(m_targetRect.left() - m_cornerSize / 2,
                          m_targetRect.bottom() - m_cornerSize / 2,
                          m_cornerSize,
                          m_cornerSize);
    QRectF bottomRightRect(m_targetRect.right() - m_cornerSize / 2,
                           m_targetRect.bottom() - m_cornerSize / 2,
                           m_cornerSize,
                           m_cornerSize);

    auto drawCorner = [&](const QPixmap &pix, const QRectF &rect) {
        if (!pix.isNull()) {
            painter->drawPixmap(rect, pix, pix.rect());
        } else {
            // Fallback موقت تا وقتی عکس‌های واقعی گوشه ست نشده‌اند
            painter->setPen(QPen(QColor(255, 60, 60), 3));
            painter->drawLine(rect.topLeft(), rect.topLeft() + QPointF(rect.width() * 0.6, 0));
            painter->drawLine(rect.topLeft(), rect.topLeft() + QPointF(0, rect.height() * 0.6));
        }
    };

    drawCorner(m_topLeft, topLeftRect);
    drawCorner(m_topRight, topRightRect);
    drawCorner(m_bottomLeft, bottomLeftRect);
    drawCorner(m_bottomRight, bottomRightRect);
}

// =========================== FloatingDamageText =============================

FloatingDamageText::FloatingDamageText(int amount, bool isHeal, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_amount(amount)
    , m_isHeal(isHeal)
    , m_opacityEffect(nullptr)
{}

QRectF FloatingDamageText::boundingRect() const
{
    return QRectF(-45, -25, 90, 50);
}

void FloatingDamageText::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    QFont font("Arial", 22, QFont::Black);
    painter->setFont(font);

    QColor textColor = m_isHeal ? QColor(90, 230, 110) : QColor(230, 40, 40);
    QString text = (m_isHeal ? QString("+") : QString("-")) + QString::number(qAbs(m_amount));

    // سایه برای خوانایی بهتر روی هر پس‌زمینه‌ای
    painter->setPen(QColor(0, 0, 0, 200));
    painter->drawText(boundingRect().translated(2, 2), Qt::AlignCenter, text);

    painter->setPen(textColor);
    painter->drawText(boundingRect(), Qt::AlignCenter, text);
}

void FloatingDamageText::play()
{
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(m_opacityEffect);
    m_opacityEffect->setOpacity(1.0);

    QPointF startPos = pos();
    QPointF endPos = startPos - QPointF(0, 70);

    QPropertyAnimation *moveAnim = new QPropertyAnimation(this, "pos");
    moveAnim->setDuration(900);
    moveAnim->setStartValue(startPos);
    moveAnim->setEndValue(endPos);
    moveAnim->setEasingCurve(QEasingCurve::OutCubic);

    QPropertyAnimation *fadeAnim = new QPropertyAnimation(m_opacityEffect, "opacity");
    fadeAnim->setDuration(900);
    fadeAnim->setKeyValueAt(0.0, 1.0);
    fadeAnim->setKeyValueAt(0.6, 1.0);
    fadeAnim->setKeyValueAt(1.0, 0.0);

    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    group->addAnimation(moveAnim);
    group->addAnimation(fadeAnim);

    connect(group, &QParallelAnimationGroup::finished, this, [this, group]() {
        group->deleteLater();
        emit finished();
    });

    group->start();
}

// =========================================================================
// انیمیشن حمله (Jolt)
// =========================================================================
void triggerAttackJolt(QGraphicsObject *attacker, qreal offsetX, qreal offsetY, int duration)
{
    if (!attacker)
        return;

    const QPointF originalPos = attacker->pos();
    const QPointF joltPos = originalPos + QPointF(offsetX, offsetY);

    auto *group = new QSequentialAnimationGroup(attacker);

    auto *forward = new QPropertyAnimation(attacker, "pos");
    forward->setDuration(duration);
    forward->setStartValue(originalPos);
    forward->setEndValue(joltPos);
    forward->setEasingCurve(QEasingCurve::OutQuad);

    auto *backward = new QPropertyAnimation(attacker, "pos");
    backward->setDuration(duration);
    backward->setStartValue(joltPos);
    backward->setEndValue(originalPos);
    backward->setEasingCurve(QEasingCurve::InQuad);

    group->addAnimation(forward);
    group->addAnimation(backward);

    QObject::connect(group, &QSequentialAnimationGroup::finished, group, &QObject::deleteLater);

    group->start();
}

// =========================================================================
// متن شناور دمیج
// =========================================================================
void showFloatingDamageText(
    QGraphicsScene *scene, QPointF scenePos, int amount, bool isHeal, bool isBlocked)
{
    if (!scene || amount <= 0)
        return;

    QString displayText = isBlocked
                              ? "BLOCKED"
                              : (isHeal ? QString("+%1").arg(amount) : QString::number(amount));

    auto *text = new QGraphicsTextItem(displayText);

    QFont font("Arial", isBlocked ? 15 : 26, QFont::Black);
    text->setFont(font);

    QColor color = isHeal      ? QColor(90, 230, 110)
                   : isBlocked ? QColor(150, 200, 255)
                               : QColor(230, 40, 40);
    text->setDefaultTextColor(color);

    QRectF textRect = text->boundingRect();
    text->setPos(scenePos.x() - textRect.width() / 2, scenePos.y() - textRect.height() / 2);
    text->setZValue(2000);

    scene->addItem(text);

    const QPointF startPos = text->pos();
    const QPointF endPos = startPos + QPointF(0, -70);

    auto *posAnim = new QPropertyAnimation(text, "pos");
    posAnim->setDuration(800);
    posAnim->setStartValue(startPos);
    posAnim->setEndValue(endPos);
    posAnim->setEasingCurve(QEasingCurve::OutCubic);

    auto *opacityAnim = new QPropertyAnimation(text, "opacity");
    opacityAnim->setDuration(800);
    opacityAnim->setStartValue(1.0);
    opacityAnim->setKeyValueAt(0.6, 1.0);
    opacityAnim->setEndValue(0.0);
    opacityAnim->setEasingCurve(QEasingCurve::InQuad);

    auto *group = new QParallelAnimationGroup(scene);
    group->addAnimation(posAnim);
    group->addAnimation(opacityAnim);

    QObject::connect(group, &QParallelAnimationGroup::finished, group, [group, text, scene]() {
        scene->removeItem(text);
        delete text;
        group->deleteLater();
    });

    group->start();
}

// =========================================================================
// لرزش صفحه - روی QGraphicsView (نه Scene)
// =========================================================================
void triggerScreenShake(QGraphicsView *view, int intensity, int durationMs)
{
    if (!view)
        return;

    auto *shakeTimer = new QTimer(view);
    auto *elapsed = new QElapsedTimer();
    elapsed->start();

    QObject::connect(shakeTimer,
                     &QTimer::timeout,
                     view,
                     [view, shakeTimer, elapsed, intensity, durationMs]() {
                         if (elapsed->elapsed() >= durationMs) {
                             shakeTimer->stop();
                             shakeTimer->deleteLater();
                             view->setTransform(QTransform());
                             delete elapsed;
                             return;
                         }

                         int dx = QRandomGenerator::global()->bounded(-intensity, intensity + 1);
                         int dy = QRandomGenerator::global()->bounded(-intensity, intensity + 1);
                         view->setTransform(QTransform::fromTranslate(dx, dy));
                     });

    shakeTimer->start(16);
}
