#ifndef SPACEGENERATIONDATA_H
#define SPACEGENERATIONDATA_H

#include "space.h"

class SpaceGenerationData
{
public:
    SpaceGenerationData() { };
    SpaceGenerationData(Space& space);

    Object build();
    static SpaceGenerationData Parse(Object* data);
private:
    int maxAreaWidth, maxAreaHeight, maxAreasPerStage;
};

#endif // SPACEGENERATIONDATA_H
