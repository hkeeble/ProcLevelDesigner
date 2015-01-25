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
    Gate(QString name, Gate::Type type, QList<Key*> keys, bool isTriggered);
    virtual ~Gate() { }

    inline QString getName() { return name; }
    inline bool isTriggered() { return triggered; }
    inline Gate::Type getType() { return type; }
    inline QList<Key*> getKeys() { return keys; }

    inline void setName(const QString& name) { this->name = name; }
    inline void setTriggered(const bool& triggered) { this->triggered = triggered; }
    inline void setType(const Gate::Type& type) { this->type = type; }
    inline void setKeys(QList<Key*> keys) { this->keys = keys; }

    /*!
     * \brief Adds a key to the gate. If the key already exists, returns false.
     */
    bool addKey(Key* key);

    /*!
     * \brief Removes a key from the gate. If the key does not exist, returns false.
     */
    bool removeKey(Key* key);

    static Gate Parse(Object* object, QMap<QString,Key>* keys);
    virtual Object build();

private:
    QString name;       /*!< The name of this gate. */
    Gate::Type type;    /*!< The type of gate. */
    QList<Key*> keys;   /*!< The list of keys that are neccesary to access this gate. */
    bool triggered;     /*!< Whether or not this gate is triggered (checks for trigger on map load, or is accessed through direct player interaction). */
};

const QString GATE_TYPE_STRINGS[Gate::Type::COUNT] =
{
  "Door",
  "NPC"
}; /*!< Use GATE_TYPE_STRINGS to retrieve a string representation of the given gate type. */

#endif // GATE_H
