#include "missionitemcollection.h"

MissionItemCollection::MissionItemCollection()
{
    gates = QMap<QString,Gate>();
    keyEvents = QMap<QString,Key>();
}

MissionItemCollection::~MissionItemCollection()
{

}

MissionItemCollection MissionItemCollection::Parse(Table* data)
{
    MissionItemCollection collection = MissionItemCollection();
    QList<Object*> keyObjects = data->getObjectsOfName(OBJ_KEY_EVENT);
    QList<Object*> gateObjects = data->getObjectsOfName(OBJ_GATE);

    // Clear any existing data
    collection.gates.clear();
    collection.keyEvents.clear();

    // Parse all objects, and insert them into the collection
    for(Object* obj : keyObjects)
    {
        Key key = Key::Parse(obj);
        collection.keyEvents.insert(key.getName(), key);
    }

    for(Object* obj : gateObjects)
    {
        Gate gate = Gate::Parse(obj, collection.getKeyEventList());
        collection.gates.insert(gate.getName(), gate);
    }

    return collection;
}

void MissionItemCollection::Build(Table* table)
{
    // Clear any existing table data
    table->clear();

    // Build all key events into the table
    for(QMap<QString,Key>::iterator iter = keyEvents.begin(); iter != keyEvents.end(); iter++)
        table->addObject(OBJ_KEY_EVENT, iter.value().Build());

    // Build all gates into the table
    for(QMap<QString,Gate>::iterator iter = gates.begin(); iter != gates.end(); iter++)
        table->addObject(OBJ_GATE, iter.value().Build());
}

Key* MissionItemCollection::getKeyEvent(QString name)
{
    QMap<QString,Key>::iterator iter = keyEvents.find(name);

    if(iter != keyEvents.end())
        return &iter.value();
    else
        return nullptr;
}

Gate* MissionItemCollection::getGate(QString name)
{
    QMap<QString,Gate>::iterator iter = gates.find(name);

    if(iter != gates.end())
        return &iter.value();
    else
        return nullptr;
}

QList<Gate*> MissionItemCollection::getGateList()
{
    QList<Gate*> gateList = QList<Gate*>();
    for(QMap<QString,Gate>::iterator iter = gates.begin(); iter != gates.end(); iter++)
    {
        gateList.append(&iter.value());
    }
    return gateList;
}

QList<Key*> MissionItemCollection::getKeyEventList()
{
    QList<Key*> keyList = QList<Key*>();
    for(QMap<QString,Key>::iterator iter = keyEvents.begin(); iter != keyEvents.end(); iter++)
    {
        keyList.append(&iter.value());
    }
    return keyList;
}

QStringList MissionItemCollection::getKeyEventNameList()
{
    QStringList list = QStringList();
    for(QMap<QString,Key>::iterator iter = keyEvents.begin(); iter != keyEvents.end(); iter++)
        list << iter.value().getName();
    return list;
}

QStringList MissionItemCollection::getGateNameList()
{
    QStringList list = QStringList();
    for(QMap<QString,Gate>::iterator iter = gates.begin(); iter != gates.end(); iter++)
        list << iter.value().getName();
    return list;
}

bool MissionItemCollection::AddGate(QString name, Gate gate)
{
    if(!gates.contains(name))
    {
        gates.insert(name, gate);
        return true;
    }
    else
        return false;
}

bool MissionItemCollection::AddKeyEvent(QString name, Key key)
{
    if(!keyEvents.contains(name))
    {
        keyEvents.insert(name, key);
        return true;
    }
    else
        return false;
}

bool MissionItemCollection::RemoveKeyEvent(QString name)
{
    if(keyEvents.contains(name))
    {
        keyEvents.remove(name);
        return true;
    }
    else
        return false;
}

bool MissionItemCollection::RemoveGate(QString name)
{
    if(gates.contains(name))
    {
        gates.remove(name);
        return true;
    }
    else
        return false;
}
