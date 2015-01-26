#include "spacescene.h"

SpaceScene::SpaceScene(QObject *parent) :
    QGraphicsScene(parent), wallColor(Qt::gray)
{
    areaCellWidth = 64.0f;
    areaCellHeight = 64.0f;
    gridCellWidth = areaCellWidth/AREA_TILE_SIZE;
    gridCellHeight = areaCellHeight/AREA_TILE_SIZE;
    gridParent = nullptr;
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
    for(QGraphicsRectItem* tile : blockedTiles)
        delete tile;

    for(QGraphicsRectItem* tile : areaTiles)
        delete tile;

    for(QVector<QGraphicsRectItem*> vector : gridTiles)
    {
        for(QGraphicsRectItem* tile : vector)
            delete tile;
    }

    for(QVector<QGraphicsRectItem*> vector : gridTiles)
        vector.clear();

    gridTiles.clear();
    areaTiles.clear();
    blockedTiles.clear();

    if(gridParent)
        delete gridParent;
}

void SpaceScene::spaceUpdated()
{
    clear(); // Clear existing tiles

    QMap<QPoint,Area>* areas = space->getAreas();

    // Update the grid
    gridParent = new QGraphicsRectItem(0, 0, space->getWidth()*areaCellWidth, space->getHeight()*areaCellHeight);
    gridParent->setPen(QPen(Qt::blue, 3));

    for(int x = 0; x < space->getWidth(); x++)
    {
        gridTiles.push_back(QVector<QGraphicsRectItem*>());
        for(int y = 0; y < space->getHeight(); y++)
        {
            gridTiles[x].push_back(new QGraphicsRectItem(x*areaCellWidth, y*areaCellHeight, areaCellWidth, areaCellHeight, gridParent));
            gridTiles[x].last()->setPen(QPen(Qt::blue, 1, Qt::DotLine));
        }
    }

    addItem(gridParent);

    // Update areas
    for(auto iter = areas->begin(); iter != areas->end(); iter++)
    {
        Area* area = &iter.value();

        // Get area data
        QPoint location = area->getLocation();
        int width = area->getWidth();
        int height = area->getHeight();
        Grid* grid = area->getGrid();

        // Create area tile
        QGraphicsRectItem* tile = new QGraphicsRectItem(location.x()*areaCellWidth, location.y()*areaCellHeight, areaCellWidth*width, areaCellHeight*height);
        tile->setBrush(area->getZone()->getColor());
        areaTiles.append(tile);

        // Create blocked tiles
        for(int x = 0; x < grid->getWidth(); x++)
        {
            for(int y = 0; y < grid->getHeight(); y++)
            {
                if(grid->getCell(x,y).isTraversable() == false)
                {
                    int xr = location.x()*areaCellWidth;
                    int yr = location.y()*areaCellHeight;
                    QGraphicsRectItem* gridTile = new QGraphicsRectItem((x*gridCellWidth) + xr, (y*gridCellHeight) + yr, gridCellWidth, gridCellHeight, tile);
                    gridTile->setBrush(QBrush(wallColor));
                    blockedTiles.append(gridTile);
                }
            }
        }

        addItem(tile);
    }
}
