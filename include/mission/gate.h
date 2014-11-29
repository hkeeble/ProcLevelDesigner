#ifndef GATE_H
#define GATE_H

#include <QString>
#include <QList>

#include "filetools.h"
#include "key.h"

class Gate
{
public:
    Gate();
    Gate(QString name, QString scriptFilePath, QList<Key*> keys);
    virtual ~Gate() { }

    inline QString getName() { return name; }
    inline QString getScriptFilePath() { return scriptFilePath; }
    inline QList<Key*> getKeys() { return keys; }

    static Gate Parse(Object* object, QList<Key*> keys);
    virtual Object Build();

private:
    QString name, scriptFilePath;
    QList<Key*> keys;
};

#endif // GATE_H
