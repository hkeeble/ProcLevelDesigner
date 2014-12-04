#include "mission.h"

Mission::Mission()
{
    observer = new MissionObserver();
}

Mission::~Mission()
{

}

void Mission::generate()
{

    observer->emitUpdate();
}

Mission Mission::Parse(Table* data)
{
    Mission mission = Mission();
    QList<Object*> keyObjects = data->getObjectsOfName(OBJ_KEY_EVENT);
    QList<Object*> gateObjects = data->getObjectsOfName(OBJ_GATE);
    QList<Object*> stageObjects = data->getObjectsOfName(OBJ_STAGE);

    // Clear any existing data
    mission.gates.clear();
    mission.keyEvents.clear();
    mission.stages.clear();

    // Parse all objects
    for(Object* obj : keyObjects)
    {
        Key key = Key::Parse(obj);
        mission.keyEvents.insert(key.getName(), key);
    }

    for(Object* obj : gateObjects)
    {
        Gate gate = Gate::Parse(obj, mission.getKeyEventList());
        mission.gates.insert(gate.getName(), gate);
    }

    // Parse all stages
    for(Object* obj : stageObjects)
    {
        Stage stage = Stage::Parse(mission.getGateList(), mission.getKeyEventList(), obj);
        mission.stages.append(stage);
    }

    // Link all of our stages
    for(Stage& stage : mission.stages)
    {
        for(Stage& stageLink : mission.stages)
        {
            if(stageLink.getID() == stage.getPreviousStageID())
                stage.setPreviousStage(&stageLink);
            else if(stageLink.getID() == stage.getNextStageID())
                stage.setNextStage(&stageLink);
        }
    }

    mission.emitUpdate();

    return mission;
}

void Mission::build(Table* table)
{
    // Clear any existing table data
    table->clear();

    // build all gates into the table
    for(QMap<QString,Gate>::iterator iter = gates.begin(); iter != gates.end(); iter++)
        table->addObject(OBJ_GATE, iter.value().build());

    // build all key events into the table
    for(QMap<QString,Key>::iterator iter = keyEvents.begin(); iter != keyEvents.end(); iter++)
        table->addObject(OBJ_KEY_EVENT, iter.value().build());

    // build all stages into the table
    for(QList<Stage>::iterator iter = stages.begin(); iter != stages.end(); iter++)
    {
        Object obj = Object();
        iter->build(&obj);
        table->addObject(OBJ_STAGE, obj);
    }
}

Key* Mission::getKeyEvent(QString name)
{
    QMap<QString,Key>::iterator iter = keyEvents.find(name);

    if(iter != keyEvents.end())
        return &iter.value();
    else
        return nullptr;
}

Gate* Mission::getGate(QString name)
{
    QMap<QString,Gate>::iterator iter = gates.find(name);

    if(iter != gates.end())
        return &iter.value();
    else
        return nullptr;
}

QList<Gate*> Mission::getGateList()
{
    QList<Gate*> gateList = QList<Gate*>();
    for(QMap<QString,Gate>::iterator iter = gates.begin(); iter != gates.end(); iter++)
    {
        gateList.append(&iter.value());
    }
    return gateList;
}

QList<Key*> Mission::getKeyEventList()
{
    QList<Key*> keyList = QList<Key*>();
    for(QMap<QString,Key>::iterator iter = keyEvents.begin(); iter != keyEvents.end(); iter++)
    {
        keyList.append(&iter.value());
    }
    return keyList;
}

QList<Stage*> Mission::getStages()
{
    QList<Stage*> stageList = QList<Stage*>();
    for(QList<Stage>::iterator iter = stages.begin(); iter != stages.end(); iter++)
    {
        stageList.append(&(*iter));
    }
    return stageList;
}

QStringList Mission::getKeyEventNameList()
{
    QStringList list = QStringList();
    for(QMap<QString,Key>::iterator iter = keyEvents.begin(); iter != keyEvents.end(); iter++)
        list << iter.value().getName();
    return list;
}

QStringList Mission::getGateNameList()
{
    QStringList list = QStringList();
    for(QMap<QString,Gate>::iterator iter = gates.begin(); iter != gates.end(); iter++)
        list << iter.value().getName();
    return list;
}

bool Mission::addGate(QString name, Gate gate)
{
    if(!gates.contains(name))
    {
        gates.insert(name, gate);

        // Add a new stage due to the addition of a new gate
        Stage newStage = Stage(stages.count()-1, nullptr, nullptr, &gates.last(), QList<Key*>());

        // Set the new stage's previous stage
        if(stages.count() > 0)
            newStage.setPreviousStage(&stages[stages.count()-1]);

        stages.append(newStage);

        // Set the stage's previous next to the new stage
        if(stages[stages.count()-1].getPreviousStage())
            stages[stages.count()-1].getPreviousStage()->setNextStage(&stages[stages.count()-1]);

        // Update stage IDs
        updateStageIDs();

        observer->emitUpdate();
        return true;
    }
    else
        return false;
}

bool Mission::addKeyEvent(QString name, Key key)
{
    if(!keyEvents.contains(name))
    {
        keyEvents.insert(name, key);
        observer->emitUpdate();
        return true;
    }
    else
        return false;
}

bool Mission::removeKeyEvent(QString name)
{
    if(keyEvents.contains(name))
    {
        keyEvents.remove(name);

        // If the key event is being used by a gate, remove it
        for(QMap<QString,Gate>::iterator iter = gates.begin(); iter != gates.end(); iter++)
        {
            if(iter.value().getKeys().contains(name))
                iter.value().removeKey(name);
        }

        observer->emitUpdate();
        return true;
    }
    else
        return false;
}

bool Mission::removeGate(QString name)
{
    if(gates.contains(name))
    {
        Gate gate = gates.take(name);

        // Find the stage ending with this gate and remove it, relink other stages
        bool found = false;
        for(int i = 0; i < stages.length(); i++)
        {
            if(stages[i].getExitGate()->getName() == gate.getName())
            {
                found = true;
                Stage stage = stages.takeAt(i);

                if(stage.getPreviousStage())
                {
                    if(stage.getNextStage())
                        stage.getPreviousStage()->setNextStage(stage.getNextStage());
                    else
                        stage.getPreviousStage()->setNextStage(nullptr);
                }

                if(stage.getNextStage())
                {
                    if(stage.getPreviousStage())
                        stage.getNextStage()->setPreviousStage(stage.getPreviousStage());
                    else
                        stage.getNextStage()->setPreviousStage(nullptr);
                }
            }
        }

        // Update all IDs
        updateStageIDs();

        observer->emitUpdate();
        return true;
    }
    else
        return false;
}

void Mission::updateStageIDs()
{
    // Ensure all IDs match list
    for(int i = 0; i < stages.length(); i++)
        stages[i].setID(i+1);

    // Update next/previous IDs based on pointers
    for(Stage& stage : stages)
    {
        if(stage.getNextStage())
            stage.setNextStageID(stage.getNextStage()->getID());
        if(stage.getPreviousStage())
            stage.setPreviousStageID(stage.getPreviousStage()->getID());
    }
}
