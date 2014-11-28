#include "key.h"

Key::Key() : MissionItem()
{
    gate = "";
}

Key::Key(QString name, QString scriptFilePath, QString gate)
    : MissionItem(name, scriptFilePath)
{
    this->gate = gate;
}

Key* Key::Parse(Object* object)
{
    Key* key = dynamic_cast<Key*>(MissionItem::Parse(object));

    key->gate = object->find(ELE_GATE_LINK, "");

    return key;
}

Object Key::Build()
{
    Object object = MissionItem::Build();

    object.insert(ELE_GATE_LINK, gate);

    return object;
}
