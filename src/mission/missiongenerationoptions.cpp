#include "missiongenerationoptions.h"

MissionGenerationOptions::MissionGenerationOptions()
{
    maximumKeyDist = 1;
}

MissionGenerationOptions MissionGenerationOptions::Parse(Object* object)
{
    MissionGenerationOptions options;

    options.maximumKeyDist = object->find(ELE_KEY_MAXIMUM_DIST, "1").toInt();

    return options;
}

Object MissionGenerationOptions::build()
{
    Object object;

    object.insert(ELE_KEY_MAXIMUM_DIST, QString::number(maximumKeyDist));

    return object;
}
