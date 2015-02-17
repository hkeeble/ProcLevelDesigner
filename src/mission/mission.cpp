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
    RandomEngine rand;

    // Clear all stages of unlocked keys
    for(Stage& stage : stages)
        stage.clearUnlockedKeys();

    // Randomly arrange gates
    QList<Stage*> unassignedStages = getStages(); // Stages that have not been assigned a gate
    QList<Gate*> gateList = getGateList();
    for(Gate* gate : gateList)
    {
        Stage* stage = unassignedStages.at(rand.randomInteger(0, unassignedStages.length()-1));
        unassignedStages.removeOne(stage);
        stage->setExitGate(gate);
    }

    // Get a list of all unlocked keys
    QList<Key*> keyList = getKeyEventList();
    for(Stage& stage :stages)
    {
        for(Key* lockedKey : stage.getLockedKeys())
        {
            if(keyList.contains(lockedKey))
                keyList.removeOne(lockedKey);
        }
    }

    // Randomly arrange all unlocked key events
    QList<Stage*> stageList = getStages();
    QList<Stage*> possibleStages; // The list of stages the key can appear in
    for(Key* key : keyList)
    {
        // Get all the stages the key can legally be placed into
        possibleStages = QList<Stage*>();
        Stage* currentStage = stageList[0];
        bool loop = true;
        while(loop)
        {

            possibleStages.append(currentStage);

            if(currentStage->getExitGate()->getKeys().contains(key))
            {
                loop = false;
                break;
            }

            currentStage = currentStage->getNextStage();

            if(currentStage == nullptr)
            {
                loop = false;
                break;
            }
        }

        // Randomly select a stage for the key
        possibleStages[rand.randomInteger(0, possibleStages.length()-1)]->addKey(key);
    }

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
        mission.keyEvents.insert(key.getName(), Key(key));
    }

    for(Object* obj : gateObjects)
    {
        Gate gate = Gate::Parse(obj, mission.getKeys());
        mission.gates.insert(gate.getName(), Gate(gate));
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

QMap<QString,Key>* Mission::getKeys()
{
    return &keyEvents;
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
        gates.insert(name, Gate(gate));

        // Add a new stage due to the addition of a new gate
        Stage newStage = Stage(stages.count()-1, nullptr, nullptr, &gates.last(), QList<Key*>(), QList<Key*>(), false);

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
        keyEvents.insert(name, Key(key));
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
        Key key = keyEvents.take(name);

        // If the key event is being used by a gate, remove it
        for(QMap<QString,Gate>::iterator iter = gates.begin(); iter != gates.end(); iter++)
        {
            if(iter.value().getKeys().contains(&key))
                iter.value().removeKey(&key);
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

bool Mission::validate()
{
    bool isValid = true;

    QList<Key*> collectedKeys; // The keys that we have come across so far

    const Stage* currentStage = &stages.at(0);
    while(currentStage != nullptr)
    {
        collectedKeys.append(currentStage->getKeys()); // Add the keys for this stage
        QList<Key*> requiredKeys = currentStage->getExitGate()->getKeys();
        for(Key* reqKey : requiredKeys) // Check if the exit gate keys have all been collected by this point
        {
            if(!collectedKeys.contains(reqKey))
            {
                isValid = false;
                break;
            }
        }
        if(!isValid) // If mission has become invalid, break loop
            break;

        currentStage = currentStage->getNextStage();

    }

    return isValid;
}

bool Mission::validateAddition(Stage* stage, Key* key)
{
    const Stage* currentStage = &stages.at(0);
    while(currentStage != stage)
    {
        if(currentStage->getExitGate()->getKeys().contains(key))
        {
            return false;
        }
        currentStage = currentStage->getNextStage();
    }

    return true;
}

 bool Mission::doesKeyExist(Key* key)
 {
     const Stage* currentStage = &stages.at(0);
     while(currentStage != nullptr)
     {
         if(currentStage->getKeys().contains(key))
             return true;
         currentStage = currentStage->getNextStage();
     }
     return false;
 }
