#include "missionitem.h"

MissionItem::MissionItem()
{
    name = "";
    scriptFilePath = "";
}

MissionItem::MissionItem(QString name, QString scriptFilePath)
{
    this->name = name;
    this->scriptFilePath = scriptFilePath;
}

MissionItem* MissionItem::Parse(Object* object)
{
    MissionItem* item = new MissionItem();

    item->name = object->find(ELE_NAME, "");
    item->scriptFilePath = object->find(ELE_SCRIPT_FILE, "");

    return item;
}

Object MissionItem::Build()
{
    Object obj = Object();
    obj.insert(ELE_NAME, this->name);
    obj.insert(ELE_SCRIPT_FILE, this->scriptFilePath);
    return obj;
}
