#include "map.h"
#include "mapButton.h"
#include "ui_map.h"

Map::Map(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::Map)
{
    ui->setupUi(this);
    srand(std::time(0));
    ////////////////////////
    m_scene = new QGraphicsScene(this);
    m_view = ui->graphicsView;

    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform);

    m_view->setScene(m_scene);
    setBackground();

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ///////////////////////
    for(int i=0; i<11; i++)
    {
        QVector<MapButton*> level;
        level.clear();
        if(i == 10)
            level.append(new MapButton(MapButtonType::BOSS, i, 0));
        else if(i == 5)
            level.append(new MapButton(MapButtonType::TREASURE, i, 0));
        else if(i == 0)
        {
            for(int j = 0; j<((rand()%2)+3); j++)
            {
                MapButton* b = new MapButton(MapButtonType::ENEMY, i, j);
                b->setEnabled(true);
                level.append(b);
            }
        }
        else if(i < 5)
        {
            for(int j = 0; j<((rand()%4)+1); j++)
            {
                MapButtonType t = static_cast<MapButtonType>(rand()%(i+1));
                level.append(new MapButton(t, i, j));
            }
        }
        else if(i > 5)
        {
            for(int j = 0; j<((rand()%4)+1); j++)
            {
                int l = (rand()%100 + 1);
                if(l <= 50)
                    level.append(new MapButton(MapButtonType::ELITE, i ,j));
                else if(l <= 10+(i*10))
                {
                    MapButtonType t = static_cast<MapButtonType>(rand()%2 + 3);
                    level.append(new MapButton(t, i, j));
                }
                else
                {
                    MapButtonType t = static_cast<MapButtonType>(rand()%2);
                    level.append(new MapButton(t, i, j));
                }
            }
        }
        m_levels.append(level);
    }
    connectLevels();
    addMapButtons();
    addRoads();
}

Map::~Map()
{
    delete ui;
}

void Map::connectLevels()
{
    for(int i=0; i<m_levels.size()-1; i++)
    {
        auto currentLevel = m_levels[i];
        auto nextLevel = m_levels[i+1];

        int currentSize = currentLevel.size();
        int nextSize = nextLevel.size();

        for(int j=0; j<std::max(currentSize, nextSize); j++)
        {
            int targetIndex = (j*nextSize) / currentSize;
            targetIndex = qBound(0, targetIndex, currentSize-1);

            if(currentSize >= nextSize)
            {
                if(!currentLevel[j]->nextButtons().contains(nextLevel[targetIndex]))
                    currentLevel[j]->nextButtons().append(nextLevel[targetIndex]);
                if(targetIndex-1 >= 0 && rand()%100 < 20 )
                {
                    currentLevel[j]->nextButtons().append(nextLevel[targetIndex-1]);
                }
                if(targetIndex+1 < nextSize && rand()%100 < 20 )
                {
                    currentLevel[j]->nextButtons().append(nextLevel[targetIndex+1]);
                }
            }
            else
            {
                if(!currentLevel[targetIndex]->nextButtons().contains(nextLevel[j]))
                    currentLevel[targetIndex]->nextButtons().append(nextLevel[j]);
                if(j-1 >= 0 && rand()%100 < 20 )
                {
                    currentLevel[targetIndex]->nextButtons().append(nextLevel[j-1]);
                }
                if(j+1 < nextSize && rand()%100 < 20 )
                {
                    currentLevel[targetIndex]->nextButtons().append(nextLevel[j+1]);
                }
            }
        }
    }
}

void Map::setBackground()
{
    QPixmap map_Top(":Map/Pics/Map/mapTop.png");
    QPixmap map_Mid(":Map/Pics/Map/mapMid.png");
    //QPixmap map_Mid2(":Map/Pics/Map/mapMid2.png");
    QPixmap map_Bot(":Map/Pics/Map/mapBot.png");

    QGraphicsPixmapItem* topMap = m_scene->addPixmap(map_Top);
    QGraphicsPixmapItem* midMap = m_scene->addPixmap(map_Mid);
    //QGraphicsPixmapItem* mid2Map = m_scene->addPixmap(map_Mid2);
    QGraphicsPixmapItem* botMap = m_scene->addPixmap(map_Bot);

    topMap->setPos(-325, -50);
    midMap->setPos(-325, map_Top.height() - 50);
    //mid2Map->setPos(0, map_Top.height()+map_Mid.height());
    botMap->setPos(-325, map_Top.height()+map_Mid.height() - 50);

    topMap->setZValue(-2);
    midMap->setZValue(-2);
    //mid2Map->setZValue(-2);
    botMap->setZValue(-2);

    int totalW = map_Mid.width() - 650;
    int totalH = map_Top.height()+map_Mid.height()+map_Bot.height() - 100;
    m_scene->setSceneRect(0, 0, totalW, totalH);
}

void Map::addMapButtons()
{
    for(int i=0; i<m_levels.size(); i++)
    {
        auto currentLevel = m_levels[i];
        for(int j=0; j<currentLevel.size(); j++)
        {
            auto currentMapButtons = currentLevel[j];
            connect(currentMapButtons, &MapButton::onClick, this, [this](MapButton* mapButton)
            {
                auto sameLevelButtons = this->m_levels[mapButton->levelIndex()];
                for(auto& mapButton : sameLevelButtons)
                    mapButton->setEnabled(false);
            });

            int buttonX = (currentMapButtons->levelPosIndex()+1)*250 + 50 + rand()%100 - rand()%100;
            if(currentLevel.size() == 1)
                buttonX = 525;
            int buttonY = this->m_scene->height() - ((currentMapButtons->levelIndex()+1)*250);
            if(i==10)
                currentMapButtons->setPos(525, 300);
            else
                currentMapButtons->setPos(buttonX, buttonY);
            currentMapButtons->setZValue(2);
            m_scene->addItem(currentMapButtons);
        }
    }
}

void Map::addRoads()
{
    for (int i = 0; i < m_levels.size() - 1; ++i)
        for (int j = 0; j < m_levels[i].size(); ++j) {
            auto currentBtn = m_levels[i][j];
            for (MapButton* nextBtn : currentBtn->nextButtons())
            {
                qreal currentCenterX = currentBtn->pos().x() + (currentBtn->boundingRect().width() / 2);
                qreal currentCenterY = currentBtn->pos().y() + (currentBtn->boundingRect().height() / 2) - 20;
                qreal nextCenterX = nextBtn->pos().x() + (nextBtn->boundingRect().width() / 2);
                qreal nextCenterY = nextBtn->pos().y() + (nextBtn->boundingRect().height() / 2) + 20;


                QGraphicsLineItem* road = new QGraphicsLineItem(
                    currentCenterX, currentCenterY,
                    nextCenterX, nextCenterY
                    );

                QPen pen(QColor(85, 55, 35));
                pen.setWidth(3);
                pen.setStyle(Qt::DashLine);
                pen.setCapStyle(Qt::RoundCap);
                road->setPen(pen);

                road->setZValue(-1);

                m_scene->addItem(road);
                }
            }
}