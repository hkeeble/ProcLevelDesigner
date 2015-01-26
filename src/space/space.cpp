#include "space.h"

static constexpr int SIDE_MAP_TRANSPORTER_SIZE = 16; /*!< The size that every side map transporter must be. */

Space::Space()
{
    observer = new SpaceObserver();
}

Space::~Space()
{

}

// Temporary function - generates walls around an area in directions it is not linked
void generateWalls(Area& area)
{
    Grid* grid = area.getGrid();

    // Set values (wall off the area)
    for(int y = 0; y < grid->getHeight(); y++)
        grid->setCellTraversable(false, 0, y);
    for(int y = 0; y < grid->getHeight(); y++)
        grid->setCellTraversable(false, grid->getWidth()-1, y);
    for(int x = 0; x < grid->getWidth(); x++)
        grid->setCellTraversable(false, x, 0);
    for(int x = 0; x < grid->getWidth(); x++)
        grid->setCellTraversable(false, x, grid->getHeight()-1);

    // break walls where there are links
    if(area.getLinkRight())
        grid->setCellTraversable(true, grid->getWidth()-1, grid->getHeight()/2);
    if(area.getLinkLeft())
        grid->setCellTraversable(true, 0, grid->getHeight()/2);
    if(area.getLinkUp())
        grid->setCellTraversable(true, grid->getWidth()/2, 0);
    if(area.getLinkDown())
        grid->setCellTraversable(true, grid->getWidth()/2, grid->getHeight()-1);
}

void Space::generate(Mission& mission)
{
    clear();

    QList<Stage*> stages = mission.getStages(); // Mission stages
    QList<Area> newAreas; // Areas to be generated from stages

    Area* previousArea = nullptr; // Pointer to the previously generated area
    Stage* previousStage = nullptr;

    RandomEngine rand;
    Direction previousDirection = static_cast<Direction>(rand.randomInteger(0, 3));

    for(Stage* stage : stages)
    {
        Area area;
        Direction newDirection;

        // If this is the first stage to be generated
        if(previousArea == nullptr)
        {
            // Choose a random location
            QPoint point = QPoint(rand.randomInteger(0, 5), rand.randomInteger(0, 5));

            // Choose a random height and width
            int height = 1;// rand.randomInteger(1, 3);
            int width = 1;//rand.randomInteger(1, 3);

            area = Area(&zones.begin().value(), point, width, height);

            newDirection = static_cast<Direction>(rand.randomInteger(0, 3));
        }
        else
        {
            // Choose a random height and width
            int height = 1; // rand.randomInteger(1, 3);
            int width = 1; // rand.randomInteger(1, 3);

            QList<Direction> possibleDirections; // The possible directions the new area can lead into
            if(previousDirection != Direction::EAST && previousArea->getLocation().x() - width >= 0)
                possibleDirections.append(Direction::WEST);
            if(previousDirection != Direction::WEST)
                possibleDirections.append(Direction::EAST);
            if(previousDirection != Direction::SOUTH && previousArea->getLocation().y() - height >= 0)
                possibleDirections.append(Direction::NORTH);
            if(previousDirection != Direction::NORTH)
                possibleDirections.append(Direction::SOUTH);

            int x, y;

            // Choose a random new direction
            do {
                newDirection = possibleDirections[rand.randomInteger(0, possibleDirections.size()-1)];

                // Choose an appropriate position based on direction
                if(newDirection == Direction::WEST)
                {
                    x = previousArea->getLocation().x()-width;
                    y = previousArea->getLocation().y();
                }
                else if(newDirection == Direction::NORTH)
                {
                    x = previousArea->getLocation().x();
                    y = previousArea->getLocation().y()-height;
                }
                else if(newDirection == Direction::EAST)
                {
                    x = previousArea->getLocation().x() + previousArea->getWidth();
                    y = previousArea->getLocation().y();
                }
                else if(newDirection == Direction::SOUTH)
                {
                    x = previousArea->getLocation().x();
                    y = previousArea->getLocation().y() + previousArea->getHeight();
                }
            } while(areas.contains(QPoint(x, y)));

            area = Area(&zones.begin().value(), QPoint(x, y), width, height);
        }

        // Add keys
        for(Key* key : stage->getKeys())
            area.addKeyEvent(key, rand.randomInteger(1, area.getGridWidth()-1), rand.randomInteger(1, area.getGridHeight()-1));

        // Generate links and add gates
        if(previousArea)
        {
            if(newDirection == Direction::NORTH)
            {
                area.setLinkDown(Link(area.getLocation(), previousArea->getLocation()));
                previousArea->setLinkUp(Link(previousArea->getLocation(), area.getLocation()));

                previousArea->addGate(previousStage->getExitGate(), previousArea->getGridWidth()/2, 0);
            }
            else if(newDirection == Direction::SOUTH)
            {
                area.setLinkUp(Link(area.getLocation(), previousArea->getLocation()));
                previousArea->setLinkDown(Link(previousArea->getLocation(), area.getLocation()));

                previousArea->addGate(previousStage->getExitGate(), previousArea->getGridWidth()/2, previousArea->getGridHeight()-1);
            }
            else if(newDirection == Direction::WEST)
            {
                area.setLinkRight(Link(area.getLocation(), previousArea->getLocation()));
                previousArea->setLinkLeft(Link(previousArea->getLocation(), area.getLocation()));

                previousArea->addGate(previousStage->getExitGate(), 0, previousArea->getGridHeight()/2);
            }
            else if(newDirection == Direction::EAST)
            {
                area.setLinkLeft(Link(area.getLocation(), previousArea->getLocation()));
                previousArea->setLinkRight(Link(previousArea->getLocation(), area.getLocation()));

                previousArea->addGate(previousStage->getExitGate(), previousArea->getGridWidth()-1, previousArea->getGridHeight()/2);
            }
        }

        newAreas.append(area);
        previousDirection = newDirection;
        previousArea = &newAreas.last();
        previousStage = stage;
    }



    for(Area& area : newAreas)
    {
        generateWalls(area);
        placeArea(area);
    }

    setStartingArea(newAreas[0].getLocation(), QPoint(8, 8));

    emitUpdate();
}

