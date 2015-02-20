#include "spacegenerationoptions.h"

SpaceGenerationOptions::SpaceGenerationOptions()
{
    // Set some default values here
    minAreasPerStage = 3;
    maxAreasPerStage = 6;

    minAreaWidth = 1;
    maxAreaWidth = 4;
    minAreaHeight = 1;
    maxAreaHeight = 4;
}

SpaceGenerationOptions::~SpaceGenerationOptions()
{
    // Nothing yet...
}

SpaceGenerationOptions SpaceGenerationOptions::Parse(Object* object)
{
    SpaceGenerationOptions options;

    options.minAreaHeight = object->find(ELE_MIN_AREA_HEIGHT, "1").toInt();
    options.maxAreaHeight = object->find(ELE_MAX_AREA_HEIGHT, "4").toInt();
    options.minAreaWidth = object->find(ELE_MIN_AREA_WIDTH, "1").toInt();
    options.maxAreaWidth = object->find(ELE_MAX_AREA_WIDTH, "4").toInt();

    options.minAreasPerStage = object->find(ELE_MIN_AREAS_PER_STAGE, "3").toInt();
    options.maxAreasPerStage = object->find(ELE_MAX_AREAS_PER_STAGE, "6").toInt();

    return options;
}

void SpaceGenerationOptions::build(Object* object)
{
    object->data.clear();

    object->insert(ELE_MIN_AREA_HEIGHT, QString::number(minAreaHeight));
    object->insert(ELE_MAX_AREA_HEIGHT, QString::number(maxAreaHeight));
    object->insert(ELE_MIN_AREA_WIDTH, QString::number(minAreaWidth));
    object->insert(ELE_MAX_AREA_WIDTH, QString::number(maxAreaWidth));

    object->insert(ELE_MIN_AREAS_PER_STAGE, QString::number(minAreasPerStage));
    object->insert(ELE_MAX_AREAS_PER_STAGE, QString::number(maxAreasPerStage));
}
