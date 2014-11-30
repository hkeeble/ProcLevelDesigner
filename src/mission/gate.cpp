#include "gate.h"

Gate::Gate()
{

}

Gate::Gate(QString name, Gate::Type type, QStringList keys, bool isTriggered)
{
    this->name = name;
    this->keys = keys;
    this->type = type;
    this->triggered = isTriggered;
}

Gate Gate::Parse(Object* object, QList<Key*> keyList)
{
    Gate gate = Gate();

    gate.name =      object->find(ELE_NAME, "");
    gate.type =      static_cast<Gate::Type>(object->find(ELE_GATE_TYPE, "").toInt());
    gate.keys =       object->find(ELE_KEY_LINKS, "").split(':');
    gate.triggered = (object->find(ELE_TRIGGERED, "false") == "false" ? false : true);

    return gate;
}

Object Gate::Build()
{
    Object obj = Object();

    obj.insert(ELE_NAME, this->name);
    obj.insert(ELE_GATE_TYPE, QString::number(this->type));
    obj.insert(ELE_TRIGGERED, (triggered ? "true" : "false"));

    QString keyVal = "";
    for(QString key : keys)
        keyVal += key + ":";
    keyVal.remove(keyVal.length()-1, 1);
    obj.insert(ELE_KEY_LINKS, keyVal);

    return obj;
}
