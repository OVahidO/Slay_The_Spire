#include "map.h"
#include "mapButton.h"
#include "ui_map.h"

int rand(int min, int max, std::mt19937& range)
{
    std::uniform_int_distribution<int> dist(min , max);
    return dist(range);
}

Map::Map(unsigned int seed, QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::Map)
{
    ui->setupUi(this);
    m_seed = seed;
    m_randRange.seed(m_seed);
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
            for(int j = 0; j<(rand(0, 1, m_randRange)+2); j++)
            {
                MapButton* b = new MapButton(MapButtonType::ENEMY, i, j);
                b->setEnabled(true);
                level.append(b);
            }
        }
        else if(i < 5)
        {
            for(int j = 0; j < rand(0, 3, m_randRange)+1; j++)
            {
                int l = rand(1, 100, m_randRange);
                if(l <= 90 - ((i-1)*15))
                {
                    if(rand(1, 100, m_randRange) <= 65)
                        level.append(new MapButton(MapButtonType::ENEMY, i, j));
                    else
                        level.append(new MapButton(MapButtonType::EVENT, i, j));
                }
                else if(l <= 100 - ((i-1)*15))
                {
                    level.append(new MapButton(MapButtonType::ELITE, i, j));
                }
                else
                {
                    if(i != 4)
                    {
                        MapButtonType t = static_cast<MapButtonType>(rand(0, 1, m_randRange) + 3);
                        level.append(new MapButton(t, i, j));
                    }
                    else
                    {
                        bool isReapet = false;
                        if(rand(1, 100, m_randRange) <= 75)
                        {
                            for(auto& mapButton : m_levels[i-1])
                            {
                                if(mapButton->buttonType() == MapButtonType::CAMPFIRE)
                                {
                                    isReapet = true;
                                    break;
                                }
                            }

                            if(!isReapet)
                                level.append(new MapButton(MapButtonType::CAMPFIRE, i, j));
                            else
                            {
                                MapButtonType t = static_cast<MapButtonType>(rand(0, 1, m_randRange));
                                level.append(new MapButton(t, i, j));
                            }
                        }
                        else
                        {
                            for(auto& mapButton : m_levels[i-1])
                            {
                                if(mapButton->buttonType() == MapButtonType::SHOP)
                                {
                                    isReapet = true;
                                    break;
                                }
                            }

                            if(!isReapet)
                                level.append(new MapButton(MapButtonType::SHOP, i, j));
                            else
                            {
                                MapButtonType t = static_cast<MapButtonType>(rand(0, 1, m_randRange));
                                level.append(new MapButton(t, i, j));
                            }
                        }
                    }
                }
            }
        }
        else if(i > 5)
        {
            for(int j = 0; j < rand(0, 3, m_randRange)+1; j++)
            {
                if(i == 9)
                    level.append(new MapButton(MapButtonType::CAMPFIRE, i ,j));
                else
                {
                    int l = rand(1, 100, m_randRange);
                    if(l <= 50)
                        level.append(new MapButton(MapButtonType::ELITE, i ,j));
                    else if(l <= 10+(i*10))
                    {
                        bool isReapet = false;
                        if(rand(1, 100, m_randRange) <= 50)
                        {
                            for(auto& mapButton : m_levels[i-1])
                            {
                                if(mapButton->buttonType() == MapButtonType::CAMPFIRE)
                                {
                                    isReapet = true;
                                    break;
                                }
                            }

                            if(!isReapet)
                                level.append(new MapButton(MapButtonType::CAMPFIRE, i, j));
                            else
                            {
                                MapButtonType t = static_cast<MapButtonType>(rand(0, 1, m_randRange));
                                level.append(new MapButton(t, i, j));
                            }
                        }
                        else
                        {
                            for(auto& mapButton : m_levels[i-1])
                            {
                                if(mapButton->buttonType() == MapButtonType::SHOP)
                                {
                                    isReapet = true;
                                    break;
                                }
                            }

                            if(!isReapet)
                                level.append(new MapButton(MapButtonType::SHOP, i, j));
                            else
                            {
                                MapButtonType t = static_cast<MapButtonType>(rand(0, 1, m_randRange));
                                level.append(new MapButton(t, i, j));
                            }
                        }
                    }
                    else
                    {
                        MapButtonType t = static_cast<MapButtonType>(rand(0, 1, m_randRange));
                        level.append(new MapButton(t, i, j));
                    }
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
                if(targetIndex-1 >= 0 && rand(1, 100, m_randRange) <= 20 )
                {
                    currentLevel[j]->nextButtons().append(nextLevel[targetIndex-1]);
                }
                if(targetIndex+1 < nextSize && rand(1, 100, m_randRange) <= 20 )
                {
                    currentLevel[j]->nextButtons().append(nextLevel[targetIndex+1]);
                }
            }
            else
            {
                if(!currentLevel[targetIndex]->nextButtons().contains(nextLevel[j]))
                    currentLevel[targetIndex]->nextButtons().append(nextLevel[j]);
                if(j-1 >= 0 && rand(1, 100, m_randRange) <= 20 )
                {
                    currentLevel[targetIndex]->nextButtons().append(nextLevel[j-1]);
                }
                if(j+1 < nextSize && rand(1, 100, m_randRange) <= 20 )
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
            connect(currentMapButtons, &MapButton::onClick, this, [this](MapButton *mapButton) {
                if (m_locked)
                    return;

                auto sameLevelButtons = this->m_levels[mapButton->levelIndex()];
                for(auto& mapButton : sameLevelButtons)
                    mapButton->setEnabled(false);
            });
            connect(currentMapButtons, &MapButton::onClick, this, &Map::nodeSelected);

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

void Map::enableLevel(int levelIndex)
{
    if (levelIndex < 0 || levelIndex >= m_levels.size())
        return;

    for (MapButton *button : m_levels[levelIndex])
        button->setEnabled(true);
}

unsigned int Map::seed() const
{
    return m_seed;
}

void Map::setLocked(bool locked)
{
    m_locked = locked;
}
