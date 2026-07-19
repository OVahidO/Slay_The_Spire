#ifndef CARDDIALOGS_H
#define CARDDIALOGS_H

#include <QDialog>
#include <QGraphicsScene>
#include <QVector>

class Card;
class GamePlay;
class EventGraphicsView;

enum class CardSelectionMode { Upgrade, Remove, PickAny };

class CardSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CardSelectionDialog(GamePlay *gamePlay,
                                 CardSelectionMode mode,
                                 int maxSelection,
                                 const QString &dialogTitle,
                                 QWidget *parent = nullptr);
    ~CardSelectionDialog();

    QVector<Card *> selectedCards() const;

private slots:
    void onSceneItemClicked(QGraphicsItem *item);

private:
    GamePlay *m_gamePlay;
    CardSelectionMode m_mode;
    int m_maxSelection;

    QGraphicsScene *m_scene;
    EventGraphicsView *m_view;

    QVector<Card *> m_selectableCards;
    QVector<Card *> m_selectedCards;

    void layoutCards();
    void finalizeSelection();
};

#endif // CARDDIALOGS_H
