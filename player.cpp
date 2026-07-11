#include "player.h"
#include "card.h"
#include "relic.h"

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

QVector<Card *> &Player::Deck()
{
    return m_Deck;
}

QVector<Card *> &Player::HandsCards()
{
    return m_HandsCards;
}

QVector<Potion *> &Player::Potions()
{
    return m_Potions;
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

bool Player::addPotion(Potion* potion)
{
    for(int i=0; i < m_Potions.size() ; i++)
    {
        if(m_Potions[i] == nullptr)
        {
            m_Potions[i] = potion;
            emit potionAdded(potion);
            return true;
        }
    }
    return false;
}

bool Player::cannotPlayAttacks() const
{
    return m_cannotPlayAttacks;
}

void Player::setCannotPlayAttacks(bool value)
{
    m_cannotPlayAttacks = value;
}

QVector<Relic *> &Player::relics()
{
    return m_relics;
}

void Player::addRelic(Relic *relic)
{
    if (relic)
        m_relics.append(relic);
}

void Player::triggerRelicsCombatStart(GamePlay *game)
{
    for (Relic *r : m_relics)
        r->onCombatStart(game);
}

void Player::triggerRelicsCombatEnd()
{
    for (Relic *r : m_relics)
        r->onCombatEnd(this);
}

void Player::triggerRelicsTurnStart()
{
    for (Relic *r : m_relics)
        r->onTurnStart(this);
}

void Player::triggerRelicsTurnEnd()
{
    for (Relic *r : m_relics)
        r->onTurnEnd(this);
}