#include "potion.h"
#include "ui_potion.h"
#include "combatant.h"
#include "player.h"

Potion::Potion(int value,QString resourcePath, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Potion)
{
    ui->setupUi(this);
    setFixedSize(40, 40);

    ui->potionIcon->setFixedSize(30, 30);
    ui->potionIcon->setScaledContents(false);
    ui->potionIcon->setAlignment(Qt::AlignCenter);
    ui->potionIcon->setCursor(Qt::PointingHandCursor);
    m_resourcePath = resourcePath;
    QPixmap pix(m_resourcePath);
    ui->potionIcon->setPixmap(pix.scaled(30,30,Qt::KeepAspectRatio, Qt::SmoothTransformation));

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

void Potion::mousePressEvent(QMouseEvent* event)
{

    emit potionClicked(this);

    QWidget::mousePressEvent(event);
}

BlockPotion::BlockPotion(QWidget *parent)
    :Potion(12, ":/Potions/Pics/Potions/block_potion.png", parent)
{
    this->setToolTip("Gain 12 Block");
}

void BlockPotion::applyEffect(Combatant* c)
{
    c->addBlock(m_value);
}

FirePotion::FirePotion(QWidget *parent)
    :Potion(20, ":/Potions/Pics/Potions/fire_potion.png", parent)
{
    this->setToolTip("Deal 20 damage");
}

void FirePotion::applyEffect(Combatant* c)
{
    c->takeDamage(m_value);
}

EnergyPotion::EnergyPotion(QWidget *parent)
    :Potion(2, ":/Potions/Pics/Potions/energy_potion.png", parent)
{
    this->setToolTip("Gain 2 Energy");
}

void EnergyPotion::applyEffect(Combatant* c)
{
    Player* p;
    if((p = dynamic_cast<Player*>(c)))
    {
        p->addEnergy(m_value);
    }
}

SwiftPotion::SwiftPotion(QWidget *parent)
    :Potion(3, ":/Potions/Pics/Potions/swift_potion.png", parent)
{
    this->setToolTip("Draw 3 cards");
}

void SwiftPotion::applyEffect(Combatant *c) {}

Fairy_in_a_Bottle::Fairy_in_a_Bottle(QWidget *parent)
    :Potion(0, ":/Potions/Pics/Potions/fairy_in_a_bottle.png", parent)
{
    this->setEnabled(false);
    this->setToolTip("When you would die, heal to 30% instead");
}

void Fairy_in_a_Bottle::applyEffect(Combatant* c)
{
    Player* p;
    if((p = dynamic_cast<Player*>(c)))
    {
        p->heal(p->maxHP()*0.3);
    }
}

emptyBottle::emptyBottle(QWidget *parent)
    :Potion(0, ":/Potions/Pics/Potions/radiant_tincture.png", parent)
{
    this->setEnabled(false);
}

void emptyBottle::applyEffect(Combatant* c)
{
}