#include "stage.h"

Stage::Stage(int id)
{
    keys = QList<Key*>();
    nextGate = nullptr;
    previousGate = nullptr;
    nextStage = nullptr;
    previousStage = nullptr;
    this->id = id;
}

Stage::Stage(int id, QList<Key*> keys)
    : Stage(id)
{
    this->keys = keys;
}


Stage::Stage(int id, Stage* previousSage, Stage* nextStage, Gate* previousGate, Gate* nextGate, QList<Key*> keys) :
    Stage(id, keys)
{
    this->nextGate = nextGate;
    this->previousGate = previousGate;
    this->nextStage = nextStage;
    this->previousStage = previousStage;
}

Stage Stage::Parse(QList<Gate*> gates, QList<Key*> keys, Object* data)
{
    // Parse all data first
    QStringList keyList = data->find(ELE_KEYS, "").split(LIST_DELIM);
    QString prevGateName = data->find(ELE_PREVIOUS_GATE, "");
    QString nextGateName = data->find(ELE_NEXT_GATE, "");
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

    // Find gates
    Gate* nextGate, *prevGate;
    for(Gate* gate : gates)
    {
        if(gate->getName() == prevGateName)
            prevGate = gate;
        else if(gate->getName() == nextGateName)
            nextGate = gate;
    }

    return Stage(id.toInt(), nullptr, nullptr, prevGate, nextGate, reqKeys);
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
    data->insert(ELE_PREVIOUS_GATE, previousGate->getName());
    data->insert(ELE_NEXT_GATE, nextGate->getName());
    data->insert(ELE_NEXT_STAGE_ID, QString::number(nextStage->getID()));
    data->insert(ELE_PREVIOUS_STAGE_ID, QString::number(previousStage->getID()));
}

Stage::~Stage()
{

}
