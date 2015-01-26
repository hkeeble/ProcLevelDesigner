#ifndef GAMEMANAGERSCRIPT_H
#define GAMEMANAGERSCRIPT_H

#include "script.h"

class GameManagerScript : public Script
{
public:
    GameManagerScript();

    void setStartingMap(QString mapID) { startingMap = mapID; }

    virtual void writeToFile(QFile& file) override final;

private:
    int maxLife;
    int initialLife;
    QString startingMap;

    QMap<QString,int> abilities;
};

#endif // GAMEMANAGERSCRIPT_H
