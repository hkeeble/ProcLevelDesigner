#include "teletransporter.h"

const QString Teletransporter::FADE = "fade";
const QString Teletransporter::SCROLL = "scrolling";
const QString Teletransporter::IMMEDIATE = "immediate";

Teletransporter::Teletransporter(int x, int y, int width, int height, QString destinationMap, QString destination, Transition transition)
    : Teletransporter()
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->destinationMap = destinationMap;
    this->destination = destination;
    this->transition = transition;
}

Teletransporter Teletransporter::Parse(Object* object)
{
    Teletransporter teletransporter;

    MapEntity::Parse(&teletransporter, object);

    teletransporter.width = object->find(ELE_WIDTH, "32").toInt();
    teletransporter.height = object->find(ELE_HEIGHT, "32").toInt();
    teletransporter.destination = object->find(ELE_DESTINATION, NULL_ELEMENT);
    teletransporter.destinationMap = object->find(ELE_DESTINATION_MAP, NULL_ELEMENT);

    // Determine transition
    QString trans = object->find(ELE_TRANSITION, IMMEDIATE);
    if(trans == IMMEDIATE)
        teletransporter.transition = Transition::Immediate;
    else if(trans == FADE)
        teletransporter.transition = Transition::Fade;
    else if(trans == SCROLL)
        teletransporter.transition = Transition::Scroll;

    return teletransporter;
}

void Teletransporter::build(Object* object)
{
    object->data.clear(); // Clear any existing data

    // build in base class data
    MapEntity::build(object);

    object->insert(ELE_WIDTH, QString::number(width));
    object->insert(ELE_HEIGHT, QString::number(height));
    object->insert(ELE_DESTINATION, destination);
    object->insert(ELE_DESTINATION_MAP, destinationMap);

    // Determine transition
    if(transition == Transition::Immediate)
        object->insert(ELE_TRANSITION, IMMEDIATE);
    else if(transition == Transition::Fade)
        object->insert(ELE_TRANSITION, FADE);
    else if(transition == Transition::Scroll)
        object->insert(ELE_TRANSITION, SCROLL);
}
