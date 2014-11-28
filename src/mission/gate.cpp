#include "gate.h"

Gate::Gate()
    : MissionItem()
{
    key = "";
}


Gate::Gate(QString name, QString scriptFilePath, QString key)
    : MissionItem(name, scriptFilePath)
{
    this->key = key;
}

Gate* Gate::Parse(Object* object)
{
    Gate* gate = dynamic_cast<Gate*>(MissionItem::Parse(object));

    gate->key = object->find(ELE_KEY_LINK, "");

    return gate;
}

Object Gate::Build()
{
    Object obj = MissionItem::Build();

    obj.insert(ELE_KEY_LINK, this->key);

    return obj;
}
