#include "map.h"
#include "mapButton.h"

Map::Map(QGraphicsItem *parent)
    : QGraphicsObject{parent}
{
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
            for(int j = 0; j<((rand()%4)+1); j++)
            {
                level.append(new MapButton(MapButtonType::ENEMY, i, j));
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
}

QRectF Map::boundingRect() const
{
    return QRectF(0, 0, 0, 0);
}

void Map::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_UNUSED(painter);
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
                if(targetIndex-1 >= 0 && rand()%100 < 30 )
                {
                    currentLevel[j]->nextButtons().append(nextLevel[targetIndex-1]);
                }
                if(targetIndex+1 < nextSize && rand()%100 < 30 )
                {
                    currentLevel[j]->nextButtons().append(nextLevel[targetIndex+1]);
                }
            }
            else
            {
                if(!currentLevel[targetIndex]->nextButtons().contains(nextLevel[j]))
                    currentLevel[targetIndex]->nextButtons().append(nextLevel[j]);
                if(j-1 >= 0 && rand()%100 < 30 )
                {
                    currentLevel[targetIndex]->nextButtons().append(nextLevel[j-1]);
                }
                if(j+1 < nextSize && rand()%100 < 30 )
                {
                    currentLevel[targetIndex]->nextButtons().append(nextLevel[j+1]);
                }
            }
        }
    }
}