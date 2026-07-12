#ifndef ALLEVENTS_H
#define ALLEVENTS_H

#include "event.h"

class Card;
class Player;
class GamePlay;

class OminousForge : public Event
{
    Q_OBJECT
public:
    explicit OminousForge(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);

private:
    void onForge();
    void onRummage();
};

class Lab : public Event
{
    Q_OBJECT
public:
    explicit Lab(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);

private:
    void onSearch();
};

class GoldenIdolEvent : public Event
{
    Q_OBJECT
public:
    explicit GoldenIdolEvent(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);

private:
    void onSteal();
};

class TheCleric : public Event
{
    Q_OBJECT
public:
    explicit TheCleric(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);

private:
    void onHeal();
    void onPurify();
};

class Augmenter : public Event
{
    Q_OBJECT
public:
    explicit Augmenter(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);

private:
    void onTestJAX();
    void onBecomeTestSubject();
    void onIngestMutagens();
    void transformCard(Card *card);
};

class FaceTrader : public Event
{
    Q_OBJECT
public:
    explicit FaceTrader(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);

private:
    void onTouch();
    void onTrade();
};

class Colosseum : public Event
{
    Q_OBJECT
public:
    explicit Colosseum(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);

private:
    void onFight();
};

class PleadingVagrant : public Event
{
    Q_OBJECT
public:
    explicit PleadingVagrant(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);

private:
    void onGive();
    void onRob();
};

class TheJoust : public Event
{
    Q_OBJECT
public:
    explicit TheJoust(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);

private:
    void placeBet(int winChancePercent, int winAmount);
};

#endif // ALLEVENTS_H
