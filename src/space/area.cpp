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

    return area;
}

void Area::build(Object* obj)
{
    obj->data.clear(); // Clear any existing data

    obj->insert(ELE_X, QString::number(location.x()));
    obj->insert(ELE_Y, QString::number(location.y()));

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
