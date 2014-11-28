#ifndef GATE_H
#define GATE_H

#include <QString>
#include <QList>

#include "missionitem.h"

class Key;

class Gate : public MissionItem
{
public:
    Gate();
    Gate(QString name, QString scriptFilePath, QString key);
    virtual ~Gate() { }

    inline QString getKey() { return key; }

    static Gate* Parse(Object* object);
    virtual Object Build() override final;

private:
    QString key;
};

#endif // GATE_H
