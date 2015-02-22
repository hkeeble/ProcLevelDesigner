#include "gate.h"

Gate::Gate()
{

}

Gate::Gate(QString name, Gate::Type type, QList<Key*> keys, bool isTriggered)
{
    this->name = name;
    this->keys = keys;
    this->type = type;
    this->triggered = isTriggered;
}

Gate Gate::Parse(Object* object, QMap<QString,Key>* keys)
{
    Gate gate = Gate();

    gate.name =      object->find(ELE_NAME, "");
    gate.type =      static_cast<Gate::Type>(object->find(ELE_GATE_TYPE, "").toInt());

    QStringList keyNames = object->find(ELE_KEY_LINKS, "").split(LIST_DELIM);
    for(QString keyName : keyNames)
    {
        if(keyName != "")
            gate.keys.append(&keys->find(keyName).value());
    }
    gate.triggered = (object->find(ELE_TRIGGERED, "false") == "false" ? false : true);

    return gate;
}

Object Gate::build()
{
    Object obj = Object();

    obj.insert(ELE_NAME, this->name);
    obj.insert(ELE_GATE_TYPE, QString::number(this->type));
    obj.insert(ELE_TRIGGERED, (triggered ? "true" : "false"));

    QString keyVal = "";
    for(Key* key : keys)
        keyVal += key->getName() + LIST_DELIM;
    keyVal.remove(keyVal.length()-1, 1);
    obj.insert(ELE_KEY_LINKS, keyVal);

    return obj;
}

bool Gate::addKey(Key* key)
{
    if(keys.contains(key))
        return false;
    else
        keys.append(key);
}

bool Gate::removeKey(Key* key)
{
    return keys.removeOne(key);
}
