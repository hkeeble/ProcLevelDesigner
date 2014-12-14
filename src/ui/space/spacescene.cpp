#include "spacescene.h"

SpaceScene::SpaceScene(QObject *parent) :
    QGraphicsScene(parent)
{
    cellWidth = 64.0f;
    cellHeight = 64.0f;
}


void SpaceScene::setSpace(Space* space)
{
    this->space = space;
    QObject::connect(space->getObserver(), SIGNAL(updated()), this, SLOT(spaceUpdated()));
    spaceUpdated();
}

SpaceScene::~SpaceScene()
{
    clear();
}

void SpaceScene::clear()
{
    for(QGraphicsRectItem* tile : tiles)
        delete tile;

    tiles.clear();
}

void SpaceScene::spaceUpdated()
{
    clear(); // Clear existing tiles

    auto areas = space->getAreas();

    for(QMap<QPoint,Area>::iterator iter = areas->begin(); iter != areas->end(); iter++)
    {
        QPoint location = iter.value().getLocation();
        QGraphicsRectItem* tile = new QGraphicsRectItem(location.x()*cellWidth, location.y()*cellHeight, cellWidth*iter.value().getWidth(), cellHeight*iter.value().getHeight());
        tile->setBrush(iter.value().getZone()->getColor());
        tiles.append(tile);
    }

    for(QGraphicsRectItem* tile : tiles)
        addItem(tile);
}
