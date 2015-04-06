#include "sessionmonitor.h"

SessionMonitor::SessionMonitor()
{

}

void SessionMonitor::clear()
{
    spaceGenerationData.clear();
    missionGenerationData.clear();
}

void SessionMonitor::build(Table* table)
{
    table->clear();

    for(SpaceGenerationData data : spaceGenerationData)
    {
        table->addObject(OBJ_SPACE_DATA, data.build());
    }

    for(MissionGenerationData data : missionGenerationData)
    {
        table->addObject(OBJ_MISSION_DATA, data.build());
    }
}

SessionMonitor SessionMonitor::Parse(Table* table)
{
    SessionMonitor monitor = SessionMonitor();

    QList<Object*> spaceData = table->getObjectsOfName(OBJ_SPACE_DATA);
    QList<Object*> missionData = table->getObjectsOfName(OBJ_MISSION_DATA);

    for(Object* data : spaceData)
    {
        monitor.addSpaceGenerationData(SpaceGenerationData::Parse(data));
    }

    for(Object* data : missionData)
    {
        monitor.addMissionGenerationData(MissionGenerationData::Parse(data));
    }

    return monitor;
}
