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
#include "teletransporter.h"
#include "randomengine.h"
#include "direction.h"
#include "destination.h"

class Space; // Forward declare space

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

class SpaceReceiver : public QObject
{
    Q_OBJECT
public:
    SpaceReceiver(Mission* mission, Space* space) : space(space) { connect(mission->getObserver(), SIGNAL(updated()), this, SLOT(missionUpdated())); }

public slots:
    void missionUpdated();

private:
    Space* space;
};

/*!
 * \brief Each cell contains an area origin representing the origin of the area contained within it. This can be used as a key to access to area
 *        within the space map. Some cells will contain no area.
 */
class GridCell
{
public:
    GridCell() : areaOrigin(QPoint(0,0)), hasArea(false) { }
    GridCell(QPoint areaOrigin) : areaOrigin(areaOrigin), hasArea(true) { }

    inline bool containsArea() { return hasArea; }
    inline QPoint getAreaOrigin() { return areaOrigin; }
    inline void setArea(QPoint areaOrigin) { this->areaOrigin = areaOrigin; hasArea = true; }
    inline void removeArea() { areaOrigin = QPoint(0,0); hasArea = false; }

private:
    QPoint areaOrigin;
    bool hasArea;
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
    void generate(Mission& mission);

    /*!
     * \brief Build a new space object from the given data table.
     */
    static Space Parse(Table* data, QList<Gate*> gates, QList<Key*> keys, QList<Tileset*> tilesets);

    /*!
     * \brief Returns a list of all zone and area data, built into tables.
     */
    void build(Table* data);

    /*!
     * \brief Builds map objects out of the current space.
     */
    QList<Map> buildMaps();

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
     * \brief Removes an area from the location given. If no area is found, returns false.
     * \param areaOrigin The origin of the area to remove.
     */
    bool removeArea(QPoint areaOrigin) { return removeArea(areaOrigin.x(), areaOrigin.y()); }

    /*!
     * \brief Removes an area equivalent to the area given. If no area is removed, returns false.
     * \param The area to remove.
     */
    bool removeArea(Area area);

    /*!
     * \brief Retrieves the cell at the given location.
     */
    GridCell* getCell(int x, int y);

    /*!
     * \brief Retrieve the width of the space grid.
     */
    int getWidth() { return cells.length(); }

    /*!
     * \brief Retrieve the height of the space grid.
     */
    int getHeight() { if(cells.length() == 0) return 0; else return cells[0].length(); }

    /*!
     * \brief Clears all space data.
     */
    void clear();

    /*!
     * \brief Set the starting area in this space. Returns false if the area does not exist (has not been added to the areas) or
     *        if the position is an invalid one. If false is returned, the function has done nothing.
     * \param areaOrigin The origin of the area in which the player will start.
     * \param position The position (in cell coordinates) that the player will begin at.
     */
    bool setStartingArea(QPoint areaOrigin, QPoint position);

    /*!
     * \brief Retrieve the starting area.
     */
    QPoint getStartingArea() { return startingArea; }

    /*!
     * \brief Will set the mission for this space to observe for changes. When the mission changes, we want to clear the space.
     * \param mission
     */
    void setMission(Mission* mission);

    /*!
     * \brief Checks whether or not space is currently clear of any areas.
     */
    bool isClear() { return areas.size() == 0; }

    void missionUpdated();

private:

    void copy(const Space& param); /*!< Internal deep copy helper function. */

    SpaceObserver* observer; /*!< The observer is used to inform the space scene whenever the space changes. */
    SpaceReceiver* receiver; /*!< The receiver receieves messages from the mission, to inform space when mission has changed. */

    QVector<QVector<GridCell>> cells; /*!< The grid of cells representing this area. Each cell contains (or does not contain) a reference to
                                           the area that it holds. */

    // Starting area and location in that area
    QPoint startingArea, startingLocation;

    QMap<QPoint,Area> areas; /*!< Map containing all areas contained by this space. The location represents the origin of the area. */

    QMap<QString,Zone> zones; /*!< Zones contained within this space. */
};

#endif // SPACE_H
