#ifndef POTION_H
#define POTION_H

#include <QWidget>

namespace Ui {
class Potion;
}

class Combatant;

class Potion : public QWidget
{
    Q_OBJECT

public:
    explicit Potion(int value,QString resourcePath, QWidget *parent = nullptr);
    virtual ~Potion();

    int value() const;
    void setValue(int value);

    virtual void applyEffect(Combatant* c) = 0;

signals:
    void potionClicked(Potion* potion);

protected:
    int m_value;
    QString m_resourcePath;

    void mousePressEvent(QMouseEvent* event) override;

private:
    Ui::Potion *ui;

};

class BlockPotion : public Potion
{
public:
    explicit BlockPotion(QWidget *parent = nullptr);
    void applyEffect(Combatant* c) override;
};

class FirePotion : public Potion
{
public:
    explicit FirePotion(QWidget *parent = nullptr);
    void applyEffect(Combatant* c) override;
};

class EnergyPotion : public Potion
{
public:
    explicit EnergyPotion(QWidget *parent = nullptr);
    void applyEffect(Combatant* c) override;
};

class SwiftPotion : public Potion
{
public:
    explicit SwiftPotion(QWidget *parent = nullptr);
    void applyEffect(Combatant *c) override;
};

class Fairy_in_a_Bottle : public Potion
{
public:
    explicit Fairy_in_a_Bottle(QWidget *parent = nullptr);
    void applyEffect(Combatant* c) override;
};

class emptyBottle : public Potion
{
public:
    explicit emptyBottle(QWidget *parent = nullptr);
    void applyEffect(Combatant* c) override;
};

#endif // POTION_H
