#ifndef SPACEGENERATIONOPTIONS_H
#define SPACEGENERATIONOPTIONS_H

#include "filetools.h"

/*!
 * \brief Represents user defined options for space generation.
 */
class SpaceGenerationOptions
{
public:
    SpaceGenerationOptions();
    virtual ~SpaceGenerationOptions();

    static SpaceGenerationOptions Parse(Object* object);
    void build(Object* object);

    inline void setMinimumAreasPerStage(int minAreas) { minAreasPerStage = minAreas; }
    inline void setMaximumAreasPerStage(int maxAreas) { maxAreasPerStage = maxAreas; }

    inline void setMinimumAreaWidth(int minWidth)     { minAreaWidth = minWidth; }
    inline void setMaximumAreaWidth(int maxWidth)     { maxAreaWidth = maxWidth; }
    inline void setMinimumAreaHeight(int minHeight)   { minAreaHeight = minHeight; }
    inline void setMaximumAreaHeight(int maxHeight)   { maxAreaHeight = maxHeight; }

    inline int getMinimumAreasPerStage() const { return minAreasPerStage; }
    inline int getMaximumAreasPerStage() const { return maxAreasPerStage; }

    inline int getMinimumAreaWidth() const    { return minAreaWidth; }
    inline int getMaximumAreaWidth() const    { return maxAreaWidth; }
    inline int getMinimumAreaHeight() const   { return minAreaHeight; }
    inline int getMaximumAreaHeight() const   { return maxAreaHeight; }

private:
    int minAreasPerStage, maxAreasPerStage;
    int minAreaWidth, maxAreaWidth, minAreaHeight, maxAreaHeight;
};

#endif // SPACEGENERATIONOPTIONS_H
