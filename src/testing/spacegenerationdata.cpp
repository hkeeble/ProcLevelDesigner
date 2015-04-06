#include "spacegenerationdata.h"

SpaceGenerationData::SpaceGenerationData(Space& space)
{
    maxAreaHeight = space.getOptions().getMaximumAreaHeight();
    maxAreaWidth = space.getOptions().getMaximumAreaWidth();
    maxAreasPerStage = space.getOptions().getMaximumAreasPerStage();
}

Object SpaceGenerationData::build()
{
    Object obj = Object();

    obj.insert(ELE_MAX_AREAS_PER_STAGE, QString::number(maxAreasPerStage));
    obj.insert(ELE_MAX_AREA_HEIGHT, QString::number(maxAreaHeight));
    obj.insert(ELE_MAX_AREA_WIDTH, QString::number(maxAreaWidth));

    return obj;
}

SpaceGenerationData SpaceGenerationData::Parse(Object* data)
{
    SpaceGenerationData spaceData = SpaceGenerationData();

    spaceData.maxAreaHeight = data->find(ELE_MAX_AREA_HEIGHT, "0").toInt();
    spaceData.maxAreaWidth = data->find(ELE_MAX_AREA_WIDTH, "0").toInt();
    spaceData.maxAreasPerStage = data->find(ELE_MAX_AREAS_PER_STAGE, "0").toInt();

    return spaceData;
}
