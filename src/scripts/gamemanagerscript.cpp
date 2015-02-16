#include "gamemanagerscript.h"

GameManagerScript::GameManagerScript()
{
    returnStatement = "return game_manager";
}

void GameManagerScript::writeToFile(QFile& file)
{
    // Construct all variables/functions
    variables.insert("game_manager", Variable("game_manager", "{}"));
    variables.insert("exists", Variable("exists", "sol.game.exists(\"save1.dat\")"));
    variables.insert("game", Variable("game", "sol.game.load(\"save1.dat\")"));

    addPriorityVariable("game_manager");
    addPriorityVariable("exists");
    addPriorityVariable("game");

    Function func = Function("game_manager:start_game");

    func.addLine("if not exists then");
    func.addLine("game:set_max_life(" + QString::number(hero.getMaxLife()) + ")");
    func.addLine("game:set_starting_location(" + hero.getStartingMap() + ")");
    func.addLine("game:set_life(" + QString::number(hero.getInitialLife()) + ")");


    QMap<Hero::Ability,int>* abilities = hero.getAbilities();
    for(auto iter = abilities->begin(); iter != abilities->end(); iter++)
        func.addLine("game:set_ability(\"" + ABILITY_STRINGS[iter.key()] + "\", " + QString::number(iter.value()) + ")");

    func.addLine("end");

    func.addLine("game:start()");

    functions.insert(func.getName(), func);

    // Write to file
    Script::writeToFile(file);
}
