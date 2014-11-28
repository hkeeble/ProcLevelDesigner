#ifndef MISSIONITEM_H
#define MISSIONITEM_H

#include <QString>

#include "filetools.h"

class MissionItem
{
protected:
    MissionItem();
    MissionItem(QString name, QString scriptFilePath);
public:
    virtual ~MissionItem() { }

    inline QString getName() { return name; }
    inline QString getScriptFilePath() { return scriptFilePath; }

    static MissionItem* Parse(Object* object);
    virtual Object Build();

private:
    QString name, scriptFilePath;
};

#endif // MISSIONITEM_H
