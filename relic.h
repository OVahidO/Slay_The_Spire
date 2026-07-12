#ifndef RELIC_H
#define RELIC_H

#include <QWidget>

class GamePlay;
class Player;
class Card;

enum class relicType { Normal, Boss, Event };

namespace Ui {
class Relic;
}

class Relic : public QWidget
{
    Q_OBJECT
public:
    explicit Relic(QString name, QString description, relicType type, QWidget *parent = nullptr);
    virtual ~Relic();

    virtual void onCombatStart(GamePlay *game) {}
    virtual void onCombatEnd(Player *player) {}
    virtual void onTurnStart(Player *player) {}
    virtual void onTurnEnd(Player *player) {}
    virtual void onCardPlayed(Card *card, Player *player) {}
    virtual void onTakeDamage(int &damage, Player *player) {}
    virtual void onEquip(GamePlay *game) {}

    QString name() const;
    QString description() const;
    int counter() const;

    void setCounter(int value);

    static Relic *createRandomNormalRelic();

signals:

protected:
    QString m_name;
    QString m_description;
    QString m_soucePath;
    QPixmap m_icon;
    relicType m_type;
    int m_counter = -1; // like for kunai

    void loadIcon();

private:
    Ui::Relic *ui;
};

#endif // RELIC_H
