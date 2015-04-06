#ifndef SESSIONMONITOR_H
#define SESSIONMONITOR_H

#include <QList>

#include "filetools.h"
#include "missiongenerationdata.h"
#include "spacegenerationdata.h"

class SessionMonitor
{
public:
    SessionMonitor();

    void addSpaceGenerationData(const SpaceGenerationData& data) { spaceGenerationData.append(data); }
    void addMissionGenerationData(const MissionGenerationData& data) { missionGenerationData.append(data); }

    void clear();

    void build(Table* table);
    static SessionMonitor Parse(Table* table);

private:
    QList<MissionGenerationData> missionGenerationData;
    QList<SpaceGenerationData> spaceGenerationData;
};

#endif // SESSIONMONITOR_H
