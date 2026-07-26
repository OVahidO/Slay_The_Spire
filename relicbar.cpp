#include "relicbar.h"
#include "relic.h"
#include "ui_relicbar.h"

RelicBar::RelicBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RelicBar)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setAutoFillBackground(false);

    this->setFixedHeight(70);
    this->setFixedWidth(500);
}

void RelicBar::addRelic(Relic *relic)
{
    ui->horizontalLayout->addWidget(relic);
}

void RelicBar::clearRelics()
{
    while (QLayoutItem *item = ui->horizontalLayout->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
}
