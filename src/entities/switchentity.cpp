#include "switchentity.h"

const QString SwitchEntity::WALKABLE = "walkable";
const QString SwitchEntity::SOLID = "solid";
const QString SwitchEntity::ARROW_TARGET = "arrow_target";

SwitchEntity SwitchEntity::Parse(Object* object)
{
    SwitchEntity entity;

    MapEntity::Parse(&entity, object);

    // Determine subtype
    QString type = object->find(ELE_SUBTYPE, "NULL");
    if(type == WALKABLE)
        entity.subType = SubType::WALKABLE;
    else if(type == SOLID)
        entity.subType = SubType::SOLID;
    else if(type == ARROW_TARGET)
        entity.subType = SubType::ARROW_TARGET;

    entity.sprite = object->find(ELE_SPRITE, NULL_ELEMENT);
    entity.sound = object->find(ELE_SOUND, NULL_ELEMENT);

    QString needsBlock = object->find(ELE_NEEDS_BLOCK, NULL_ELEMENT);
    if(needsBlock == "false" || needsBlock == NULL_ELEMENT)
        entity.needsBlock = false;
    else
        entity.needsBlock = true;

    QString inactiveWhenLeaving = object->find(ELE_INACTIVATE_WHEN_LEAVING, NULL_ELEMENT);
    if(inactiveWhenLeaving == "false" || inactiveWhenLeaving == NULL_ELEMENT)
        entity.inactiveWhenLeaving = false;
    else
        entity.inactiveWhenLeaving = true;
}

void SwitchEntity::build(Object* object)
{
    object->data.clear();

    MapEntity::build(object);

    if(subType == SubType::WALKABLE)
        object->insert(ELE_SUBTYPE, WALKABLE);
    else if(subType == SubType::SOLID)
        object->insert(ELE_SUBTYPE, SOLID);
    else if(subType == SubType::ARROW_TARGET)
        object->insert(ELE_SUBTYPE, ARROW_TARGET);

    object->insert(ELE_SPRITE, sprite);

    if(sound != NULL_ELEMENT)
       object->insert(ELE_SOUND, sound);

    if(!inactiveWhenLeaving)
        object->insert(ELE_INACTIVATE_WHEN_LEAVING, DataType::Boolean, "false");
    else
        object->insert(ELE_INACTIVATE_WHEN_LEAVING, DataType::Boolean, "true");

    if(!needsBlock)
        object->insert(ELE_NEEDS_BLOCK, DataType::Boolean, "false");
    else
        object->insert(ELE_NEEDS_BLOCK, DataType::Boolean, "true");
}

SwitchEntity* SwitchEntity::clone()
{
    return new SwitchEntity(*this);
}

