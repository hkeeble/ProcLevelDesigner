#ifndef GATE_H
#define GATE_H

#include <QString>
#include <QList>

#include "filetools.h"
#include "key.h"

class Gate
{
public:
    enum Type
    {
        Door,
        NPC,
        COUNT
    };

    Gate();
    Gate(QString name, Gate::Type type, QStringList keys, bool isTriggered);
    virtual ~Gate() { }

    inline QString getName() { return name; }

    static Gate Parse(Object* object, QList<Key*> keys);
    virtual Object Build();

private:
    QString name;       /*!< The name of this gate. */
    Gate::Type type;      /*!< The type of gate. */
    QStringList keys;   /*!< The list of keys that are neccesary to access this gate. */
    bool triggered;     /*!< Whether or not this gate is triggered (checks for trigger on map load, or is accessed through direct player interaction). */
};

const QString GATE_TYPE_STRINGS[Gate::Type::COUNT] =
{
  "Door",
  "NPC"
}; /*!< Use GATE_TYPE_STRINGS to retrieve a string representation of the given gate type. */

#endif // GATE_H
