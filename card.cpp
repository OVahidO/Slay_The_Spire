#include "card.h"

Card::Card(QString name, CardType type, int energyCost, QString path, bool isRare, QGraphicsItem *parent)
    :QGraphicsObject(parent), m_ID(rand()%1000+1000), m_name(name), m_type(type), m_energyCost(energyCost), m_sourcePath(path), m_isRare(isRare)
{}

Card::~Card(){}

QRectF Card::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

void Card::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{}