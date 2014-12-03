#include "mission.h"

Mission::Mission()
{
    stages = QList<Stage*>();
}

Mission::~Mission()
{
    for(Stage* stage : stages)
        delete stage;
}

void Mission::init(Table* itemData)
{
    itemCollection = MissionItemCollection::Parse(itemData);
}


void Mission::generate()
{
    stages.clear();

    // Obtain keys and gates
    QList<Gate*> gates = itemCollection.getGateList();
    QList<Key*> keys = itemCollection.getKeyEventList();

    // Initialize stages
    for(int i = 0; i < gates.length()+1; i++)
        stages.append(new Stage(i));

    // Initialize stage links and gate links
    for(int i = 0; i < stages.length(); i++)
    {
        if(i != stages.length()-1)
        {
            stages[i]->setNextStage(stages[i+1]);
            stages[i]->setNextGate(gates[i]);
        }

        if(i != 0)
        {
            stages[i]->setPreviousStage(stages[i-1]);
            stages[i]->setPreviousGate(gates[i-1]);
        }
    }

    qDebug() << "Mission Structure: ";
    QDebug debug = qDebug();
    for(Stage* stage : stages)
    {
        debug << "Stage " + QString::number(stage->getID()) << " -> " << (stage->getNextGate() == nullptr ? "" : stage->getNextGate()->getName() + " -> ");
    }
}
