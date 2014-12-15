#include "space.h"

// Define a '<' operator for QPoint (so we can store it in a QMap)
bool operator<(const QPoint& lhs, const QPoint& rhs)
{
    if(lhs.x() < rhs.x())
        return true;
    else if (lhs.x() == rhs.x())
        return lhs.y() < rhs.y();
    else
        return false;
}

Space::Space()
{
    observer = new SpaceObserver();
}

Space::~Space()
{

}

void Space::generate(const Mission& mission)
{
    QVector<QVector<bool>> grid = QVector<QVector<bool>>(AREA_TILE_SIZE);
    for(int x = 0; x < grid.length(); x++)
        grid[x] = QVector<bool>(AREA_TILE_SIZE);

    for(int x = 0; x < AREA_TILE_SIZE; x++)
    {
        for(int y = 0; y < AREA_TILE_SIZE; y++)
            if((qrand() % ((100 + 1) - 0) + 0) > 50)
                grid[x][y] = false;
            else
                grid[x][y] = true;
    }

    areas.insert(QPoint(2,0),Area(&zones.find("field").value(), QPoint(2,0), 1, 1, QList<Key*>()));
    areas.last().setGrid(grid);
    areas.insert(QPoint(1,0),Area(&zones.find("field").value(), QPoint(1,0), 1, 2, QList<Key*>()));
    areas.insert(QPoint(2,1),Area(&zones.find("field").value(), QPoint(2, 1), 2, 2));
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

    // Read in all areas
    QList<Object*> areaObjects = data->getObjectsOfName(OBJ_AREA);
    for(Object* obj : areaObjects)
    {
        Area area = Area::Parse(obj, keys);
        space.areas.insert(area.getLocation(), area);
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
    for(Area& area : space.areas)
    {
        Zone* zone = &space.zones.find(area.getZoneName()).value();
        if(zone)
            area.setZone(zone);
    }

    // Place all links into their correct areas
    for(Link& link : linkList)
    {
        Area* first = &space.areas.find(link.first).value();
        Area* second = &space.areas.find(link.second).value();

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

    // Build all areas and compile a list of the links that need building
    QList<Link> linkList;
    for(QMap<QPoint,Area>::iterator iter = areas.begin(); iter != areas.end(); iter++)
    {
        Object obj;
        iter.value().build(&obj);
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
