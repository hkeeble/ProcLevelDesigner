#include "preferences.h"

const int Preferences::MAX_RECENT_QUESTS = 5;

Preferences::Preferences()
{
    recentQuestMenu = nullptr;
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
        data->getObject(OBJ_PREFERENCES)->data.insert(ELE_SOLARUS_PATH, Data("DEFAULT"));
    }

    Object* prefs = data->getObject(OBJ_PREFERENCES);
    preferences.setSolarusPath(prefs->find(ELE_SOLARUS_PATH, "DEFAULT"));

    QList<Object*> recents = data->getObjectsOfName(OBJ_RECENT_QUEST);
    for(Object* object : recents)
        preferences.addRecentQuestPath(object->find(ELE_PATH, ""));

    return preferences;
}

void Preferences::setAsRecentQuestManager(QObject* receiver, QMenu* recentQuestMenu, const char* openSlot)
{
    this->recentQuestMenu = recentQuestMenu;
    recentQuestActions = QList<QAction*>();

    for(QString recentPath : recentQuestPaths)
    {
        QAction* action = recentQuestMenu->addAction(recentPath);
        action->setData(recentPath);
        recentQuestActions.append(action);
        QObject::connect(recentQuestMenu, SIGNAL(triggered(QAction*)), receiver, openSlot, Qt::UniqueConnection);
    }
}

void Preferences::build(Table* data)
{
    data->clear();

    Object prefs = Object();
    prefs.insert(ELE_SOLARUS_PATH, solarusPath);
    data->addObject(OBJ_PREFERENCES, prefs);

    for(QString recent : recentQuestPaths)
    {
        Object obj = Object();
        obj.insert(ELE_PATH, recent);
        data->addObject(OBJ_RECENT_QUEST, obj);
    }
}

void Preferences::addRecentQuestPath(QString path)
{
    if(!recentQuestPaths.contains(path))
    {
        recentQuestPaths.append(path);

        // If we have a recent quest menu, update it with the new quest.
        if(recentQuestMenu)
        {
            QAction* action = recentQuestMenu->addAction(path);
            action->setData(path);
            recentQuestActions.append(action);
        }

        if(recentQuestPaths.length() > MAX_RECENT_QUESTS)
            removeRecentQuestPath(recentQuestPaths[0]);
    }
}

bool Preferences::removeRecentQuestPath(QString path)
{
    // Remove the action from the menu
    if(recentQuestMenu != nullptr)
    {
        QAction* rmAction = nullptr;
        for(QAction* action : recentQuestActions)
        {
            if(action->data().toString() == path)
                rmAction = action;
        }
        if(rmAction != nullptr)
            recentQuestMenu->removeAction(rmAction);
    }

    return recentQuestPaths.removeOne(path);
}

Preferences::~Preferences()
{

}
