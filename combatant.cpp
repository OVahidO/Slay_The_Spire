#include "combatant.h"
#include <QtAlgorithms>
#include <QtGlobal>

Combatant::Combatant(QString name, int maxHP, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_name(name)
    , m_maxHP(maxHP)
    , m_currentHP(maxHP)
    , m_block(0)
    , m_turnCount(0)
{
    m_healthBar = new HealthBarItem(this);
    m_healthBar->setPos(0, 150);
    m_healthBar->updateStats(m_currentHP, m_maxHP, m_block);
}

Combatant::~Combatant()
{
    qDeleteAll(m_activeEffects);
}

int Combatant::takeDamage(int incomingDamage, bool isAttackDamage)
{
    int modified = incomingDamage;

    if (isAttackDamage && effectStacks(BuffDebuffType::Vulnerable) > 0)
        modified = static_cast<int>(modified * 1.5);

    int damageAfterBlock = modified - m_block;
    if (damageAfterBlock < 0)
        damageAfterBlock = 0;

    m_block -= modified;
    if (m_block < 0)
        m_block = 0;

    m_currentHP -= damageAfterBlock;
    if (m_currentHP < 0)
        m_currentHP = 0;

    m_healthBar->updateStats(m_currentHP, m_maxHP, m_block);

    emit combatStateChanged();

    return damageAfterBlock;
}

void Combatant::setBlock(int amount)
{
    m_block = amount;
    m_healthBar->updateStats(m_currentHP, m_maxHP, m_block);
    emit combatStateChanged();
}

void Combatant::addBlock(int amount)
{
    m_block += amount;
    m_healthBar->updateStats(m_currentHP, m_maxHP, m_block);
    emit combatStateChanged();
}

void Combatant::addBlockFromCard(int amount)
{
    addBlock(calculateBlock(amount));
}

void Combatant::resetBlock()
{
    if (!m_hasBarricade)
        m_block = 0;
    m_healthBar->updateStats(m_currentHP, m_maxHP, m_block);
    emit combatStateChanged();
}

bool Combatant::isDead() const
{
    return m_currentHP <= 0;
}

const QString &Combatant::name() const
{
    return m_name;
}

int Combatant::currentHP() const
{
    return m_currentHP;
}

int Combatant::maxHP() const
{
    return m_maxHP;
}

int Combatant::block() const
{
    return m_block;
}

int Combatant::turnCount() const
{
    return m_turnCount;
}

void Combatant::nextTurn()
{
    m_turnCount++;
}

void Combatant::setBarricade(bool status)
{
    m_hasBarricade = status;
}

bool Combatant::hasBarricade() const
{
    return m_hasBarricade;
}

int Combatant::effectStacks(BuffDebuffType type) const
{
    for (BuffDebuff *effect : m_activeEffects)
        if (effect->type() == type)
            return effect->stacks();

    return 0;
}

QVector<BuffDebuff *> Combatant::getActiveEffects() const
{
    return m_activeEffects;
}

void Combatant::applyBuffDebuff(BuffDebuffType type, int stacks)
{
    for (int i = 0; i < m_activeEffects.size(); ++i) {
        if (m_activeEffects[i]->type() == type) {
            m_activeEffects[i]->addStacks(stacks);

            if (m_activeEffects[i]->stacks() == 0) {
                delete m_activeEffects[i];
                m_activeEffects.removeAt(i);
            }

            updateBuffUI();
            return;
        }
    }

    if (stacks != 0) {
        m_activeEffects.append(new BuffDebuff(type, stacks));
        updateBuffUI();
    }
    emit combatStateChanged();
}

void Combatant::tickDecayingBuffDebuff()
{
    for (int i = m_activeEffects.size() - 1; i >= 0; --i) {
        m_activeEffects[i]->decayOneStack();

        if (m_activeEffects[i]->isDepleted()) {
            delete m_activeEffects[i];
            m_activeEffects.removeAt(i);
        }
    }

    updateBuffUI();
    emit combatStateChanged();
}

int Combatant::calculateOutgoingDamage(int baseDamage) const
{
    int modified = baseDamage + effectStacks(BuffDebuffType::Strength);

    if (effectStacks(BuffDebuffType::Weak) > 0)
        modified = static_cast<int>(modified * 0.75);

    if (modified < 0)
        modified = 0;

    return modified;
}

int Combatant::calculateBlock(int baseAmount) const
{
    int modified = baseAmount + effectStacks(BuffDebuffType::Dexterity);

    if (effectStacks(BuffDebuffType::Frail) > 0)
        modified = static_cast<int>(modified * 0.75);

    return qMax(0, modified);
}

QVector<PowerEffect> &Combatant::powerEffects()
{
    return m_powerEffects;
}

void Combatant::triggerPowerEffects(PowerUseTime time, GamePlay *game)
{
    for (const PowerEffect &effect : m_powerEffects)
        if (effect.useTime == time)
            effect.func(this, effect.value, game);
}

void Combatant::overrideHP(int hp)
{
    if (hp < 1)
        hp = 1;

    m_maxHP = hp;
    m_currentHP = hp;
    m_healthBar->updateStats(m_currentHP, m_maxHP, m_block);
}

void Combatant::setCurrentHPDirect(int hp)
{
    if (hp < 0)
        hp = 0;
    if (hp > m_maxHP)
        hp = m_maxHP;

    m_currentHP = hp;
    m_healthBar->updateStats(m_currentHP, m_maxHP, m_block);
}

HealthBarItem::HealthBarItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setAcceptHoverEvents(true);
}

