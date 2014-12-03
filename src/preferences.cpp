#include "preferences.h"

Preferences::Preferences()
{

}

Preferences Preferences::Parse(Table* data)
{
    Preferences preferences;

    // Create/read preferences (if they do not exist, set some defaults)
    if(data->isEmpty())
    {
        Object obj = Object();
        obj.insert(ELE_SOLARUS_PATH, "DEFAULT");
        data->addObject(OBJ_PREFERENCES, obj);
    }
    else if(data->getObject(OBJ_PREFERENCES)->data.isEmpty()) // Insert default solarus path if not found
    {
        data->getObject(OBJ_PREFERENCES)->data.insert(ELE_SOLARUS_PATH, "DEFAULT");
    }

    Object* prefs = data->getObject(OBJ_PREFERENCES);
    preferences.setSolarusPath(prefs->find(ELE_SOLARUS_PATH, "DEFAULT"));

    QList<Object*> recents = data->getObjectsOfName(OBJ_RECENT_QUEST);
    for(Object* object : recents)
        preferences.addRecentQuestPath(object->find(ELE_PATH, ""));

    return preferences;
}

void Preferences::Build(Table* data)
{
    data->clear();

    Object prefs = Object();
    prefs.insert(ELE_SOLARUS_PATH, solarusPath);
    data->addObject(OBJ_PREFERENCES, prefs);

    for(QString recent : recentQuestPaths)
    {
        Object obj = Object();
        obj.insert(ELE_PATH, recent);
        data->addObject(obj);
    }
}

Preferences::~Preferences()
{

}
