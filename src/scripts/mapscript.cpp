#include "mapscript.h"

MapScript::MapScript()
{
    functions = QMap<QString,Function>();
    variables = QMap<QString,Variable>();

    functions.insert("map:on_started", Function("map:on_started"));
    functions.insert("map:on_update", Function("map:on_update"));

    variables.insert("map", Variable("map", "..."));
    variables.insert("game", Variable("game", "map:get_game()"));

    addPriorityVariable("map");
    addPriorityVariable("game");
}

void MapScript::addSwitch(SwitchEntity switchEntity)
{
    // Create on active function for this script
    Function onActivate = Function(switchEntity.getName() + ":on_activated");

    // Create on_active function contents
    onActivate.addLine("game:set_value(\"" + switchEntity.getName() + "Active\",true)\n");

    // Add to functions
    functions.insert(onActivate.getName(), onActivate);

    // Create and add switch variable to map
    Variable switchVar = Variable(switchEntity.getName(), "map:get_entity(\"" + switchEntity.getName() + "\")");

    // Add the switch variable to the script
    variables.insert(switchVar.getName(), switchVar);
}

void MapScript::addDoor(Door door, QList<Key*> keys)
{
    // Construct the neccesary script
    QString ifStatement;

    if(keys.size() > 0)
    {
        ifStatement += "if(";
        for(int i = 0; i < keys.size(); i++)
        {
            ifStatement += "game:get_value(\"" + keys[i]->getName() + "Active\") == true";
            if(i != keys.size()-1)
                ifStatement += " and ";
            else
                ifStatement += ") then";
        }
    }

    QString result = "map:open_doors(\"" + door.getName() + "\")";
    QString end = "end";

    // Add the script to all neccesary functions

    // The on_started function is used to check, when the map opens, if the required keys have been activated
    Function& onStarted = functions.find("map:on_started").value();

    // The update function checks on each frame if the switch has been activated (this is neccessary for maps on which the keys and gates are
    // both located)
    Function& onUpdate = functions.find("map:on_update").value();

    onStarted.addLine(ifStatement);
    onStarted.addLine(result);
    onStarted.addLine(end);

    onUpdate.addLine(ifStatement);
    onUpdate.addLine(result);
    onUpdate.addLine(end);
}
