#include "area.h"

Area::Area()
{
    up = left = right = down = nullptr;
    zone = nullptr;
}

Area::~Area()
{
    clearAllLinks();
}

Area::Area(const Area& param)
{
    cpy(param);
}

Area& Area::operator=(const Area& param)
{
    if(this == &param)
        return *this;
    else
    {
        cpy(param);
        return *this;
    }
}

Area::Area(Zone* zone, QPoint location, int width, int height, QList<Key*> keyEvents)
    : Area()
{
    this->zone = zone;
    this->location = location;
    this->keyEvents = keyEvents;
    this->width = width;
    this->height = height;
    this->grid = QVector<QVector<bool>>(width*AREA_TILE_SIZE);

    for(int x = 0; x < width*AREA_TILE_SIZE; x++)
        this->grid[x] = QVector<bool>(height*AREA_TILE_SIZE);

    for(int x = 0; x < width*AREA_TILE_SIZE; x++)
    {
        for(int y = 0; y < height*AREA_TILE_SIZE; y++)
        {
            this->grid[x][y] = true;
        }
    }

    this->zoneName = zone->getName();
}

Area Area::Parse(Object* obj, QList<Key*> keys)
{
    Area area;
    int x = obj->find(ELE_X, "0").toInt();
    int y = obj->find(ELE_Y, "0").toInt();
    area.location = QPoint(x, y);

    QStringList keyList = obj->find(ELE_KEYS, NULL_ELEMENT).split(LIST_DELIM);

    // Search through list of keys, apply them
    for(Key* key : keys)
    {
        if(keyList.contains(key->getName()))
            area.keyEvents.append(key);
    }

    area.zoneName = obj->find(ELE_ZONE, NULL_ELEMENT);

    area.width = obj->find(ELE_WIDTH, "1").toInt();
    area.height = obj->find(ELE_HEIGHT, "1").toInt();

    return area;
}

void Area::build(Object* obj)
{
    obj->data.clear(); // Clear any existing data

    obj->insert(ELE_X, QString::number(location.x()));
    obj->insert(ELE_Y, QString::number(location.y()));

    obj->insert(ELE_WIDTH, QString::number(width));
    obj->insert(ELE_HEIGHT, QString::number(height));

    QString keyList = "";
    for(Key* key : keyEvents)
        keyList += key->getName() + LIST_DELIM;
    keyList.remove(keyList.length()-1, 1);

    obj->insert(ELE_KEYS, keyList);

    if(zone)
        obj->insert(ELE_ZONE, zone->getName());
}

void Area::cpy(const Area& param)
{
    this->zoneName = param.zoneName;
    this->location = param.location;
    this->keyEvents = param.keyEvents;
    this->zone = param.zone;
    this->width = param.width;
    this->height = param.height;
    this->grid = param.grid;

    this->right = this->left = this->down = this->up = nullptr;

    if(param.right)
        this->right = new Link(*param.right);
    if(param.left)
        this->left = new Link(*param.left);
    if(param.down)
        this->down = new Link(*param.down);
    if(param.up)
        this->up = new Link(*param.up);
}

void Area::clearAllLinks()
{
    removeRightLink();
    removeLeftLink();
    removeDownLink();
    removeUpLink();
}

void Area::removeRightLink()
{
    if(right)
        delete right;

    right = nullptr;
}

void Area::removeLeftLink()
{
    if(left)
        delete left;

    left = nullptr;
}

void Area::removeDownLink()
{
    if(down)
        delete down;

    down = nullptr;
}

void Area::removeUpLink()
{
    if(up)
        delete up;

    up = nullptr;
}

Map Area::buildMap()
{
    Map map(QString::number(location.x()) + QString::number(location.y()), width, height, zone->getTileset()->getTileSize(), "village", "", zone->getTileset());

    for(int x = 0; x < map.getWidth(); x++)
    {
        for(int y = 0; y < map.getHeight(); y++)
        {
            MapTile tile = MapTile(0, x, y, map.getTileSize(), 0);
            map.setTile(x, y, MapTile(tile));
        }
    }

    return map;
}
