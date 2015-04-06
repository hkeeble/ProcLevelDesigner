#ifndef MISSIONGENERATIONDATA_H
#define MISSIONGENERATIONDATA_H

#include "mission.h"

class MissionGenerationData
{
public:
    MissionGenerationData() { };
    MissionGenerationData(Mission& mission);

    int getKeyLockCount() { return keyLockCount; }
    int getGateLockCount() { return gateLockCount; }
    int getTotalLockCount() { return keyLockCount + gateLockCount; }
    int getMaxKeyDist() { return maxKeyDist; }

    Object build();
    static MissionGenerationData Parse(Object* data);

private:
    int keyLockCount, gateLockCount;
    int maxKeyDist;
};

#endif // MISSIONGENERATIONDATA_H
