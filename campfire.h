#ifndef CAMPFIRE_H
#define CAMPFIRE_H

#include <QDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

class Player;
class GamePlay;
class Card;
class Relic;

class CampfireGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CampfireGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);

signals:
    void itemClicked(QGraphicsItem *item);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class CampfireOptionItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit CampfireOptionItem(const QString &title,
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

    static constexpr qreal m_width = 190;
    static constexpr qreal m_height = 175;
};

class Campfire : public QWidget
{
    Q_OBJECT
public:
    explicit Campfire(Player *player, GamePlay *gamePlay, QWidget *parent = nullptr);
    ~Campfire();

signals:
    void campfireFinished();

private slots:
    void onRestClicked();
    void onSmithClicked();
    void onLiftClicked();

private:
    Player *m_player;
    GamePlay *m_gamePlay;

    QGraphicsScene *m_scene;
    CampfireGraphicsView *m_view;

    CampfireOptionItem *m_restOption;
    CampfireOptionItem *m_smithOption;
    CampfireOptionItem *m_liftOption;

    QGraphicsTextItem *m_titleItem;

    QWidget* m_overlay;

    void setupScene();
    void checkAvailableOptions();
};

class UpgradeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UpgradeDialog(GamePlay *gamePlay, QWidget *parent = nullptr);

    bool cardWasUpgraded() const;
    int upgradedCardID() const;

private slots:
    void onSceneItemClicked(QGraphicsItem *item);

private:
    QGraphicsScene *m_scene;
    CampfireGraphicsView *m_view;

    QVector<Card *> m_selectableCards;

    bool m_cardWasUpgraded = false;
    int m_upgradedCardID = -1;
};

#endif // CAMPFIRE_H
