#include "mapentity.h"

void MapEntity::Parse(MapEntity* mapEntity, Object* object)
{
    mapEntity->x = object->find(ELE_X, "0").toInt();
    mapEntity->y = object->find(ELE_Y, "0").toInt();
    mapEntity->layer = object->find(ELE_LAYER, "0").toInt();
}

void MapEntity::build(Object* object)
{
    object->insert(ELE_X, DataType::Integer, QString::number(x));
    object->insert(ELE_Y, DataType::Integer, QString::number(y));
    object->insert(ELE_LAYER, DataType::Integer, QString::number(layer));
}
