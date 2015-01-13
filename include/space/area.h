#ifndef AREA_H
#define AREA_H

#include <QPoint>

#include "filetools.h"
#include "link.h"
#include "key.h"
#include "zone.h"
#include "map.h"

static const int AREA_TILE_SIZE = 16; /*!< The tile dimensions of a single area (AREA_TILE_SIZE x AREA_TILE_SIZE tiles make up a single area) */

/*!
 * \brief Represents an individual cell within an area. Translates into a single in game tile when built.
 */
struct Cell
{
public:
    Cell() : Cell(QPoint(0,0), true, nullptr, nullptr) { }

    Cell(QPoint location, bool traversable, Key* key)
        : Cell(location, traversable, key, nullptr) { }

    Cell(QPoint location, bool traversable, Gate* gate)
        : Cell(location, traversable, nullptr, gate) { }

    Cell(QPoint location, bool traversable)
        : Cell(location, traversable, nullptr, nullptr) { }

    bool isTraversable() const { return traversable; }

    Key* getKey() const { return key; }
    Gate* getGate() const { return gate; }

    bool hasKey() const { return (key != nullptr); }
    bool hasGate() const { return (key != nullptr); }

    QPoint getLocation() const { return location; }

    static Cell Parse(Object* obj, QList<Key*> keys, QList<Gate*> gates);
    void build(Object* obj);

    bool operator==(const Cell& rhs) { return location == rhs.location && traversable == rhs.traversable; }

private:
    Cell(QPoint location, bool traversable, Key* key, Gate* gate)
        : location(location), traversable(traversable), key(key), gate(gate) { }

    bool traversable; // In future iterations, this could be a tilepattern as opposed to a traversable tile type
    QPoint location; // Location of this cell (for use in parse and build)
    Key* key;
    Gate* gate;
};

/*!
 * \brief An area is representative of an individual map in game. They are given a Zone pointer,
 *        which holds aesthetic data such as the tileset to be used by the area in game.
 *        Areas contain pointers to adjacent accessible areas, and their location on the grid
 *        contained by the Space object. They also contain a collection of the key events located
 *        in this area.
 */
class Area
{
public:
    Area();
    virtual ~Area();

    Area(Zone* zone, QPoint location, int height, int width, QList<Key*> keyEvents = QList<Key*>());

    Area(const Area& param);
    Area& operator=(const Area& param);

    static Area Parse(Object* obj, QString filePath, QList<Key*> keys, QList<Gate*> gates);
    void build(Object* obj, QString filePath);

    QPoint getLocation() { return location; }

    QList<Key*> getKeyEvents() { return keyEvents; }

    QString getZoneName() { return zoneName; }

    Zone* getZone() { return zone; }
    void setZone(Zone* zone) { this->zone = zone; }

    void setGrid(const QVector<QVector<Cell>>& grid) { this->grid = grid; }
    QVector<QVector<Cell>> getGrid() const { return grid; }

    int getWidth() { return width; }
    int getHeight() { return height; }

    Link* getLinkUp() { return up; }
    Link* getLinkLeft() { return left; }
    Link* getLinkRight() { return right; }
    Link* getLinkDown() { return down; }

    void setLinkUp(Link link) { up = new Link(link); }
    void setLinkLeft(Link link) { left = new Link(link); }
    void setLinkRight(Link link) { right = new Link(link); }
    void setLinkDown(Link link) { down = new Link(link); }

    void clearAllLinks();

    void removeRightLink();
    void removeLeftLink();
    void removeDownLink();
    void removeUpLink();

    /*!
     * \brief Build the map object for this area, making use of the grid, key events, gates and links.
     * \return
     */
    Map buildMap();

    bool operator==(const Area rhs);

private:
    void cpy(const Area& param); /*!< Internal copying function. */

    void parseGrid(QString filePath, QList<Key*> keys, QList<Gate*> gates);           /*!< Parse this area's grid data. */
    void buildGrid(QString filePath);           /*!< Build this area's grid. */

    void initGrid(int width, int height);       /*!< Initializes the grid with the given width and height values. */

    QString zoneName;                           /*!< Name of the zone this area should point to (only use this when parsing!) */
    Zone* zone;                                 /*!< The zone this area belongs to. */
    QPoint location;                            /*!< The grid location of this area. */
    QVector<QVector<Cell>> grid;                /*!< The grid of cells in this area. */
    int height, width;                          /*!< The height and width of this area. */
    QList<Key*> keyEvents;                      /*!< The key events that take place in this area. */
    Link *up, *left, *right, *down;             /*!< This area's links to other areas. */
};
#endif // AREA_H
