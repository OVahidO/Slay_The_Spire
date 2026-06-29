#include "player.h"

Player::Player(QGraphicsItem *parent)
    : QGraphicsObject{parent}
{}

Player::~Player(){}

QRectF Player::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

void Player::paint(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{}