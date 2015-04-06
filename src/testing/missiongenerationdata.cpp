#include "missiongenerationdata.h"

MissionGenerationData::MissionGenerationData(Mission& mission)
{
    keyLockCount = 0;
    gateLockCount = 0;

    QList<Stage*> stages = mission.getStages();
    for(Stage* stage : stages)
    {
        keyLockCount += stage->getLockedKeys().size();
        gateLockCount += stage->isGateLocked() ? 1 : 0;
    }

    maxKeyDist = mission.getOptions().getMaximumKeyDist();
}

Object MissionGenerationData::build()
{
    Object obj = Object();

    obj.insert(ELE_MAX_KEY_DIST, QString::number(maxKeyDist));
    obj.insert(ELE_KEY_LOCK_COUNT, QString::number(keyLockCount));
    obj.insert(ELE_GATE_LOCK_COUNT, QString::number(gateLockCount));

    return obj;
}

MissionGenerationData MissionGenerationData::Parse(Object* data)
{
    MissionGenerationData missionData = MissionGenerationData();

    missionData.gateLockCount = data->find(ELE_GATE_LOCK_COUNT, "0").toInt();
    missionData.keyLockCount = data->find(ELE_KEY_LOCK_COUNT, "0").toInt();
    missionData.maxKeyDist = data->find(ELE_MAX_KEY_DIST, "0").toInt();

    return missionData;
}
