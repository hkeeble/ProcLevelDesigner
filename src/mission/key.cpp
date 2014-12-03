#include "key.h"

Key::Key()
{

}

Key::Key(QString name, Key::Type type, QString message)
{
    this->name = name;
    this->type = type;
    this->message = message;
}

Key Key::Parse(Object* object)
{
    Key key = Key();

    key.name = object->find(ELE_NAME, "");
    key.message = object->find(ELE_KEY_MESSAGE, "");
    key.type = static_cast<Key::Type>(object->find(ELE_KEY_TYPE, "0").toInt());

    return key;
}

Object Key::build()
{
    Object obj = Object();

    obj.insert(ELE_NAME, this->name);
    obj.insert(ELE_KEY_MESSAGE, this->message);
    obj.insert(ELE_KEY_TYPE, QString::number(this->type));

    return obj;
}
