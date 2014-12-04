#ifndef ZONE_H
#define ZONE_H

#include <QPoint>
#include <QList>

#include "gate.h"
#include "key.h"
#include "tileset.h"

// Forward declarations of classes that make up a zone
class Link;
class Zone;
class Area;

class Link
{
public:
    Link() { };
    virtual ~Link() { };

    static Link Parse(Object* obj) { };
    void build(Object* obj) { };

private:
    QPoint first, second;
    Gate* gate;     /*!< The gate attached to this link. Null pointer means there is not gate. */
};

/*!
 * \brief An area is representative of an individual map in game, several areas compose a Zone.
 *        Areas contain pointers to adjacent accessible areas, and their location on the grid
 *        contained by the Space object their Zone resides in. They also contain a collection
 *        of the key events located in this area.
 */
class Area
{
public:
    Area() { };
    virtual ~Area() { };

    static Area Parse(Object* obj) { };
    void build(Object* obj) { };

private:
    QPoint location;             /*!< The grid location of this area. */
    QList<Key*> keyEvents;       /*!< The key events that take place in this area. */
    Link* up, left, right, down; /*!< This area's links to other areas. */
};

/*!
 * \brief A zone represents a stage of a mission in physical space in the game level. It contains
 *        various areas, each of which is it's own in game map. The links held by a zone are representative
 *        of mission gates.
 */
class Zone
{
public:
    Zone();
    Zone(int areaCount);
    Zone(Tileset* tileset);
    Zone(Tileset* tileset, int areaCount);
    Zone(Tileset* tileset, int areaCount, QList<Area> areas);
    virtual ~Zone();

    Tileset* getTileset() { return tileset; }
    void setTileset(Tileset* tileset) { this->tileset = tileset; }

    inline QString getName() { return name; }
    inline int getAreaCount() { return areaCount; }

    static Zone Parse(Table* data);
    void build(Table* data);

private:
    QString name;             /*!< The name of this zone. */
    int areaCount;            /*!< The maximum number of areas that can be within this zone. */
    Tileset* tileset;         /*!< The tileset used by this zone. */
    QMap<QPoint,Area> areas;  /*!< The areas contained by this zone. */
};

#endif // ZONE_H
