#ifndef MISSIONGENERATIONOPTIONS_H
#define MISSIONGENERATIONOPTIONS_H

#include "filetools.h"

class MissionGenerationOptions
{
public:
    MissionGenerationOptions();
    virtual ~MissionGenerationOptions() { };

    static MissionGenerationOptions Parse(Object* object);

    Object build();

    /*!
     * \brief Set the maximum distance a key can be from it's gate.
     * \param keyDist The maximum distance, in number of stages, a key can be from it's gate.
     */
    void setMaximumKeyDist(int keyDist) { maximumKeyDist = keyDist; if(maximumKeyDist <= 0) maximumKeyDist = 1; }

    /*!
     * \brief Get the maximum distance a key can be from it's gate.
     */
    int getMaximumKeyDist() { return maximumKeyDist; }

private:
    int maximumKeyDist; /*!< Maximum distance a key can be from it's gate. */
};

#endif // MISSIONGENERATIONOPTIONS_H
