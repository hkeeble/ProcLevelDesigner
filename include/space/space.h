#ifndef SPACE_H
#define SPACE_H

#include <QMap>
#include <QScopedPointer>
#include <QDebug>

#include "qpointext.h"
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

/*!
 * \brief Each cell contains a reference to the area contained within it. Null cells contain no area.
 */
class GridCell
{
public:
    GridCell() : area(nullptr) { }
    GridCell(Area* area) : area(area) { }

    inline Area* getArea() { return area; }

    inline void removeArea() { area = nullptr; }

private:
    Area* area;
};

/*!
 * \brief Represents the space structure for a mission. Contains all areas, and a grid showing their layout.
 */
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
    QList<Area>* getAreas() { return &areas; }

    /*!
     * \brief Places a new area at the given location in the grid.
     * \param x The X location of the top-left corner of the area.
     * \param y The Y location of the top-left corner of the area.
     */
    void placeArea(Area area);

    /*!
     * \brief Removes an area from the location given. If no area is found, returns false.
     * \param x The X location of the top-left corner of the area to remove.
     * \param y The Y location of the top-left corner of the area to remove.
     */
    bool removeArea(int x, int y);

    /*!
     * \brief Removes an area equivalent to the area given. If no area is removed, returns false.
     * \param The area to remove.
     */
    bool removeArea(Area area);

    /*!
     * \brief Retrieves the cell at the given location.
     */
    GridCell* getCell(int x, int y);

private:
    SpaceObserver* observer;

    QVector<QVector<GridCell>> cells;
    QList<Area> areas;

    QMap<QString,Zone> zones; /*!< Zones contained within this space. */
};

#endif // SPACE_H
