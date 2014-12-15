#ifndef SPACE_H
#define SPACE_H

#include <QMap>
#include <QScopedPointer>
#include <QDebug>

#include "filetools.h"
#include "zone.h"
#include "area.h"
#include "mission.h"

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
     * \brief Generate space for the given mission.
     * \param mission The mission to generate space for.
     */
    void generate(const Mission& mission);

    /*!
     * \brief Build a new space object from the given data table.
     */
    static Space Parse(Table* data, QList<Gate*> gates, QList<Key*> keys, QList<Tileset*> tilesets);

    /*!
     * \brief Returns a list of all zone data, built into tables.
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

    /*!
     * \brief Get the zone with the given name. If the name was not found, returns a null pointer.
     */
    Zone* getZone(QString name);

    /*!
     * \brief Returns a list of pointers to all zones included in this space.
     */
    QList<Zone*> getZoneList();

    /*!
     * \brief Returns a list of the areas in this space.
     */
    QMap<QPoint,Area>* getAreas() { return &areas; }

private:

    SpaceObserver* observer;

    QMap<QPoint,Area> areas; /*!< Areas contained within this space. */
    QMap<QString,Zone> zones; /*!< Zones contained within this space. */
};

#endif // SPACE_H
