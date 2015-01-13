#include "spacescene.h"

SpaceScene::SpaceScene(QObject *parent) :
    QGraphicsScene(parent), wallColor(Qt::gray)
{
    areaCellWidth = 64.0f;
    areaCellHeight = 64.0f;
    gridCellWidth = areaCellWidth/AREA_TILE_SIZE;
    gridCellHeight = areaCellHeight/AREA_TILE_SIZE;
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
    for(QGraphicsRectItem* tile : gridTiles)
        delete tile;

    for(QGraphicsRectItem* tile : areaTiles)
        delete tile;

    areaTiles.clear();
    gridTiles.clear();
}

void SpaceScene::spaceUpdated()
{
    clear(); // Clear existing tiles

    QList<Area>* areas = space->getAreas();

    for(Area& area : *areas)
    {
        // Get area data
        QPoint location = area.getLocation();
        int width = area.getWidth();
        int height = area.getHeight();
        QVector<QVector<Cell>> grid = area.getGrid();

        // Create area tile
        QGraphicsRectItem* tile = new QGraphicsRectItem(location.x()*areaCellWidth, location.y()*areaCellHeight, areaCellWidth*width, areaCellHeight*height);
        tile->setBrush(area.getZone()->getColor());
        areaTiles.append(tile);

        // Create grid tiles
        for(int x = 0; x < grid.length(); x++)
        {
            for(int y = 0; y < grid[0].length(); y++)
            {
                if(grid[x][y].isTraversable() == false)
                {
                    int xr = location.x()*areaCellWidth;
                    int yr = location.y()*areaCellHeight;
                    QGraphicsRectItem* gridTile = new QGraphicsRectItem((x*gridCellWidth) + xr, (y*gridCellHeight) + yr, gridCellWidth, gridCellHeight, tile);
                    gridTile->setBrush(QBrush(wallColor));
                    gridTiles.append(gridTile);
                }
            }
        }

        addItem(tile);
    }
}
