#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>

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
     * \brief Builds the preferences into the given table.
     */
    void Build(Table* data);

    void setSolarusPath(QString solarusPath) { this->solarusPath = solarusPath; }
    inline QString getSolarusPath() { return solarusPath; }

    QStringList getRecentQuestPaths() { return recentQuestPaths; }
    void addRecentQuestPath(QString path) { recentQuestPaths.append(path); }
    bool removeRecentQuestPath(QString path) { recentQuestPaths.removeOne(path); }

    void saveToDisk() { data->saveToDisk(); }

private:
    Table* data;
    QString solarusPath;
    QStringList recentQuestPaths;
};

#endif // PREFERENCES_H
