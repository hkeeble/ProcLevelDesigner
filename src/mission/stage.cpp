#include "stage.h"

Stage::Stage(int id)
{
    keys = QList<Key*>();
    next = nullptr;
    previous = nullptr;
    exitGate = nullptr;
    this->id = id;
}

Stage::Stage(int id, QList<Key*> keys, QList<Key*> lockedKeys, bool gateLocked)
    : Stage(id)
{
    this->keys = keys;
    this->lockedKeys = lockedKeys;
    this->gateLocked = gateLocked;
}

Stage::Stage(int id, int previousID, int nextID, Gate* exitGate, QList<Key*> keys, QList<Key*> lockedKeys, bool gateLocked)
    : Stage(id, nullptr, nullptr, exitGate, keys, lockedKeys, gateLocked)
{
    this->prevID = previousID;
    this->nextID = nextID;
}

Stage::Stage(int id, Stage* previous, Stage* next, Gate* exitGate, QList<Key*> keys, QList<Key*> lockedKeys, bool gateLocked) :
    Stage(id, keys, lockedKeys, gateLocked)
{
    this->exitGate = exitGate;
    this->next = next;
    this->previous = previous;
}

Stage Stage::Parse(QList<Gate*> gates, QList<Key*> keys, Object* data)
{
    // Parse all data first
    QStringList keyList = data->find(ELE_KEYS, "").split(LIST_DELIM);
    QStringList lockedKeyList = data->find(ELE_LOCKED_KEYS, "").split(LIST_DELIM);
    QString exitGateName = data->find(ELE_EXIT_GATE, NULL_ELEMENT);
    QString nextStageID = data->find(ELE_NEXT_STAGE_ID, "");
    QString prevStageID = data->find(ELE_PREVIOUS_STAGE_ID, "");
    QString id = data->find(ELE_ID, "0");
    bool gateLocked = data->find(ELE_GATE_LOCKED, "false") == "true" ? true : false;

    // Find list of keys and locked keys
    QList<Key*> reqKeys;
    QList<Key*> reqLockedKeys;
    for(QString keyName : keyList)
    {
        for(Key* key : keys)
        {
            if(key->getName() == keyName)
            {
                reqKeys.append(key);

                if(lockedKeyList.contains(key->getName()))
                    reqLockedKeys.append(key);
            }
        }
    }

    // Find exit gate
    Gate *exGate;
    for(Gate* gate : gates)
    {
        if(gate->getName() == exitGateName)
            exGate = gate;
    }

    return Stage(id.toInt(), prevStageID.toInt(), nextStageID.toInt(), exGate, reqKeys, reqLockedKeys, gateLocked);
}

void Stage::build(Object* data)
{
    data->data.clear();

    // Construct key list
    QString keyString = "";
    for(Key* key : keys)
    {
        keyString += key->getName() + LIST_DELIM;
    }
    keyString.remove(keyString.length()-1, 1);

    data->insert(ELE_KEYS, keyString);
    data->insert(ELE_ID, QString::number(id));
    data->insert(ELE_EXIT_GATE, exitGate->getName());

    if(next != nullptr)
        data->insert(ELE_NEXT_STAGE_ID, QString::number(next->getID()));
    else
        data->insert(ELE_NEXT_STAGE_ID, NULL_ELEMENT);

    if(previous != nullptr)
        data->insert(ELE_PREVIOUS_STAGE_ID, QString::number(previous->getID()));
    else
        data->insert(ELE_PREVIOUS_STAGE_ID, NULL_ELEMENT);

    // Build locked key list
    QString lockedKeysList;
    for(Key* key : lockedKeys)
    {
        lockedKeysList += key->getName() + LIST_DELIM;
    }
    lockedKeysList.remove(lockedKeysList.length()-1, 1);

    data->insert(ELE_LOCKED_KEYS, lockedKeysList);
}

Stage::~Stage()
{

}

Key* Stage::getKey(QString name)
{
    for(Key* key : keys)
    {
        if(key->getName() == name)
        {
            return key;
        }
    }

    return nullptr;
}


void Stage::lockKey(Key* key)
{
    if(keys.contains(key))
    {
        if(!lockedKeys.contains(key))
            lockedKeys.append(key);
    }
}

void Stage::unlockKey(Key* key)
{
    if(keys.contains(key))
    {
        if(lockedKeys.contains(key))
            lockedKeys.removeOne(key);
    }
}

bool Stage::isKeyLocked(Key* key)
{
    if(keys.contains(key))
    {
        if(lockedKeys.contains(key))
            return true;
    }

    return false;
}

void Stage::clearUnlockedKeys()
{
    for(Key* key : keys)
    {
        if(!lockedKeys.contains(key))
            keys.removeOne(key);
    }
}

bool Stage::removeKey(Key* key)
{
    if(lockedKeys.contains(key))
        lockedKeys.removeOne(key);

    return keys.removeOne(key);
}
