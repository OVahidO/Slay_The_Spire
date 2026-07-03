#include "player.h"
#include "card.h"

Player::Player(QString name, int maxHP, QGraphicsItem *parent)
    : Combatant(name, maxHP, parent)
{}

QRectF Player::boundingRect() const
{
    return QRectF(0, 0, 100, 100);
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

int Player::coin() const
{
    return m_coin;
}

int Player::energy() const
{
    return m_energy;
}

int Player::handSize() const
{
    return m_handSize;
}

QVector<Card *> &Player::HandsCards()
{
    return m_HandsCards;
}

void Player::setCoin(int coin)
{
    m_coin = (coin < 0) ? 0 : coin;
}

void Player::setEnergy(int energy)
{
    m_energy = (energy < 0) ? 0 : energy;
}

void Player::setHandSize(int handSize)
{
    m_handSize = (handSize < 0) ? 0 : handSize;
}

void Player::addMaxHp(int n)
{
    m_maxHP += (n < 0) ? 0 : n;
}

void Player::addEnergy(int n)
{
    m_energy += (n < 0) ? 0 : n;
}

void Player::loseHp(int n)
{
    takeDamage(n + m_block);
}

void Player::heal(int n)
{
    if (n < 0)
        return;

    m_currentHP += n;

    if (m_currentHP > m_maxHP)
        m_currentHP = m_maxHP;
}

QVector<PowerEffect> &Player::powerEffects()
{
    return m_powerEffects;
}