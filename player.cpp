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

int Player::Hp() const {return m_Hp;}
int Player::coin() const {return m_coin;}
int Player::energy() const {return m_energy;}
int Player::block() const {return m_block;}
int Player::maxHp() const {return m_maxHp;}

void Player::setHp(int hp) {m_Hp = (hp < 0)? 0 : hp ;}
void Player::setCoin(int coin) {m_coin = (coin < 0)? 0 : coin;}
void Player::setEnergy(int energy) {m_energy = (energy < 0)? 0 : energy;}
void Player::setBlock(int block) {m_block = (block < 0)? 0 : block;}
void Player::setMaxHp(int maxHp) {m_coin = (maxHp < 0)? 0 : maxHp;}

void Player::addMaxHp(int n) {m_maxHp += (n < 0)? 0 : n;}
void Player::addBlock(int n) {m_block += (n < 0)? 0 : n;}
void Player::addEnergy(int n) {m_energy += (n < 0)? 0 : n;}
void Player::loseHp(int n) {m_Hp -= (n < 0)? 0 : n; if(m_Hp < 0) {m_Hp = 0;} }