Space Space::Parse(Table* data, QList<Gate*> gates, QList<Key*> keys, QList<Tileset*> tilesets)
{
    Space space;

    // Read in all zones
    QList<Object*> zoneObjects = data->getObjectsOfName(OBJ_ZONE);
    for(Object* obj : zoneObjects)
    {
        Zone zone = Zone::Parse(obj, tilesets);
        space.zones.insert(zone.getName(), zone);
    }

    // Read in all areas (areas subvert the quest data system, they must load their cells from a table that they manage,
    //                      therefore they are given the filepath of the space data to use as a basis for loading this table)
    QList<Object*> areaObjects = data->getObjectsOfName(OBJ_AREA);
    for(Object* obj : areaObjects)
    {
        space.placeArea(Area::Parse(obj, QFileInfo(data->getFilePath()).absoluteDir().absolutePath(), keys, gates));
    }

    // Read in all links
    QList<Link> linkList;
    QList<Object*> linkObjects = data->getObjectsOfName(OBJ_LINK);
    for(Object* obj : linkObjects)
    {
        Link link = Link::Parse(obj);
        linkList.append(link);
    }

    // Point areas to their zones
    QMap<QPoint,Area>* areas = space.getAreas();
    for(auto iter = areas->begin(); iter != areas->end(); iter++)
    {
        Zone* zone = &space.zones.find(iter.value().getZoneName()).value();
        if(zone)
            iter.value().setZone(zone);
    }

    // Place all links into their correct areas
    for(Link& link : linkList)
    {
        Area* first = &areas->find(link.getOrigin()).value();
        Area* second = &areas->find(link.getTarget()).value();

        LinkDirection direction;

        // Decide on the link direction (respective to first area)
        if(link.getOrigin().x() < link.getTarget().x())
            direction = LinkDirection::Right;
        else if(link.getOrigin().x() > link.getTarget().x())
            direction = LinkDirection::Left;
        else if(link.getOrigin().y() < link.getTarget().y())
            direction = LinkDirection::Up;
        else if(link.getOrigin().y() > link.getTarget().y())
            direction = LinkDirection::Down;

        switch(direction)
        {
        case LinkDirection::Right:
            first->setLinkRight(link);
            second->setLinkLeft(link);
            break;
        case LinkDirection::Left:
            first->setLinkLeft(link);
            second->setLinkRight(link);
            break;
        case LinkDirection::Up:
            first->setLinkUp(link);
            second->setLinkDown(link);
            break;
        case LinkDirection::Down:
            first->setLinkDown(link);
            second->setLinkUp(link);
            break;
        }
    }

    return space;
}

