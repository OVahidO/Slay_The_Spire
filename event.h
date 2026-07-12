#ifndef EVENT_H
#define EVENT_H

#include <QGraphicsObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QVector>
#include <QWidget>
#include <functional>

class Player;
class GamePlay;

enum class EventScope { Act1, Act2, Any };

struct EventOption
{
    QString title;
    QString subtitle;
    QString iconPath;
    std::function<void()> onSelected;
    std::function<bool()> isAvailable = nullptr;
};

class EventGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit EventGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);

signals:
    void itemClicked(QGraphicsItem *item);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class SelectableOptionItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit SelectableOptionItem(const QString &title,
                                  const QString &subtitle,
                                  const QString &iconPath,
                                  QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setOptionEnabled(bool enabled);
    bool isOptionEnabled() const;
    void setSubtitle(const QString &subtitle);

signals:
    void clicked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QString m_title;
    QString m_subtitle;
    QPixmap m_icon;
    bool m_enabled = true;
    bool m_hovered = false;

    static constexpr qreal m_width = 220;
    static constexpr qreal m_height = 260;
};

class Event : public QWidget
{
    Q_OBJECT
public:
    explicit Event(const QString &eventTitle,
                   const QString &backgroundPath,
                   EventScope scope,
                   Player *player,
                   GamePlay *gamePlay,
                   QWidget *parent = nullptr);
    virtual ~Event() = default;

    EventScope scope() const;
    QString eventTitle() const;

signals:
    void eventFinished();
    void triggerEliteCombat();

protected:
    Player *m_player;
    GamePlay *m_gamePlay;

    void addOption(const EventOption &option);

    void setDescription(const QString &text);

private:
    QString m_eventTitle;
    EventScope m_scope;

    QGraphicsScene *m_scene;
    EventGraphicsView *m_view;
    QGraphicsTextItem *m_titleItem;
    QGraphicsTextItem *m_descriptionItem;
    QGraphicsPixmapItem *m_backgroundItem = nullptr;

    QVector<SelectableOptionItem *> m_optionItems;
    QVector<EventOption> m_options;

    void relayoutOptions();
    void refreshAvailability();
};

#endif // EVENT_H
