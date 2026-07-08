#ifndef ALLRELICS_H
#define ALLRELICS_H

#include <QSoundEffect>
#include "relic.h"

class FaceOfTheCleric : public Relic
{
public:
    explicit FaceOfTheCleric(QGraphicsItem *parent = nullptr);
    void onCombatEnd(Player *player) override;
};

class CultistHeadpiece : public Relic
{
public:
    explicit CultistHeadpiece(QGraphicsItem *parent = nullptr);
    void onCombatStart(GamePlay *game) override;

private:
    QSoundEffect m_cawSound;
};

class MutagenicStrength : public Relic
{
public:
    explicit MutagenicStrength(QGraphicsItem *parent = nullptr);
    void onCombatStart(GamePlay *game) override;
    void onTurnEnd(Player *player) override;

private:
    bool m_isActive = false;
};

class WarpedTongs : public Relic
{
public:
    explicit WarpedTongs(QGraphicsItem *parent = nullptr);
    void onCombatStart(GamePlay *game) override;
};

#endif // ALLRELICS_H
