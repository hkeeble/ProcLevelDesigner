#include "preferences.h"

Preferences::Preferences()
{
    data = new Table(DAT_PREFERENCES);
    if(data->isEmpty())
    {
        Object obj = Object();
        obj.insert(ELE_SOLARUS_PATH, "DEFAULT");
        data->addObject(OBJ_PREFERENCES, obj);
    }
    else if(data->getObject(OBJ_PREFERENCES)->data.isEmpty())
    {
        data->getObject(OBJ_PREFERENCES)->data.insert(ELE_SOLARUS_PATH, "DEFAULT");
    }

    prefs = data->getObject(OBJ_PREFERENCES);

    solarusPath = prefs->find(ELE_SOLARUS_PATH, "DEFAULT");

    data->saveToDisk();
}

Preferences::~Preferences()
{
    delete data;
}
