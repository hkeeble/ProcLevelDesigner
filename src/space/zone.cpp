#include "zone.h"

Zone::Zone()
{
    tileset = nullptr;
}

Zone::Zone(QString name, int areaCount, Tileset *tileset)
    : Zone()
{
    this->name = name;
    this->areaCount = areaCount;
    this->tileset = tileset;
}

Zone Zone::Parse(Object* data, QList<Tileset*> tilesets)
{
    Zone zone;

    zone.name = data->find(ELE_NAME, NULL_ELEMENT);
    zone.areaCount = data->find(ELE_AREA_COUNT, "1").toInt();

    QString tilesetName = data->find(ELE_TILESET, NULL_ELEMENT);
    if(tilesetName != NULL_ELEMENT)
    {
        for(Tileset* tileset : tilesets)
        {
            if(tileset->getName() == tilesetName)
                zone.tileset = tileset;
        }
    }

    return zone;
}

void Zone::build(Object* object)
{
    object->data.clear(); // Clear any existing data

    object->insert(ELE_NAME, name);
    object->insert(ELE_TILESET, tileset->getName());
    object->insert(ELE_AREA_COUNT, QString::number(areaCount));
}

Zone::~Zone()
{

}
