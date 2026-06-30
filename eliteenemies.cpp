#include "eliteenemies.h"

GremlinKnob::GremlinKnob(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Gremlin Knob", 82, 86, isMultiplayer, parent) {
    calculateNextIntent();
}

void GremlinKnob::calculateNextIntent() {
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = EnemyIntent{ IntentType::Buff, 99, 0, false };
        m_enrageStacks = 99;
        return;
    }

    QList<QPair<int, EnemyIntent>> options =
        {
            { 67, EnemyIntent{ IntentType::Attack, 14, 0, false } },
            { 33, EnemyIntent{ IntentType::AttackDebuff, 6, 2, false } }
        };
    m_currentIntent = pickIntent(options);
}

void GremlinKnob::onPlayerSkillPlayed() {
    if (m_enrageStacks > 0) {
        // BuffDebuff* strength = new BuffDebuff("Strength", 2);
        // addEffect(strength);
    }
}


Sentry::Sentry(bool startsWithBeam, bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("Sentry", 38, 42, isMultiplayer, parent), m_startsWithBeam(startsWithBeam) {
    calculateNextIntent();
}

void Sentry::calculateNextIntent() {
    m_turnCount++;

    bool useBeam = m_startsWithBeam ? (m_turnCount % 2 == 1) : (m_turnCount % 2 == 0);

    if (useBeam)
        m_currentIntent = EnemyIntent{ IntentType::Attack, 9, 0, false };
    else
        m_currentIntent = EnemyIntent{ IntentType::Debuff, 2, 0, false };

    // if (!useBeam) {
    //     player->shuffleIntoDiscard(new DAZE());
    //     player->shuffleIntoDiscard(new DAZE());
    // }
}