void Space::build(Table *data)
{
    data->clear(); // Clear any existing data

    // Build all zones
    for(QMap<QString,Zone>::iterator iter = zones.begin(); iter != zones.end(); iter++)
    {
        Object obj;
        iter.value().build(&obj);
        data->addObject(OBJ_ZONE, obj);
    }

    // Build all areas and compile a list of the links that need building. As with parsing,
    // areas currently subvert the quest data system as they manage their own tables for cell
    // data. Therefore the location of the space data is given to the build function in order
    // to manage this.
    QList<Link> linkList;
    for(auto iter = areas.begin(); iter != areas.end(); iter++)
    {
        Object obj;
        iter.value().build(&obj, QFileInfo(data->getFilePath()).absoluteDir().absolutePath());
        data->addObject(OBJ_AREA, obj);

        Link* right = iter.value().getLinkRight();
        Link* left = iter.value().getLinkLeft();
        Link* down = iter.value().getLinkDown();
        Link* up = iter.value().getLinkUp();

        if(right != nullptr && !linkList.contains(*right))
            linkList.append(*right);
        if(left != nullptr && !linkList.contains(*left))
            linkList.append(*left);
        if(down != nullptr && !linkList.contains(*down))
            linkList.append(*down);
        if(up != nullptr && !linkList.contains(*up))
            linkList.append(*up);
    }

    // Build all links
    for(Link link : linkList)
    {
        Object obj;
        link.build(&obj);
        data->addObject(OBJ_LINK, obj);
    }
}

bool Space::addZone(QString name, Zone zone)
{
    if(!zones.contains(name))
    {
        zones.insert(name, zone);
        return true;
    }
    else
        return false;
}

bool Space::removeZone(QString name)
{
    if(zones.contains(name))
    {
        zones.remove(name);
        return true;
    }
    else
        return false;
}

Zone* Space::getZone(QString name)
{
    auto iter = zones.find(name);
    if(iter == zones.end())
        return nullptr;
    else
        return &iter.value();
}

QList<Zone*> Space::getZoneList()
{
    QList<Zone*> zoneList;
    for(QMap<QString,Zone>::iterator iter = zones.begin(); iter != zones.end(); iter++)
        zoneList.append(&iter.value());
    return zoneList;
}

void Space::placeArea(Area area)
{
    // Ensure there is enough room in the grid for the new area.
    // - Expand X direction
    if(cells.length() < area.getLocation().x() + area.getWidth())
    {
        while(cells.length() < area.getLocation().x() + area.getWidth())
        {
            cells.append(QVector<GridCell>());
            while(cells[0].length() > cells.last().length())
                cells.last().append(GridCell());
        }
    }
    // - Expand Y direction
    if(cells[0].length() < area.getLocation().y() + area.getHeight())
    {
        for(int x = 0; x < cells.length(); x++)
        {
            while(cells[x].length() < area.getLocation().y() + area.getHeight())
                cells[x].append(GridCell());
        }
    }

    for(int i = area.getLocation().x(); i < area.getLocation().x() + area.getWidth(); i++)
    {
        for(int j = area.getLocation().y(); j < area.getLocation().y() + area.getHeight(); j++)
        {
            // Remove any areas in the way of this one
            if(cells[i][j].containsArea())
                removeArea(cells[i][j].getAreaOrigin());

            // Assign the cell to the correct area ID:
            cells[i][j] = GridCell(area.getLocation());
        }
    }

    // Insert area into area map
    areas.insert(area.getLocation(), area);
}

bool Space::removeArea(int x, int y)
{
    if(cells[x][y].containsArea())
    {
        Area* area = &areas.find(QPoint(x,y)).value();

        for(int i = x; i < area->getWidth(); i++)
        {
            for(int j = y; j < area->getHeight(); j++)
            {
                cells[i][j].removeArea();
            }
        }

        areas.remove(QPoint(x,y));

        return true;
    }

    return false;
}

bool Space::removeArea(Area area)
{
    for(auto iter = areas.begin(); iter != areas.end(); iter++)
    {
        if(iter.value() == area)
        {
            areas.remove(iter.key());
            return true;
        }
    }

    return false;
}

GridCell* Space::getCell(int x, int y)
{
    if(x < cells.length() && y < cells[x].length())
        return &cells[x][y];

    return nullptr;
}

void Space::clear()
{
    areas.clear();
    for(int x = 0; x < cells.size(); x++)
    {
        for(int y = 0; y < cells[0].size(); y++)
        {
            cells[x][y] = GridCell();
        }
    }
}

bool Space::setStartingArea(QPoint areaOrigin, QPoint position)
{
    // Ensure this is a valid starting area before assigning it
    if(!areas.contains(areaOrigin))
        return false;

    Area* area = &areas.find(areaOrigin).value();

    if(position.x() < 0 && position.y() < 0 &&
            position.x() > area->getWidth() * AREA_TILE_SIZE && position.y() > area->getHeight() * AREA_TILE_SIZE)
        return false;

    startingArea = areaOrigin;
    startingLocation = position;

    return true;
}

