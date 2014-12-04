#include "space.h"

Space::Space()
{
    observer = new SpaceObserver();
}

Space::~Space()
{

}

Space Space::Parse(Table* data)
{
    Space space;

    QList<Object*> zoneObjects = data->getObjectsOfName(OBJ_ZONE);

    for(Object* obj : zoneObjects)
    {
    }

    return space;
}

void Space::build(Table *data)
{

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

QList<Zone*> Space::getZoneList()
{
    QList<Zone*> zoneList;
    for(QMap<QString,Zone>::iterator iter = zones.begin(); iter != zones.end(); iter++)
        zoneList.append(&iter.value());
    return zoneList;
}
