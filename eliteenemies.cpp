#include "eliteenemies.h"

GremlinKnob::GremlinKnob(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Gremlin Knob", 82, 86, isMultiplayer, parent)
{
    calculateNextIntent();
}

void GremlinKnob::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = buffIntent(99);
        m_enrageStacks = 99;
        return;
    }

    QVector<QPair<int, EnemyIntent>> options = {{67, attackIntent(14)},
                                                {33, attackDebuffIntent(6, 2)}};
    m_currentIntent = pickIntent(options);
}

void GremlinKnob::onPlayerSkillPlayed()
{
    if (m_enrageStacks > 0)
        applyBuffDebuff(BuffDebuffType::Strength, 2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Sentry::Sentry(bool startsWithBeam, bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Sentry", 38, 42, isMultiplayer, parent)
    , m_startsWithBeam(startsWithBeam)
{
    calculateNextIntent();
}

void Sentry::calculateNextIntent()
{
    m_turnCount++;

    bool useBeam = m_startsWithBeam ? (m_turnCount % 2 == 1) : (m_turnCount % 2 == 0);

    if (useBeam)
        m_currentIntent = attackIntent(9);
    else
        m_currentIntent = debuffIntent(2);

    // if (!useBeam) {
    //     player->shuffleIntoDiscard(new DAZE());
    //     player->shuffleIntoDiscard(new DAZE());
    // }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

BookOfStabbing::BookOfStabbing(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Book of Stabbing", 160, 162, isMultiplayer, parent)
{
    calculateNextIntent();
}

void BookOfStabbing::calculateNextIntent()
{
    m_turnCount++;

    QVector<QPair<int, EnemyIntent>> options
        = {{85, EnemyIntent{IntentType::Attack, 6, m_multiStabUsedCount + 2, true}},
           {15, EnemyIntent{IntentType::Attack, 21, 1, false}}};

    m_currentIntent = pickIntent(options);

    if (m_currentIntent.secondaryValue != 1 || m_currentIntent.value != 21)
        m_multiStabUsedCount++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Taskmaster::Taskmaster(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Taskmaster", 54, 60, isMultiplayer, parent)
{
    calculateNextIntent();
}

void Taskmaster::calculateNextIntent()
{
    m_turnCount++;
    m_currentIntent = EnemyIntent{IntentType::Attack, 7, 1, false};

    // player->addCardToDiscard(new WOUND());
}