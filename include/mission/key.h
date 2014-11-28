#ifndef KEY_H
#define KEY_H

#include <QString>
#include <QList>

#include "missionitem.h"
#include "filetools.h"

class Key : public MissionItem
{
public:
    Key();
    Key(QString name, QString scriptFilePath, QString gate);
    virtual ~Key() { }

    inline QString getGate() { return gate; }

    static Key* Parse(Object* object);
    virtual Object Build() override final;

private:
    QString gate;
};

#endif // KEY_H
