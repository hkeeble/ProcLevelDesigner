#ifndef KEY_H
#define KEY_H

#include <QString>
#include <QList>

#include "filetools.h"

class Key
{
public:
    Key();
    Key(QString name, QString scriptFilePath);
    virtual ~Key() { }

    inline QString getName() { return name; }
    inline QString getScriptFilePath() { return scriptFilePath; }

    static Key Parse(Object* object);
    virtual Object Build();

private:
    QString name, scriptFilePath;
};

#endif // KEY_H
