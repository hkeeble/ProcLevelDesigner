#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QMenu>
#include <QAction>

#include "filetools.h"

/*!
 * \brief Represents program preferences.
 */
class Preferences
{
public:
    Preferences();
    virtual ~Preferences();

    /*!
     * \brief Parses preferences from the given table.
     */
    static Preferences Parse(Table* data);

    /*!
     * \brief builds the preferences into the given table.
     */
    void build(Table* data);

    /*!
     * \brief Sets this object to manage the recent quests menu given.
     * \param recentQuestMenu The menu to fill with recent quest links.
     * \param openSlot The slot used to open these recent quests.
     */
    void setAsRecentQuestManager(QObject* receiver, QMenu* recentQuestMenu, const char* openSlot);

    void setSolarusPath(QString solarusPath) { this->solarusPath = solarusPath; }
    inline QString getSolarusPath() { return solarusPath; }

    QStringList getRecentQuestPaths() { return recentQuestPaths; }
    void addRecentQuestPath(QString path);
    bool removeRecentQuestPath(QString path);

private:
    static const int MAX_RECENT_QUESTS; /*!< The maximum number of recent quests to remember. */

    QList<QAction*> recentQuestActions;
    QMenu* recentQuestMenu;

    QString solarusPath;
    QStringList recentQuestPaths;
};

#endif // PREFERENCES_H
