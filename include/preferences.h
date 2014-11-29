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

    void setSolarusPath(QString solarusPath) { this->solarusPath = solarusPath; }
    inline QString getSolarusPath() { return solarusPath; }

    void saveToDisk() { data->saveToDisk(); }

private:
    Table* data;
    Object* prefs;
    QString solarusPath;
};

#endif // PREFERENCES_H
