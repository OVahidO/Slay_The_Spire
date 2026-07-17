#include "relic.h"
#include <QRandomGenerator>
#include "allrelics.h"
#include "ui_relic.h"

Relic::Relic(QString name, QString description, relicType type, QWidget *parent)
    : QWidget(parent)
    , m_name(name)
    , m_description(description)
    , m_type(type)
    , ui(new Ui::Relic)
{
    ui->setupUi(this);
    setFixedSize(64, 64);

    ui->counterLabel->hide();
}

Relic::~Relic()
{
    delete ui;
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
    if (m_counter >= 0) {
        ui->counterLabel->setText(QString::number(m_counter));
        ui->counterLabel->show();
    } else {
        ui->counterLabel->hide();
    }
}

void Relic::loadIcon()
{
    QPixmap pix(m_soucePath);
    ui->relicIcon->setPixmap(pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

Relic *Relic::createRandomNormalRelic()
{
    static const int normalRelicCount = 5;

    int r = QRandomGenerator::global()->bounded(normalRelicCount);

    switch (r) {
    case 0:
        return new Anchor();
    case 1:
        return new Girya();
    case 2:
        return new Kunai();
    case 3:
        return new Shuriken();
    case 4:
        return new IceCream();
    default:
        return new Anchor();
    }
}