void HealthBarItem::updateStats(int currentHp, int maxHp, int block)
{
    m_currentHp = currentHp;
    m_maxHp = maxHp;
    m_block = block;

    QString tooltipText = QString("HP: %1 / %2").arg(m_currentHp).arg(m_maxHp);
    if (m_block > 0) {
        tooltipText += QString("\nBlock: %1").arg(m_block);
    }
    setToolTip(tooltipText);

    update();
}

QRectF HealthBarItem::boundingRect() const
{
    return QRectF(0, -20, m_width, m_height + 20);
}

int HealthBarItem::width() const
{
    return m_width;
}
int HealthBarItem::height() const
{
    return m_height;
}

void HealthBarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    qreal hpPercentage = (qreal) m_currentHp / (qreal) m_maxHp;
    if (hpPercentage < 0.0)
        hpPercentage = 0.0;
    if (hpPercentage > 1.0)
        hpPercentage = 1.0;

    painter->setBrush(QColor(50, 50, 50));
    painter->setPen(Qt::black);
    painter->drawRect(0, 0, m_width, m_height);

    painter->setBrush(QColor(200, 50, 50));
    painter->drawRect(0, 0, m_width * hpPercentage, m_height);

    if (m_block > 0) {
        painter->setPen(QPen(QColor(100, 150, 255), 2));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(0, 0, m_width, m_height);

        painter->setPen(QColor(100, 150, 255));
        painter->drawText(QRectF(0, -18, m_width, 15), Qt::AlignLeft, QString("[%1]").arg(m_block));
    }

    painter->setPen(Qt::white);
    painter->drawText(QRectF(0, 0, m_width, m_height),
                      Qt::AlignCenter,
                      QString("%1/%2").arg(m_currentHp).arg(m_maxHp));
}

BuffItem::BuffItem(BuffDebuffType type, int stacks, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_type(type)
    , m_stacks(stacks)
{
    setAcceptHoverEvents(true);
    setupAppearance();
}

void BuffItem::setupAppearance()
{
    QString tooltipText;

    switch (m_type) {
    case BuffDebuffType::Strength:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_Strength.png");
        tooltipText = "Deal additional damage.";
        break;
    case BuffDebuffType::Vulnerable:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Debuffs/StS2_Icon_Vulnerable.png");
        tooltipText = "Take 50% more damage from attacks.";
        break;
    case BuffDebuffType::Weak:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Debuffs/StS2_Icon_Weak.png");
        tooltipText = "Deal 50% less damage with attacks.";
        break;
    case BuffDebuffType::Dexterity:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Debuffs/StS2_Icon_Weak.png");
        tooltipText = "Improves Block Block gained from cards.";
        break;
    case BuffDebuffType::Frail:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Debuffs/StS2_Icon_Frail.png");
        tooltipText = "Gain 25% less Block from cards.";
        break;
    case BuffDebuffType::Barricade:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/Icon_Barricade.png");
        tooltipText = "Block Block is not removed at the beginning of your turn.";
        break;
    case BuffDebuffType::Berserk:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/Icon_Berserk.png");
        tooltipText = "At the start of your turn, gain Energy.";
        break;
    case BuffDebuffType::Brutality:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/Icon_Brutality.png");
        tooltipText = "At the start of your turn, lose HP and draw cards.";
        break;
    case BuffDebuffType::Metallicize:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/Icon_Metallicize.png");
        tooltipText = "At the end of every turn, gain Block.";
        break;
    case BuffDebuffType::DemonForm:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_DemonForm.png");
        tooltipText = "At the start of your turn, gain Strength.";
        break;
    case BuffDebuffType::FeelNoPain:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_FeelNoPain.png");
        tooltipText = "Whenever you Exhaust a card, gain Block.";
        break;
    case BuffDebuffType::Flex:
        m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_FlexPotion.png");
        tooltipText
            = "Strength adds additional damage to Attacks. At the end of this turn, lose Strength.";
        break;
        // case BuffDebuffType::Intangible:
        //     m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_FlexPotion.png");
        //     tooltipText
        //         = "Strength adds additional damage to Attacks. At the end of this turn, lose Strength.";
        //     break;
        // case BuffDebuffType::Rage:
        //     m_icon.load(":/buffdebuff-intent/Pics/Buff-Debuff/Buffs/StS2_Icon_FlexPotion.png");
        //     tooltipText
        //         = "Strength adds additional damage to Attacks. At the end of this turn, lose Strength.";
        //     break;

    default:
        tooltipText = "Unknown Effect";
        break;
    }

    setToolTip(tooltipText);
}

void BuffItem::setStacks(int stacks)
{
    m_stacks = stacks;
    update();
}

QRectF BuffItem::boundingRect() const
{
    return QRectF(0, 0, 32, 32);
}

void BuffItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_icon.isNull()) {
        painter->drawPixmap(0, 0, 32, 32, m_icon);
    }

    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 10, QFont::Bold));
    painter->drawText(boundingRect(), Qt::AlignBottom | Qt::AlignRight, QString::number(m_stacks));
}

void Combatant::updateBuffUI()
{
    for (BuffItem *item : m_buffItems)
        delete item;
    m_buffItems.clear();

    int startX = 0;
    int startY = m_healthBar->pos().y() + m_healthBar->height() + 10;
    int spacing = 35;

    for (BuffDebuff *effect : m_activeEffects) {
        if (effect->stacks() <= 0)
            continue;

        BuffItem *newItem = new BuffItem(effect->type(), effect->stacks(), this);
        newItem->setPos(startX + (m_buffItems.size() * spacing), startY);
        m_buffItems.append(newItem);
    }
}

BuffDebuffType BuffItem::Btype() const
{
    return m_type;
}