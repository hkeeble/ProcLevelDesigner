#ifndef HERO_H
#define HERO_H

#include <QMap>

#include "filetools.h"

class Hero
{
public:
    enum Ability
    {
       Lift,
       Sword,
       SwordKnowledge,
       Tunic,
       Shield,
       Swim,
       Run,
       DetectWeakWalls,
       COUNT
    };

    Hero();

    void setAbility(Ability ability, int value);
    int getAbilityValue(Ability ability) const;

    QMap<Ability,int>* getAbilities() { return &abilities; }

    void setStartingMap(QString mapID) { this->startingMap = mapID; }
    QString getStartingMap() const { return startingMap; }

    void setMaxLife(int maxLife) { this->maxLife = maxLife; }
    int getMaxLife() const { return maxLife; }

    void setInitialLife(int initialLife) { this->initialLife = initialLife; }
    int getInitialLife() const { return initialLife; }

    void build(Table* table);
    static Hero Parse(Table* table);

private:
    QMap<Ability,int> abilities;

    QString startingMap;
    int maxLife, initialLife;
};

/*!
    Use this array to convert from an ability enum into a usable string that game script files understand.
  */
const QString ABILITY_STRINGS[Hero::Ability::COUNT] {
    "lift", "sword", "sword_knowledge", "tunic", "shield",
    "swim", "run", "detect_weak_walls"
};

/*!
    Use this array to convert from an ability enum into a formatted string that can be used for display purposes.
  */
const QString ABILITY_STRINGS_FORMATTED[Hero::Ability::COUNT] {
    "Lift", "Sword", "Sword Knowledge", "Tunic", "Shield",
    "Swim", "Run", "Detect Weak Walls"
};


#endif // HERO_H
