#include "bossenemies.h"

KingSlime::KingSlime(bool isMultiplayer, QGraphicsItem *parent)
    : Slime("King Slime", 140, 140, isMultiplayer, parent)
{
    calculateNextIntent();
}

void KingSlime::calculateNextIntent()
{
    m_turnCount++;

    if (!m_hasSplit && shouldSplit()) {
        m_currentIntent = EnemyIntent{IntentType::Unknown, 0, 0, false};
        return;
    }

    int phase = (m_turnCount - 1) % 3;

    switch (phase) {
    case 0:
        m_currentIntent = EnemyIntent{IntentType::Unknown, 0, 0, false};
        break;
    case 1:
        m_currentIntent = EnemyIntent{IntentType::Attack, 35, 0, false};
        break;
    case 2:
        m_currentIntent = EnemyIntent{IntentType::Debuff, 3, 0, false};
        break;
    }

    // if (phase == 2) {
    //     for (int i = 0; i < 3; ++i)
    //         player->addCardToDiscard(new SLIME());
    // }
}

bool KingSlime::shouldSplit() const
{
    return !m_hasSplit && m_currentHP <= m_maxHP / 2;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

HexaGhost::HexaGhost(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("HexaGhost", 250, 250, isMultiplayer, parent)
{
    calculateNextIntent();
}

void HexaGhost::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount == 1) {
        m_currentIntent = EnemyIntent{IntentType::Unknown, 0, 0, false};
        return;
    }

    if (m_turnCount == 2) {
        int dividerDamage = 1; // placeholder
        m_currentIntent = EnemyIntent{IntentType::Attack, dividerDamage, 6, false};
        return;
    }

    int cycleIndex = (m_turnCount - 3) % 7;

    switch (cycleIndex) {
    case 0: // Sear
    case 2:
    case 5:
        m_currentIntent = EnemyIntent{IntentType::Attack, 6, 1, false};
        break;
    case 1: // Tackle
    case 4:
        m_currentIntent = EnemyIntent{IntentType::Attack, 5, 2, false};
        break;
    case 3: // Inflame
        m_currentIntent = EnemyIntent{IntentType::DefendBuff, 12, 2, false};
        break;
    case 6: // Inferno
        m_currentIntent = EnemyIntent{IntentType::Attack, 2, 6, false};
        break;
    }

    // if (cycleIndex == 0 || cycleIndex == 2 || cycleIndex == 5)
    //     player->addCardToDiscard(new Burn());
    // if (cycleIndex == 6) {
    //     for (int i = 0; i < 3; ++i)
    //         player->addCardToDiscard(new Burn());
    //     player->upgradeAllBurnsInDeck();
    // }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

TheChamp::TheChamp(bool isMultiplayer, QGraphicsItem *parent)
    : Enemy("The Champ", 420, 420, isMultiplayer, parent)
{
    calculateNextIntent();
}

void TheChamp::calculateNextIntent()
{
    m_turnCount++;

    if (m_turnCount % 2 == 1) {
        m_currentIntent = EnemyIntent{IntentType::Debuff, 2, 2, false};
        return;
    }

    QList<QPair<int, EnemyIntent>> options = {{15,
                                               EnemyIntent{IntentType::DefendBuff, 15, 5, false}},
                                              {15, EnemyIntent{IntentType::Buff, 2, 0, false}},
                                              {25,
                                               EnemyIntent{IntentType::AttackDebuff, 12, 2, false}},
                                              {45, EnemyIntent{IntentType::Attack, 8, 2, false}}};

    m_currentIntent = pickIntent(options);
}