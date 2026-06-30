#ifndef ELITEENEMIES_H
#define ELITEENEMIES_H

#include "enemy.h"

class GremlinKnob : public Enemy
{
public:
    explicit GremlinKnob(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);

    void calculateNextIntent() override;
    void onPlayerSkillPlayed();

private:
    int m_enrageStacks = 0;
};


#endif // ELITEENEMIES_H
