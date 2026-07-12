#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include <random>

class MapButton;

QT_BEGIN_NAMESPACE
namespace Ui {
class Map;
}
QT_END_NAMESPACE

class Map : public QWidget
{
    Q_OBJECT
public:
    explicit Map(unsigned int seed, QWidget *parent = nullptr);
    ~Map();

signals:

private:
    Ui::Map *ui;
    QGraphicsView* m_view;
    QGraphicsScene* m_scene;
    void setBackground();
    void addMapButtons();
    void addRoads();

    QVector<QVector<MapButton*>> m_levels;
    void connectLevels();

    std::mt19937 m_randRange;
    unsigned int m_seed;
};

#endif // MAP_H
