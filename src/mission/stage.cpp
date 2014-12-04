#include "stage.h"

Stage::Stage(int id)
{
    keys = QList<Key*>();
    next = nullptr;
    previous = nullptr;
    exitGate = nullptr;
    this->id = id;
}

Stage::Stage(int id, QList<Key*> keys)
    : Stage(id)
{
    this->keys = keys;
}

Stage::Stage(int id, int previousID, int nextID, Gate* exitGate, QList<Key*> keys)
    : Stage(id, nullptr, nullptr, exitGate, keys)
{
    this->prevID = previousID;
    this->nextID = nextID;
}

Stage::Stage(int id, Stage* previous, Stage* next, Gate* exitGate, QList<Key*> keys) :
    Stage(id, keys)
{
    this->exitGate = exitGate;
    this->next = next;
    this->previous = previous;
}

Stage Stage::Parse(QList<Gate*> gates, QList<Key*> keys, Object* data)
{
    // Parse all data first
    QStringList keyList = data->find(ELE_KEYS, "").split(LIST_DELIM);
    QString exitGateName = data->find(ELE_EXIT_GATE, NULL_ELEMENT);
    QString nextStageID = data->find(ELE_NEXT_STAGE_ID, "");
    QString prevStageID = data->find(ELE_PREVIOUS_STAGE_ID, "");
    QString id = data->find(ELE_ID, "0");

    // Find list of keys
    QList<Key*> reqKeys;
    for(QString keyName : keyList)
    {
        for(Key* key : keys)
        {
            if(key->getName() == keyName)
                reqKeys.append(key);
        }
    }

    // Find exit gate
    Gate *exGate;
    for(Gate* gate : gates)
    {
        if(gate->getName() == exitGateName)
            exGate = gate;
    }

    return Stage(id.toInt(), prevStageID.toInt(), nextStageID.toInt(), exGate, reqKeys);
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
}

Stage::~Stage()
{

}
