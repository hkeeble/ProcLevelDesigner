#include "space.h"

Space::Space()
{
    observer = new SpaceObserver();
}

Space::~Space()
{

}

void Space::generate(const Mission& mission)
{
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

    grid.placeArea(Area(&zones.find("field").value(), QPoint(0,0), 2, 2, QList<Key*>()));
    grid.placeArea(Area(&zones.find("field").value(), QPoint(0,2), 2, 2, QList<Key*>()));
    grid.placeArea(Area(&zones.find("field").value(), QPoint(2,0), 2, 2));
    grid.placeArea(Area(&zones.find("field").value(), QPoint(2,2), 2, 2));

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
        space.grid.placeArea(Area::Parse(obj, QFileInfo(data->getFilePath()).absoluteDir().absolutePath(), keys, gates));
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
    QList<Area>* areas = space.getAreas();
    for(Area& area : *areas)
    {
        Zone* zone = &space.zones.find(area.getZoneName()).value();
        if(zone)
            area.setZone(zone);
    }

    // Place all links into their correct areas
    for(Link& link : linkList)
    {
        Area* first;
        Area* second;

        for(int i = 0; i < areas->length(); i++)
        {
            if((*areas)[i].getLocation() == link.first)
                first = &(*areas)[i];
            if((*areas)[i].getLocation() == link.second)
                second = &(*areas)[i];
        }

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
    QList<Area>* areas = grid.getAreas();
    for(Area& area : *areas)
    {
        Object obj;
        area.build(&obj, QFileInfo(data->getFilePath()).absoluteDir().absolutePath());
        data->addObject(OBJ_AREA, obj);

        Link* right = area.getLinkRight();
        Link* left = area.getLinkLeft();
        Link* down = area.getLinkDown();
        Link* up = area.getLinkUp();

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
