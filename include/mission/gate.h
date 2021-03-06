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
    inline bool isTriggered() { return triggered; }
    inline Gate::Type getType() { return type; }
    inline QStringList getKeys() { return keys; }

    inline void setName(const QString& name) { this->name = name; }
    inline void setTriggered(const bool& triggered) { this->triggered = triggered; }
    inline void setType(const Gate::Type& type) { this->type = type; }
    inline void setKeys(const QStringList& keys) { this->keys = keys; }

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
