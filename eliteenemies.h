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

class Sentry : public Enemy
{
public:
    explicit Sentry(bool startsWithBeam,
                    bool isMultiplayer = false,
                    QGraphicsItem *parent = nullptr);

    void calculateNextIntent() override;

private:
    bool m_startsWithBeam;
};

class BookOfStabbing : public Enemy
{
public:
    explicit BookOfStabbing(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);

    void calculateNextIntent() override;

private:
    int m_multiStabUsedCount = 0;
};

class Taskmaster : public Enemy
{
public:
    explicit Taskmaster(bool isMultiplayer = false, QGraphicsItem *parent = nullptr);

    void calculateNextIntent() override;
};

#endif // ELITEENEMIES_H
