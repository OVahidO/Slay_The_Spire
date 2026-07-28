#include "endcombatpages.h"
#include "ui_endcombatpages.h"
#include <QPainter>

endCombatPages::endCombatPages(endMod endMod, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::endCombatPages)
{
    ui->setupUi(this);

    this->setGeometry(525,100,500,625);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    setAttribute(Qt::WA_TranslucentBackground);

    if(endMod == endMod::Victory)
    {
        ui->endPageLabel->setText("Victory");
    }
    else
    {
        ui->endPageLabel->setText("Defate");
    }
}

endCombatPages::~endCombatPages()
{
    delete ui;
}

void endCombatPages::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPixmap background(":/EndCombatPages/Pics/EndCombatPages/Bg2.png");
    painter.drawPixmap(this->rect(), background);
}

void endCombatPages::on_endPageButton_clicked()
{
    this->accept();
}

