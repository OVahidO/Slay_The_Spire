#include "powercards.h"
#include "enemy.h"
#include "gameplay.h"
#include "player.h"
PowerCard::PowerCard(CardID ID,
                     QString name,
                     int energyCost,
                     QString description,
                     bool requiresTarget,
                     bool isExhaust,
                     bool isRare,
                     QGraphicsItem *parent)
    : Card(ID, name, CardType::Power, energyCost, description, isRare, isExhaust, requiresTarget, parent)
{}

QString PowerCard::getDynamicDescription(Player *player, Enemy *target) const
{
    return m_description;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Inflame::Inflame(QGraphicsItem *parent)
    : PowerCard(CardID::Inflame, "Inflame", 1, "Gain 2 Strength", false, false, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Power/inflame.png";
    loadPixmap();
}

void Inflame::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player)
        player->applyBuffDebuff(BuffDebuffType::Strength, m_strengthValue);
}

void Inflame::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_strengthValue += 1;
    m_description = "Gain 3 Strength";
    m_name = "Inflame+";
}

Card *Inflame::clone() const
{
    Inflame *copy = new Inflame();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool InflameRegistered = []()
{
    Card::creators()[CardID::Inflame] = [](){return new class Inflame;};
    return true;
}();

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Brutality::Brutality(QGraphicsItem *parent)
    : PowerCard(CardID::Brutality,
                "Brutality",
                0,
                "At the start of your turn, lose 1 HP and draw 1 card.",
                false,
                false,
                true,
                parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Power/Red-Brutality-Art.png";
    loadPixmap();
}

void Brutality::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->powerEffects().append(PowerEffect{1,
                                                  [](Combatant *self, int value, GamePlay *game) {
                                                      Player *p = dynamic_cast<Player *>(self);
                                                      if (p)
                                                          p->loseHp(value);

                                                      if (game)
                                                          game->drawFromDrawPile();
                                                  },
                                                  PowerUseTime::StartTurn});
        player->applyBuffDebuff(BuffDebuffType::Brutality, 1);
    }
}

void Brutality::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    setInnate(true);

    m_name = "Brutality+";
    m_description = "At the start of your turn, lose 1 HP and draw 1 card/nInnate.";
}

Card *Brutality::clone() const
{
    Brutality *copy = new Brutality();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool BrutalityRegistered = []()
{
    Card::creators()[CardID::Brutality] = [](){return new class Brutality;};
    return true;
}();

///////////////////////////////////////////////////////////////////////////////////////////////////////////

DemonForm::DemonForm(QGraphicsItem *parent)
    : PowerCard(CardID::DemonForm,
                "Demon Form",
                3,
                "At the start of your turn gain 3 Strength",
                false,
                false,
                true,
                parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Power/demon_form.png";
    loadPixmap();
}

void DemonForm::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->powerEffects().append(
            PowerEffect{m_strengthPerTurn,
                        [](Combatant *self, int value, GamePlay *) {
                            self->applyBuffDebuff(BuffDebuffType::Strength, value);
                        },
                        PowerUseTime::StartTurn});
        player->applyBuffDebuff(BuffDebuffType::DemonForm, m_strengthPerTurn);
    }
}

void DemonForm::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_strengthPerTurn += 1;
    m_description = "At the start of your turn gain 4 Strength";
    m_name = "DemonForm+";
}

Card *DemonForm::clone() const
{
    DemonForm *copy = new DemonForm();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool DemonFormRegistered = []()
{
    Card::creators()[CardID::DemonForm] = [](){return new class DemonForm;};
    return true;
}();

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Metallicize::Metallicize(QGraphicsItem *parent)
    : PowerCard(CardID::Metallicize, "Metallicize", 1, "At the end of your turn get 3 block", false, false, false, parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Power/Red-Metallicize-Art.png";
    loadPixmap();
}

void Metallicize::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->powerEffects().append(
            PowerEffect{m_blockPerTurn,
                        [](Combatant *self, int value, GamePlay *) { self->addBlock(value); },
                        PowerUseTime::EndTurn});
        player->applyBuffDebuff(BuffDebuffType::Metallicize, m_blockPerTurn);
    }
}

void Metallicize::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_blockPerTurn += 1;
    m_name = "Metallicize+";
    m_description = "At the end of your turn get 4 block";
}

