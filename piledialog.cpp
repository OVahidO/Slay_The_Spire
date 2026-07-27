#include "piledialog.h"
#include "ui_piledialog.h"
#include "card.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

PileDialog::PileDialog(const std::vector<Card*>& pile, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PileDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsView* view = ui->graphicsView;
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, width(), height());

    QLinearGradient bgGradient(0, 0, 0, 500);
    bgGradient.setColorAt(0.0, QColor(35, 30, 45));
    bgGradient.setColorAt(1.0, QColor(15, 12, 20));
    scene->setBackgroundBrush(bgGradient);

    view->setScene(scene);
    int i=0;
    for(auto& card : pile)
    {
        Card* clone = Card::Creat(static_cast<CardID>(card->ID()) , card->isUpgraded());
        clone->setFlag(QGraphicsItem::ItemIsMovable , false);
        clone->setFlag(QGraphicsItem::ItemIsSelectable , false);
        int XSpace = (i%4)*(clone->boundingRect().width() + 15);
        int YSpace = (i/4)*(clone->boundingRect().height() + 15);
        clone->setPos(50 + XSpace, 50 + YSpace);
        scene->addItem(clone);
        i++;
    }
}

PileDialog::~PileDialog()
{
    delete ui;
}

void PileDialog::on_BackButton_clicked()
{
    this->accept();
}

