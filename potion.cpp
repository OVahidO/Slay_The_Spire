#include "potion.h"
#include "ui_potion.h"
#include "combatant.h"
#include "player.h"

Potion::Potion(int value,QString resourcePath, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Potion)
{
    ui->setupUi(this);
    setValue(value);
}

Potion::~Potion()
{
    delete ui;
}

int Potion::value() const
{
    return m_value;
}

void Potion::setValue(int value)
{
    m_value = (value < 0)? 0 : value;
}

BlockPotion::BlockPotion(QWidget *parent)
    :Potion(12, ":/Potions/Pics/Potions/block_potion.png", parent) {}

void BlockPotion::applyEffect(Combatant* c)
{
    c->addBlock(m_value);
}

FirePotion::FirePotion(QWidget *parent)
    :Potion(20, ":/Potions/Pics/Potions/fire_potion.png", parent) {}

void FirePotion::applyEffect(Combatant* c)
{
    c->takeDamage(m_value);
}

EnergyPotion::EnergyPotion(QWidget *parent)
    :Potion(2, ":/Potions/Pics/Potions/energy_potion.png", parent) {}

void EnergyPotion::applyEffect(Combatant* c)
{
    Player* p;
    if((p = dynamic_cast<Player*>(c)))
    {
        p->addEnergy(m_value);
    }
}

SwiftPotion::SwiftPotion(QWidget *parent)
    :Potion(3, ":/Potions/Pics/Potions/swift_potion.png", parent) {}

//void SwiftPotion::applyEffect(Combatant* c) {}

Fairy_in_a_Bottle::Fairy_in_a_Bottle(QWidget *parent)
    :Potion(0, ":/Potions/Pics/Potions/fairy_in_a_bottle.png", parent)
{
    this->disconnect();
}

void Fairy_in_a_Bottle::applyEffect(Combatant* c)
{
    Player* p;
    if((p = dynamic_cast<Player*>(c)))
    {
        p->heal(p->maxHP()*0.3);
    }
}