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

Stage::~Stage()
{

}