Card *Metallicize::clone() const
{
    Metallicize *copy = new Metallicize();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool MetallicizeRegistered = []()
{
    Card::creators()[CardID::Metallicize] = [](){return new class Metallicize;};
    return true;
}();

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Berserk::Berserk(QGraphicsItem *parent)
    : PowerCard(CardID::Berserk,
                "Berserk",
                0,
                "Gain 2 Vulnerable\nAt the start of your turn get 1 extra Energy",
                false,
                false,
                true,
                parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Power/Red-Berserk-Art.png";
    loadPixmap();
}

void Berserk::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->applyBuffDebuff(BuffDebuffType::Vulnerable, m_vulnerableAmount);
        player->applyBuffDebuff(BuffDebuffType::Berserk, 1);

        player->powerEffects().append(PowerEffect{1,
                                                  [](Combatant *self, int value, GamePlay *) {
                                                      Player *p = dynamic_cast<Player *>(self);
                                                      if (p)
                                                          p->addEnergy(value);
                                                  },
                                                  PowerUseTime::StartTurn});
    }
}

void Berserk::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_vulnerableAmount -= 1;
    m_name = "Berserk+";
    m_description = "Gain 2 Vulnerable\nAt the start of your turn get 1 extra Energy";
}

Card *Berserk::clone() const
{
    Berserk *copy = new Berserk();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool BerserkRegistered = []()
{
    Card::creators()[CardID::Berserk] = [](){return new class Berserk;};
    return true;
}();

///////////////////////////////////////////////////////////////////////////////////////////////////////////

DarkEmbrace::DarkEmbrace(QGraphicsItem *parent)
    : PowerCard(CardID::DarkEmbrace,
                "Dark Embrace",
                2,
                "Every time a card is Exhausted, draw a card",
                false,
                false,
                false,
                parent)
{
    m_sourcePath = ":/card-art/Pics/Cards/Power/dark_embrace.png";
    loadPixmap();
}

void DarkEmbrace::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->powerEffects().append(PowerEffect{0,
                                                  [](Combatant *self, int value, GamePlay *game) {
                                                      Q_UNUSED(self);
                                                      Q_UNUSED(value);

                                                      if (game)
                                                          game->drawFromDrawPile();
                                                  },
                                                  PowerUseTime::OnExhaust});
    }
}

void DarkEmbrace::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_energyCost -= 1;
    m_name = "DarkEmbrace+";
}

Card *DarkEmbrace::clone() const
{
    DarkEmbrace *copy = new DarkEmbrace();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool DarkEmbraceRegistered = []()
{
    Card::creators()[CardID::DarkEmbrace] = [](){return new class DarkEmbrace;};
    return true;
}();

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Barricade::Barricade(QGraphicsItem *parent)
    : PowerCard(CardID::Barricade,
                "Barricade",
                3,
                "Block is not removed at the start of your turn",
                false,
                false,
                true,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Power/Barricade.png";
    loadPixmap();
}

void Barricade::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->setBarricade(true);
        player->applyBuffDebuff(BuffDebuffType::Barricade, 1);
    }
}

void Barricade::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_energyCost -= 1;
    m_name = "Barricade+";
}

Card *Barricade::clone() const
{
    Barricade *copy = new Barricade();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool BarricadeRegistered = []()
{
    Card::creators()[CardID::Barricade] = [](){return new class Barricade;};
    return true;
}();

///////////////////////////////////////////////////////////////////////////////////////////////////////////

FeelNoPain::FeelNoPain(QGraphicsItem *parent)
    : PowerCard(CardID::FeelNoPain,
                "Feel No Pain",
                1,
                "Every time a card is Exhausted, gain 3 block",
                false,
                false,
                false,
                parent)
{
    m_sourcePath = ":/cards/Pics/Cards/Power/FeelNoPain.png";
    loadPixmap();
}

void FeelNoPain::applyEffect(Player *player, Enemy *target)
{
    Q_UNUSED(target);

    if (player) {
        player->powerEffects().append(
            PowerEffect{m_blockAmount,
                        [](Combatant *self, int value, GamePlay *) { self->addBlock(value); },
                        PowerUseTime::OnExhaust});
        player->applyBuffDebuff(BuffDebuffType::FeelNoPain, m_blockAmount);
    }
}

void FeelNoPain::upgrade()
{
    if (m_isUpgraded)
        return;

    Card::upgrade();

    m_blockAmount++;
    m_name = "Feel No Pain+";
    m_description = "Every time a card is Exhausted, gain 4 block";
}

Card *FeelNoPain::clone() const
{
    FeelNoPain *copy = new FeelNoPain();

    if (m_isUpgraded)
        copy->upgrade();

    copy->setLifetime(lifetime());

    return copy;
}

static inline bool FeelNoPainRegistered = []()
{
    Card::creators()[CardID::FeelNoPain] = [](){return new class FeelNoPain;};
    return true;
}();