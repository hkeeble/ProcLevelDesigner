#include "spacescene.h"

SpaceScene::SpaceScene(QStatusBar* statusBar, QObject *parent) :
    QGraphicsScene(parent), wallColor(Qt::gray), gateColor(Qt::yellow), keyColor(Qt::red)
{
    this->statusBar = statusBar;

    areaCellWidth = 64.0f;
    areaCellHeight = 64.0f;
    gridCellWidth = areaCellWidth/AREA_TILE_SIZE;
    gridCellHeight = areaCellHeight/AREA_TILE_SIZE;
    gridParent = nullptr;

    highlightedArea = nullptr;
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
    QGraphicsScene::clear();

    for(QVector<QGraphicsRectItem*> vector : gridTiles)
        vector.clear();

    gridTiles.clear();
    areaTiles.clear();
    blockedTiles.clear();
    keys.clear();
    gates.clear();
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
        QGraphicsRectItem* rect = new QGraphicsRectItem(location.x()*areaCellWidth, location.y()*areaCellHeight, areaCellWidth*width, areaCellHeight*height);
        rect->setBrush(area->getZone()->getColor());
        areaTiles.append(AreaTile(area, rect));
        AreaTile* tile = &areaTiles[areaTiles.count()-1];

        // Create blocked tiles
        for(int x = 0; x < grid->getWidth(); x++)
        {
            for(int y = 0; y < grid->getHeight(); y++)
            {
                if(grid->getCell(x,y).isTraversable() == false || grid->getCell(x,y).hasGate() || grid->getCell(x,y).hasKey() ||
                        (area->getLocation() == space->getStartingArea() && QPoint(x,y) == space->getStartingLocation()))
                {
                    int xr = location.x()*areaCellWidth;
                    int yr = location.y()*areaCellHeight;
                    QGraphicsRectItem* gridTile = new QGraphicsRectItem((x*gridCellWidth) + xr, (y*gridCellHeight) + yr, gridCellWidth, gridCellHeight, tile->getRect());

                    if(grid->getCell(x,y).isTraversable() == false)
                    {
                        gridTile->setBrush(QBrush(wallColor));
                        blockedTiles.append(gridTile);
                    }
                    else if (grid->getCell(x,y).hasGate())
                    {
                        gridTile->setBrush(QBrush(gateColor));
                        gates.append(GateTile(grid->getCell(x,y).getGate(), gridTile));
                    }
                    else if (grid->getCell(x,y).hasKey())
                    {
                        gridTile->setBrush(QBrush(keyColor));
                        keys.append(KeyTile(grid->getCell(x,y).getKey(), gridTile));
                    }
                    else if(area->getLocation() == space->getStartingArea() && QPoint(x,y) == space->getStartingLocation())
                    {
                        gridTile->setBrush(QBrush(Qt::blue));
                    }
                }
            }
        }

        addItem(tile->getRect());
    }
}

void SpaceScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    bool areaFound = false;
    for(AreaTile& area : areaTiles)
    {
        if(area.getRect()->contains(event->scenePos()))
        {
            highlightedArea = &area;
            area.getRect()->setBrush(QBrush(Qt::red));
            areaFound = true;

            statusBar->clearMessage();
            QString msg;
            msg += "Area:" + QString::number(area.getArea()->getLocation().x()) + ", " + QString::number(area.getArea()->getLocation().y());
            msg += " - Belongs to Stage: " + QString::number(area.getArea()->getStageID());
            msg += " - W: " + QString::number(area.getArea()->getWidth()) + " H: " + QString::number(area.getArea()->getHeight());

            if(area.getArea()->getKeyEvents().length() > 0)
            {
                msg += " Keys: ";
                for(Key* key : area.getArea()->getKeyEvents())
                {
                    msg += key->getName() + ", ";
                }
                msg.remove(msg.length()-2, 2);
            }

            statusBar->showMessage(msg);
        }
        else
            area.resetColor();
    }

    if(areaFound == false)
    {
        highlightedArea = nullptr;
        statusBar->clearMessage();
    }
}

void SpaceScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{

}
