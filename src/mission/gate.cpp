#include "gate.h"

Gate::Gate()
{
    keys = QList<Key*>();
}


Gate::Gate(QString name, QString scriptFilePath, QList<Key*> keys)
{
    this->name = name;
    this->scriptFilePath = scriptFilePath;
    this->keys = keys;
}

Gate Gate::Parse(Object* object, QList<Key*> keyList)
{
    Gate gate = Gate();

    gate.name = object->find(ELE_NAME, "");
    gate.scriptFilePath = object->find(ELE_SCRIPT_FILE, "");

    // Find all keys
    QString keyVal = object->find(ELE_KEY_LINK, "");
    QStringList keyNames = keyVal.split(',');
    for(Key* key : keyList)
    {
        for(QString name : keyNames)
        {
            if(key->getName() == name)
                gate.keys.append(key);
        }
    }

    return gate;
}

Object Gate::Build()
{
    Object obj = Object();

    obj.insert(ELE_NAME, this->name);
    obj.insert(ELE_SCRIPT_FILE, this->scriptFilePath);

    // Output key events as names
    QString keyVal = "";
    for(Key* key : keys)
        keyVal += key->getName() + ",";
    obj.insert(ELE_KEY_LINK, keyVal);

    return obj;
}