QList<Map> Space::buildMaps()
{
    // Build all areas into maps.
    QMap<QPoint, Map> maps = QMap<QPoint, Map>();
    for(auto iter = areas.begin(); iter != areas.end(); iter++)
    {
        maps.insert(iter.key(), iter.value().buildMap());

        Map* map = &maps.last();

        if(iter.key() == startingArea)
            map->addEntity(new Destination(0, startingLocation.x() * map->getTileSize(), startingLocation.y() * map->getTileSize(), "start", Direction::EAST));
    }

    // Link all maps together by looping grid cells
    for(int x = 0; x < getWidth(); x++)
    {
        for(int y = 0; y < getHeight(); y++)
        {
            if(cells[x][y].containsArea())
            {
                // Retrieve the area and map associated with the area (by using area location).
                Area area = areas.find(cells[x][y].getAreaOrigin()).value();
                Map* map = &maps.find(area.getLocation()).value();

                // Retrieve all valid adjacent cells
                GridCell* left = nullptr;
                GridCell* right = nullptr;
                GridCell* up = nullptr;
                GridCell* down = nullptr;
                if(x - 1 >= 0)
                    left = &cells[x-1][y];
                if(y - 1 >= 0)
                    up = &cells[x][y-1];
                if(x + 1 < getWidth())
                    right = &cells[x+1][y];
                if(y + 1 < getHeight())
                    down = &cells[x][y+1];

                QVector<MapEntity*> transporters = QVector<MapEntity*>(); // collection of transporters that need adding to the map

                // If the there is a valid link to a different area, create a valid teletransporter to link the two cells
                if(left && left->containsArea() && area.getLinkLeft() != nullptr && left->getAreaOrigin() != area.getLocation())
                {
                    Map* destination = &maps.find(left->getAreaOrigin()).value();
                    int yPos = (AREA_TILE_SIZE*map->getTileSize()) * (y - area.getLocation().y());
                    int xPos = -SIDE_MAP_TRANSPORTER_SIZE;
                    transporters.append(new Teletransporter(xPos, yPos, SIDE_MAP_TRANSPORTER_SIZE, (AREA_TILE_SIZE*SIDE_MAP_TRANSPORTER_SIZE)*map->getTileSize(), destination->getName(), "_side",
                                                                  Teletransporter::Transition::Scroll));
                }
                if(right && right->containsArea() && area.getLinkRight() != nullptr  && right->getAreaOrigin() != area.getLocation())
                {
                    Map* destination = &maps.find(right->getAreaOrigin()).value();
                    int yPos = (AREA_TILE_SIZE*map->getTileSize()) * (y - area.getLocation().y());
                    int xPos = (map->getWidth()*map->getTileSize());
                    transporters.append(new Teletransporter(xPos, yPos, SIDE_MAP_TRANSPORTER_SIZE, (AREA_TILE_SIZE*SIDE_MAP_TRANSPORTER_SIZE)*map->getTileSize(), destination->getName(), "_side",
                                                                  Teletransporter::Transition::Scroll, "entities/teletransporter"));
                }
                if(down && down->containsArea() && area.getLinkDown() != nullptr  && down->getAreaOrigin() != area.getLocation())
                {
                    Map* destination = &maps.find(down->getAreaOrigin()).value();
                    int yPos = (map->getHeight()*map->getTileSize());
                    int xPos = (AREA_TILE_SIZE*map->getTileSize()) * (x - area.getLocation().x());
                    transporters.append(new Teletransporter(xPos, yPos, (AREA_TILE_SIZE*SIDE_MAP_TRANSPORTER_SIZE)*map->getTileSize(), SIDE_MAP_TRANSPORTER_SIZE, destination->getName(), "_side",
                                                                  Teletransporter::Transition::Scroll));
                }
                if(up && up->containsArea() && area.getLinkUp() != nullptr && up->getAreaOrigin() != area.getLocation())
                {
                    Map* destination = &maps.find(up->getAreaOrigin()).value();
                    int yPos = -SIDE_MAP_TRANSPORTER_SIZE;
                    int xPos = (AREA_TILE_SIZE*map->getTileSize()) * (x - area.getLocation().x());
                    transporters.append(new Teletransporter(xPos, yPos, (AREA_TILE_SIZE*SIDE_MAP_TRANSPORTER_SIZE)*map->getTileSize(), SIDE_MAP_TRANSPORTER_SIZE, destination->getName(), "_side",
                                                                  Teletransporter::Transition::Scroll));
                }

                // Add all transporters to the map.
                for(MapEntity* transporter : transporters)
                    map->addEntity(transporter);
            }
        }
    }

    QList<Map> mapList = QList<Map>();
    for(auto iter = maps.begin(); iter != maps.end(); iter++)
    {
        mapList.append(iter.value());
    }
    return mapList;
}
