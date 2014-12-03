#include "mission.h"

Mission::Mission()
{
    stages = QList<Stage*>();
    observer = new MissionObserver();
}

Mission::~Mission()
{
    for(Stage* stage : stages)
        delete stage;
}

void Mission::generate()
{
    stages.clear();

    // Obtain keys and gates
    QList<Gate*> gates = getGateList();
    QList<Key*> keys = getKeyEventList();

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

        stages[i]->addKey(new Key("Dummy Key", Key::Type::Switch));
    }

    qDebug() << "Mission Structure: ";
    QDebug debug = qDebug();
    for(Stage* stage : stages)
    {
        debug << "Stage " + QString::number(stage->getID()) << " -> " << (stage->getNextGate() == nullptr ? "" : stage->getNextGate()->getName() + " -> ");
    }

    observer->emitUpdate();
}

Mission Mission::Parse(Table* data)
{
    Mission mission = Mission();
    QList<Object*> keyObjects = data->getObjectsOfName(OBJ_KEY_EVENT);
    QList<Object*> gateObjects = data->getObjectsOfName(OBJ_GATE);

    // Clear any existing data
    mission.gates.clear();
    mission.keyEvents.clear();

    // Parse all objects, and insert them into the collection
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

    mission.getObserver()->emitUpdate();

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
        return true;
    }
    else
        return false;
}

bool Mission::removeGate(QString name)
{
    if(gates.contains(name))
    {
        gates.remove(name);
        return true;
    }
    else
        return false;
}
