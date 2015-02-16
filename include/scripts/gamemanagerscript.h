#ifndef GAMEMANAGERSCRIPT_H
#define GAMEMANAGERSCRIPT_H

#include "script.h"
#include "hero.h"

class GameManagerScript : public Script
{
public:
    GameManagerScript();

    void setHero(const Hero& hero) { this->hero = hero; }

    virtual void writeToFile(QFile& file) override final;

private:
    Hero hero; /*!< Hero data to be used by this script. */
};

#endif // GAMEMANAGERSCRIPT_H
