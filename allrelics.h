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

class CallingBell : public Relic
{
public:
    explicit CallingBell(QGraphicsItem *parent = nullptr);
    void onEquip(GamePlay *game) override;
};

class MarkOfPain : public Relic
{
public:
    explicit MarkOfPain(QGraphicsItem *parent = nullptr);
    void onEquip(GamePlay *game) override;
    void onCombatStart(GamePlay *game) override;
};

class VelvetChoker : public Relic
{
public:
    explicit VelvetChoker(QGraphicsItem *parent = nullptr);
    void onEquip(GamePlay *game) override;
    void onTurnStart(Player *player) override;
    void onCardPlayed(Card *card, Player *player) override;
};

class BlackStar : public Relic
{
public:
    explicit BlackStar(QGraphicsItem *parent = nullptr);
    void onEquip(GamePlay *game) override;

    // به صورت غیرفعال عمل می‌کند
    // منطق آن توسط GamePlay چک می‌شود.
};

#endif // ALLRELICS_H
