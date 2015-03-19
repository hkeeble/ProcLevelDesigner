#include "zone.h"

Zone::Zone()
{
    tileset = nullptr;
    color = Qt::white;
}

Zone::Zone(QString name, Tileset *tileset, QColor color)
    : Zone()
{
    this->name = name;
    this->tileset = tileset;
    this->color = color;
}

Zone Zone::Parse(Object* data, QList<Tileset*> tilesets)
{
    Zone zone;

    zone.name = data->find(ELE_NAME, NULL_ELEMENT);

    // Read zone color
    int r = data->find(ELE_RED, "0").toInt();
    int g = data->find(ELE_GREEN, "0").toInt();
    int b = data->find(ELE_BLUE, "0").toInt();
    zone.color = QColor(r, g, b);

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

    // Build color
    int r, g, b;
    color.getRgb(&r, &g, &b);

    object->insert(ELE_RED, QString::number(r));
    object->insert(ELE_GREEN, QString::number(g));
    object->insert(ELE_BLUE, QString::number(b));
}

Zone::~Zone()
{

}
