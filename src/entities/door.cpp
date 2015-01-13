#include "door.h"

const QString Door::NONE = "none";
const QString Door::INTERACTION = "interaction";
const QString Door::INTERACTION_IF_SAVEGAME_VARIABLE = "interaction_if_savegame_variable";
const QString Door::INTERACTION_IF_ITEM = "interaction_if_item";
const QString Door::EXPLOSION = "explosion";

Door Door::Parse(Object* object)
{
    Door door;

    // Parse base map entity data
    MapEntity::Parse(&door, object);

    door.direction = Direction(object->find(ELE_DIRECTION, "0").toInt());

    // Determine opening method
    QString method = object->find(ELE_OPENING_METHOD, NONE);
    if(method == NONE)
        door.openingMethod = OpeningMethod::None;
    else if(method == INTERACTION)
        door.openingMethod = OpeningMethod::Interaction;
    else if(method == EXPLOSION)
        door.openingMethod = OpeningMethod::Explosion;
    else if(method == INTERACTION_IF_ITEM)
        door.openingMethod = OpeningMethod::InteractionIfItem;
    else if(method == INTERACTION_IF_SAVEGAME_VARIABLE)
        door.openingMethod = OpeningMethod::InteractionIfSavegameVariable;

    door.sprite = object->find(ELE_SPRITE, "NULL");

    return door;
}

void Door::build(Object* object)
{
    // Build base map entity data
    MapEntity::build(object);

    object->insert(ELE_DIRECTION, DataType::Integer, QString::number(direction));

    if(openingMethod == OpeningMethod::None)
        object->insert(ELE_OPENING_METHOD, NONE);
    else if(openingMethod == OpeningMethod::Interaction)
        object->insert(ELE_OPENING_METHOD, INTERACTION);
    else if(openingMethod == OpeningMethod::Explosion)
        object->insert(ELE_OPENING_METHOD, EXPLOSION);
    else if(openingMethod == OpeningMethod::InteractionIfItem)
        object->insert(ELE_OPENING_METHOD, INTERACTION_IF_ITEM);
    else if(openingMethod == OpeningMethod::InteractionIfSavegameVariable)
        object->insert(ELE_OPENING_METHOD, INTERACTION_IF_SAVEGAME_VARIABLE);

    object->insert(ELE_SPRITE, sprite);
}
