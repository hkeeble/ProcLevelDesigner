#ifndef KEY_H
#define KEY_H

#include <QString>
#include <QList>

#include "filetools.h"

/*!
 * \brief Represents a key event in the mission structure.
 */
class Key
{
public:
    /*!
     * \brief Represents type of key.
     */
    enum Type
    {
        Switch, /*!< The key is represented in-game with a switch event. */
        NPC,    /*!< The key is represented in-game via an NPC dialog. */
        Chest,  /*!< The key is represented in-game through an item taken from a chest. */
        Item,    /*!< The key is represented in-game by an item that can be collected by the player. */
        COUNT
    };

    Key();
    Key(QString name, Key::Type type, QString message = "");
    virtual ~Key() { }

    inline QString getName()    { return name; }
    inline QString getMessage() { return message; }
    inline Key::Type getKeyType() { return type; }

    inline void setName(const QString& name) { this->name = name; }
    inline void setMessage(const QString& message) { this->message = message; }
    inline void setType(const Key::Type& type) { this->type = type; }

    static Key Parse(Object* object);
    virtual Object build();

private:
    QString name, message;
    Key::Type type; /*!< The type of this key event. */
};

const QString KEY_TYPE_STRINGS[Key::Type::COUNT] =
{
  "Switch",
  "NPC",
  "Chest",
  "Item"
}; /*!< Use KEY_TYPE_STRINGS to retrieve a string representation of the given key type. */

#endif // KEY_H
