#ifndef SPACE_H
#define SPACE_H

#include <QMap>

#include "filetools.h"
#include "zone.h"

/*!
 * \brief Observes a space object, and is used to emit a signal when space has changed to inform all slots to update.
 */
class SpaceObserver : public QObject
{
    Q_OBJECT

public:
    SpaceObserver() { }

    void emitUpdate() { emit updated(); }

signals:
    void updated();
};

class Space
{
public:
    Space();
    virtual ~Space();

    /*!
     * \brief Build a new space object from the given data table.
     */
    static Space Parse(Table* data);

    /*!
     * \brief Build this space into the given data table.
     */
    void build(Table* data);

    /*!
     * \brief Emit an update informing any slots watching this space to update.
     */
    void emitUpdate() { observer->emitUpdate(); }

    /*!
     * \brief Obtain the object observing this space.
     */
    SpaceObserver* getObserver() { return observer; }

    /*!
     * \brief Add a new zone to this space. If the name already exists, returns false and does not add the zone.
     */
    bool addZone(QString name, Zone zone);

    /*!
     * \brief Removes a zone from this space. If the name was not found, returns false.
     */
    bool removeZone(QString name);

    QList<Zone*> getZoneList();

private:
    SpaceObserver* observer;
    QMap<QString,Zone> zones; /*!< Zones contained within this space. */
};

#endif // SPACE_H
