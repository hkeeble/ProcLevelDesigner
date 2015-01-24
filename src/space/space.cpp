#include "space.h"

static constexpr int SIDE_MAP_TRANSPORTER_SIZE = 16; /*!< The size that every side map transporter must be. */

Space::Space()
{
    observer = new SpaceObserver();
}

Space::~Space()
{

}

void Space::generate(const Mission& mission)
{
    // Create grid size here
    int gridSize = 10;
    for(int x = 0; x < gridSize; x++)
    {
        cells.push_back(QVector<GridCell>());
        for(int y = 0; y < gridSize; y++)
        {
            cells[x].push_back(GridCell());
        }
    }

    QVector<QVector<Cell>> tileGrid = QVector<QVector<Cell>>(AREA_TILE_SIZE*2);
    for(int x = 0; x < tileGrid.length(); x++)
        tileGrid[x] = QVector<Cell>(AREA_TILE_SIZE*2);

    for(int x = 0; x < AREA_TILE_SIZE*2; x++)
    {
        for(int y = 0; y < AREA_TILE_SIZE*2; y++)
            if((qrand() % ((100 + 1) - 0) + 0) > 80)
                tileGrid[x][y] = Cell(QPoint(x, y), false);
            else
                tileGrid[x][y] = Cell(QPoint(x, y), true);
    }

    Key* switchTest = new Key("switchTest", Key::Type::Switch, "Test");

    placeArea(Area(&zones.find("field").value(), QPoint(0,0), 2, 2));
    areas.last().setGrid(tileGrid);
    placeArea(Area(&zones.find("field").value(), QPoint(0,2), 2, 2));
    areas.last().setGrid(tileGrid);
    placeArea(Area(&zones.find("field").value(), QPoint(2,0), 2, 2));
    areas.last().setGrid(tileGrid);
    placeArea(Area(&zones.find("field").value(), QPoint(2,2), 2, 2));
    areas.last().setGrid(tileGrid);
    areas.last().addKeyEvent(switchTest);

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
        Link link = Link::Parse(obj, gates);
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
        Area* first = &areas->find(link.first).value();
        Area* second = &areas->find(link.second).value();

        LinkDirection direction;

        // Decide on the link direction (respective to first area)
        if(link.first.x() < link.second.x())
            direction = LinkDirection::Right;
        else if(link.first.x() > link.second.x())
            direction = LinkDirection::Left;
        else if(link.first.y() < link.second.y())
            direction = LinkDirection::Up;
        else if(link.first.y() > link.second.y())
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
        Area area = areas.find(QPoint(x,y)).value();

        for(int i = x; i < area.getWidth(); i++)
        {
            for(int j = y; j < area.getHeight(); j++)
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
}

QList<Map> Space::buildMaps()
{
    // Build all areas into maps.
    QMap<QPoint, Map> maps = QMap<QPoint, Map>();
    for(auto iter = areas.begin(); iter != areas.end(); iter++)
    {
        maps.insert(iter.key(), iter.value().buildMap());
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
                if(x - 1 > 0)
                    left = &cells[x-1][y];
                if(y - 1 > 0)
                    up = &cells[x][y-1];
                if(x + 1 < getWidth())
                    right = &cells[x+1][y];
                if(y + 1 < getHeight())
                    down = &cells[x][y+1];

                QVector<MapEntity*> transporters = QVector<MapEntity*>(); // collection of transporters that need adding to the map

                // If the there is a valid link to a different area, create a valid teletransporter to link the two cells
                if(left /*&& area.getLinkLeft() != nullptr*/ && left->getAreaOrigin() != area.getLocation())
                {
                    Map* destination = &maps.find(left->getAreaOrigin()).value();
                    int yPos = (AREA_TILE_SIZE*map->getTileSize()) * (y - area.getLocation().y());
                    int xPos = -SIDE_MAP_TRANSPORTER_SIZE;
                    transporters.append(new Teletransporter(xPos, yPos, SIDE_MAP_TRANSPORTER_SIZE, (AREA_TILE_SIZE*SIDE_MAP_TRANSPORTER_SIZE)*map->getTileSize(), destination->getName(), "_side",
                                                                  Teletransporter::Transition::Scroll));
                }
                if(right /*&& area.getLinkRight() != nullptr*/ && right->getAreaOrigin() != area.getLocation())
                {
                    Map* destination = &maps.find(right->getAreaOrigin()).value();
                    int yPos = (AREA_TILE_SIZE*map->getTileSize()) * (y - area.getLocation().y());
                    int xPos = (map->getWidth()*map->getTileSize());
                    transporters.append(new Teletransporter(xPos, yPos, SIDE_MAP_TRANSPORTER_SIZE, (AREA_TILE_SIZE*SIDE_MAP_TRANSPORTER_SIZE)*map->getTileSize(), destination->getName(), "_side",
                                                                  Teletransporter::Transition::Scroll, "entities/teletransporter"));
                }
                if(down /*&& area.getLinkDown() != nullptr*/ && down->getAreaOrigin() != area.getLocation())
                {
                    Map* destination = &maps.find(down->getAreaOrigin()).value();
                    int yPos = (map->getHeight()*map->getTileSize());
                    int xPos = (AREA_TILE_SIZE*map->getTileSize()) * (x - area.getLocation().x());
                    transporters.append(new Teletransporter(xPos, yPos, (AREA_TILE_SIZE*SIDE_MAP_TRANSPORTER_SIZE)*map->getTileSize(), SIDE_MAP_TRANSPORTER_SIZE, destination->getName(), "_side",
                                                                  Teletransporter::Transition::Scroll));
                }
                if(up /*&& area.getLinkUp() != nullptr*/ && up->getAreaOrigin() != area.getLocation())
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
