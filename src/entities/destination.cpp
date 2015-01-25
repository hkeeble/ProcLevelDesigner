#include "destination.h"

Destination* Destination::clone()
{
    return new Destination(*this);
}

Destination Destination::Parse(Object* object)
{
    Destination destination;

    MapEntity::Parse(&destination, object);

    destination.direction = static_cast<Direction>(object->find(ELE_DIRECTION, "0").toInt());

    QString def = object->find(ELE_DEFAULT_DESTINATION, "false");
    if(def == "false")
        destination.defaultDestination = false;
    else
        destination.defaultDestination = true;

    return destination;
}

void Destination::build(Object* object)
{
    object->data.clear();

    MapEntity::build(object);

    object->insert(ELE_DIRECTION, QString::number(static_cast<int>(direction)));
    object->insert(ELE_DEFAULT_DESTINATION, DataType::Boolean, (defaultDestination ? "true" : "false"));
}
