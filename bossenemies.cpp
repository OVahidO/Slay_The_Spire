#include "bossenemies.h"

KingSlime::KingSlime(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("King Slime", 140, 140, isMultiplayer, parent) { calculateNextIntent(); }

void KingSlime::calculateNextIntent() {
    m_turnCount++;

    if (!m_hasSplit && shouldSplit()) {
        m_currentIntent = EnemyIntent{ IntentType::Unknown, 0, 0, false };
        return;
    }

    int phase = (m_turnCount - 1) % 3;

    switch (phase) {
    case 0:
        m_currentIntent = EnemyIntent{ IntentType::Unknown, 0, 0, false };
        break;
    case 1:
        m_currentIntent = EnemyIntent{ IntentType::Attack, 35, 0, false };
        break;
    case 2:
        m_currentIntent = EnemyIntent{ IntentType::Debuff, 3, 0, false };
        break;
    }

    // if (phase == 2) {
    //     for (int i = 0; i < 3; ++i)
    //         player->addCardToDiscard(new SLIME());
    // }
}

bool KingSlime::shouldSplit() const { return !m_hasSplit && m_currentHP <= m_maxHP / 2; }