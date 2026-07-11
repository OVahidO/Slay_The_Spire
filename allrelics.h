#ifndef ALLRELICS_H
#define ALLRELICS_H

#include <QSoundEffect>
#include "relic.h"

class FaceOfTheCleric : public Relic
{
public:
    explicit FaceOfTheCleric(QWidget *parent = nullptr);
    void onCombatEnd(Player *player) override;
};

class CultistHeadpiece : public Relic
{
public:
    explicit CultistHeadpiece(QWidget *parent = nullptr);
    void onCombatStart(GamePlay *game) override;

private:
    QSoundEffect m_cawSound;
};

class MutagenicStrength : public Relic
{
public:
    explicit MutagenicStrength(QWidget *parent = nullptr);
    void onCombatStart(GamePlay *game) override;
    void onTurnEnd(Player *player) override;

private:
    bool m_isActive = false;
};

class WarpedTongs : public Relic
{
public:
    explicit WarpedTongs(QWidget *parent = nullptr);
    void onCombatStart(GamePlay *game) override;
};

class CallingBell : public Relic
{
public:
    explicit CallingBell(QWidget *parent = nullptr);
    void onEquip(GamePlay *game) override;
};

class MarkOfPain : public Relic
{
public:
    explicit MarkOfPain(QWidget *parent = nullptr);
    void onEquip(GamePlay *game) override;
    void onCombatStart(GamePlay *game) override;
};

class VelvetChoker : public Relic
{
public:
    explicit VelvetChoker(QWidget *parent = nullptr);
    void onEquip(GamePlay *game) override;
    void onTurnStart(Player *player) override;
    void onCardPlayed(Card *card, Player *player) override;
};

class BlackStar : public Relic
{
public:
    explicit BlackStar(QWidget *parent = nullptr);
    // void onEquip(GamePlay *game) override;
    // به صورت غیرفعال عمل می‌کند
    // منطق آن توسط GamePlay چک می‌شود.
};

class Girya : public Relic
{
public:
    explicit Girya(QWidget *parent = nullptr);
    // منطق اصلی این رلیک در استراحتگاه (Campfire) پیاده‌سازی می‌شود
    void onCombatStart(GamePlay *game) override;
};

class Kunai : public Relic
{
public:
    explicit Kunai(QWidget *parent = nullptr);
    void onTurnStart(Player *player) override;
    void onCardPlayed(Card *card, Player *player) override;
};

class Shuriken : public Relic
{
public:
    explicit Shuriken(QWidget *parent = nullptr);
    void onTurnStart(Player *player) override;
    void onCardPlayed(Card *card, Player *player) override;
};

class IceCream : public Relic
{
public:
    explicit IceCream(QWidget *parent = nullptr);
    void onTurnEnd(Player *player) override;
    void onTurnStart(Player *player) override;

private:
    int m_savedEnergy = 0;
};

class Anchor : public Relic
{
public:
    explicit Anchor(QWidget *parent = nullptr);
    void onCombatStart(GamePlay *game) override;
};

#endif // ALLRELICS_H
