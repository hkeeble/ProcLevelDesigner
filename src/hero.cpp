#include "hero.h"

Hero::Hero()
{
    startingMap = "";
    initialLife = 12;
    maxLife = 12;

    setAbility(Lift, 2);
    setAbility(Sword, 1);
    setAbility(SwordKnowledge, 0);
    setAbility(Tunic, 1);
    setAbility(Shield, 0);
    setAbility(Swim, 0);
    setAbility(Run, 0);
    setAbility(DetectWeakWalls, 0);
}

void Hero::setAbility(Ability ability, int value)
{
    if(abilities.contains(ability))
    {
        int& val = abilities.find(ability).value();
        val = value;
    }
    else
        abilities.insert(ability, value);
}

int Hero::getAbilityValue(Ability ability) const
{
    if(abilities.contains(ability))
        return abilities.find(ability).value();
    else
        return 0;
}

void Hero::build(Table* table)
{
    table->clear();

    // Add main hero object
    Object hero;
    hero.insert(ELE_MAX_LIFE, QString::number(maxLife));
    hero.insert(ELE_INITIAL_LIFE, QString::number(initialLife));
    hero.insert(ELE_STARTING_MAP, startingMap);

    table->addObject(OBJ_HERO, hero);

    // Add abilities
    for(auto iter = abilities.begin(); iter != abilities.end(); iter++)
    {
        Object obj;
        obj.insert(ELE_ID, QString::number(static_cast<int>(iter.key())));
        obj.insert(ELE_VALUE, QString::number(iter.value()));
        table->addObject(OBJ_ABILITY, obj);
    }
}

Hero Hero::Parse(Table* table)
{
    Hero hero;

    Object* heroObject = table->getObject(OBJ_HERO);
    QList<Object*> abilities = table->getObjectsOfName(OBJ_ABILITY);

    if(heroObject)
    {
        hero.maxLife = heroObject->find(ELE_MAX_LIFE, "12").toInt();
        hero.initialLife = heroObject->find(ELE_INITIAL_LIFE, QString::number(hero.maxLife)).toInt();
        hero.startingMap = heroObject->find(ELE_STARTING_MAP, NULL_ELEMENT);
    }

    // Parse abilities
    for(Object* ability : abilities)
        hero.setAbility(static_cast<Ability>(ability->find(ELE_ID, "0").toInt()), ability->find(ELE_VALUE, "0").toInt());

    return hero;
}
