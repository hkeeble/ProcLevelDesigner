#include "key.h"

Key::Key()
{

}

Key::Key(QString name, QString scriptFilePath)
{
    this->name = name;
    this->scriptFilePath = scriptFilePath;
}

Key Key::Parse(Object* object)
{
    Key key = Key();

    key.name = object->find(ELE_NAME, "");
    key.scriptFilePath = object->find(ELE_SCRIPT_FILE, "");

    return key;
}

Object Key::Build()
{
    Object obj = Object();

    obj.insert(ELE_NAME, this->name);
    obj.insert(ELE_SCRIPT_FILE, this->scriptFilePath);

    return obj;
}
