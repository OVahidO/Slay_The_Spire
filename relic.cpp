#include "relic.h"
#include <QRandomGenerator>
#include "allrelics.h"

#include <QPainter>

Relic::Relic(QString name, QString description, relicType type, QGraphicsItem *parent)
    : m_name(name)
    , m_description(description)
    , m_type(type)
    , QGraphicsObject(parent)
{}

QRectF Relic::boundingRect() const
{
    return QRectF(0, 0, 64, 64);
}

void Relic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    if (!m_icon.isNull()) {
        painter->drawPixmap(boundingRect().toRect(), m_icon);
    } else {
        painter->setBrush(Qt::darkGray);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(boundingRect());

        painter->setPen(Qt::white);
        painter->drawText(boundingRect(), Qt::AlignCenter, "?");
    }

    if (m_counter >= 0) {
        QRectF counterRect(44, 44, 20, 20);

        painter->setBrush(Qt::black);
        painter->setPen(Qt::white);
        painter->drawEllipse(counterRect);

        painter->drawText(counterRect, Qt::AlignCenter, QString::number(m_counter));
    }
}

QString Relic::name() const
{
    return m_name;
}

QString Relic::description() const
{
    return m_description;
}

int Relic::counter() const
{
    return m_counter;
}

void Relic::setCounter(int value)
{
    m_counter = value;
}

void Relic::loadIcon()
{
    m_icon.load(m_soucePath);
}

Relic *createRandomNormalRelic()
{
    // فعلاً با relicهایی که پیاده‌سازی شدن
    // بعداً expand کن
    static const int normalRelicCount = 3; // ← با هر relic جدید آپدیت کن
    int r = QRandomGenerator::global()->bounded(normalRelicCount);
    switch (r) {
    case 0:
        return new FaceOfTheCleric();
    case 1:
        return new MutagenicStrength();
    case 2:
        return new WarpedTongs();
    default:
        return new FaceOfTheCleric();
    }
}