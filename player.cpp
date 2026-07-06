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

int Player::maxEnergy() const
{
    return m_maxEnergy;
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
    emit energyChanged();
}

void Player::setMaxEnergy(int maxEnergy)
{
    m_maxEnergy = (maxEnergy < 0) ? 0 : maxEnergy;
    emit energyChanged();
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
    emit energyChanged();
}

void Player::loseHp(int n)
{
    takeDamage(n + m_block);
}

void Player::loseEnergy(int n)
{
    m_energy -= (n < 0) ? 0 : n;
    if(m_energy < 0)
        m_energy = 0;
    emit energyChanged();
}

void Player::heal(int n)
{
    if (n < 0)
        return;

    m_currentHP += n;

    if (m_currentHP > m_maxHP)
        m_currentHP = m_maxHP;
}

bool Player::cannotPlayAttacks() const
{
    return m_cannotPlayAttacks;
}

void Player::setCannotPlayAttacks(bool value)
{
    m_cannotPlayAttacks = value;
}