#ifndef AREA_H
#define AREA_H

#include <QPoint>

#include "filetools.h"
#include "link.h"
#include "key.h"
#include "zone.h"
#include "map.h"
#include "spacegenerationoptions.h"
#include "randomengine.h"

static const int AREA_TILE_SIZE = 8; /*!< The tile dimensions of a single area (AREA_TILE_SIZE x AREA_TILE_SIZE tiles make up a single area) */

/*!
 * \brief Represents an individual cell within an area. Translates into a single in game tile when built.
 */
struct Cell
{
public:
    Cell() : Cell(QPoint(0,0), true, nullptr, nullptr) { }

    Cell(QPoint location) : Cell(location, true, nullptr, nullptr) { }

    Cell(QPoint location, bool traversable, Key* key)
        : Cell(location, traversable, key, nullptr) { }

    Cell(QPoint location, bool traversable, Gate* gate)
        : Cell(location, traversable, nullptr, gate) { }

    Cell(QPoint location, bool traversable)
        : Cell(location, traversable, nullptr, nullptr) { }

    void setTraversable(bool traversable) { this->traversable = traversable; }
    bool isTraversable() const { return traversable; }

    void setKey(Key* key) { this->key = key; }
    void setGate(Gate* gate) { this->gate = gate; }
    Key* getKey() const { return key; }
    Gate* getGate() const { return gate; }

    bool hasKey() const { return (key != nullptr); }
    bool hasGate() const { return (gate != nullptr); }

    QPoint getLocation() const { return location; }
    void setLocation(QPoint location) { this->location = location; }

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

/**
 * @brief Represents a grid of cells stored within an area.
 */
class Grid
{
public:
    Grid();
    Grid(int width, int height);
    Grid(QVector<QVector<Cell>> cells);

    ~Grid() { }

    bool operator==(const Grid& rhs);

    Cell& getCell(int x, int y) { return cells[x][y]; }
    void setCell(Cell cell, int x, int y);

    void setCellGate(Gate* gate, int x, int y) { cells[x][y].setGate(gate); }
    void setCellKey(Key* key, int x, int y) { cells[x][y].setKey(key); }
    void setCellTraversable(bool traversable, int x, int y) { cells[x][y].setTraversable(traversable); }

    Gate* getCellGate(int x, int y) { return cells[x][y].getGate(); }
    Key* getCellKey(int x, int y) { return cells[x][y].getKey(); }
    bool cellHasGate(int x, int y) { return cells[x][y].hasGate(); }
    bool cellHasKey(int x, int y) { return cells[x][y].hasKey(); }
    bool isCellTraversable(int x, int y) { return cells[x][y].isTraversable(); }


    int getWidth() { return width; }
    int getHeight() { return height; }

    void build(Table* table);
    static Grid Parse(Table* table, int width, int height, QList<Key*> keys, QList<Gate*> gates);

private:
    int width, height;
    QVector<QVector<Cell>> cells;
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

    Area(int stageID, Zone* zone, QPoint location, int width, int height);
    Area(int stageID, Zone* zone, int width, int height);

    Area(const Area& param);
    Area& operator=(const Area& param);

    static Area Parse(Object* obj, QString filePath, QList<Key*> keys, QList<Gate*> gates);
    void build(Object* obj, QString filePath);

    QPoint getLocation() const { return location; }

    QList<Key*> getKeyEvents() { return keyEvents; }

    QList<Gate*> getGates() { return gates; }

    QString getZoneName() { return zoneName; }

    Zone* getZone() { return zone; }
    void setZone(Zone* zone) { this->zone = zone; }

    void setGrid(Grid grid) { this->grid = grid; }
    Grid* getGrid() { return &grid; }

    Cell getCell(int x, int y) { return grid.getCell(x, y); }
    void setCell(int x, int y, Cell cell) { grid.setCell(cell, x, y); }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    int getGridWidth() { return grid.getWidth(); }
    int getGridHeight() { return grid.getHeight(); }

    int getStageID() { return stageID; }

    /*!
     * \brief addLink Add a new link to this area.
     * \param location The location relative to this area (the cell coordinate) where this link is.
     * \param targetOrigin The origin of the target area of this link.
     * \param direction The direction of the link.
     */
    void addLink(const QPoint& location, const QPoint& targetOrigin, const Direction& direction);

    void addLink(const Link& link);

    QList<Link*>& getLinks() { return links; }

    void clearAllLinks();

    /*!
     * \brief Add a key event to this area. Returns false if the location given was invalid, and the key is not added to the area.
     *        The area does not take ownership over the key.
     * \param key The key to add to this area.
     * \param x The x position (in tile coordinates) of the key event in the area.
     * \param y The y position (in tile coordinates) of the key in the area.
     */
    bool addKeyEvent(Key* key, int x, int y);

    /*!
     * \brief Add a gate to this area. Returns false if the location given was invalid, and the gate is not added to the area.
     *        The area does not take ownership over the gate.
     * \param gate The gate to add to this area.
     * \param x The x position (in tile coordinates) of the gate event in the area.
     * \param y The y position (in tile coordinates) of the gate in the area.
     */
    bool addGate(Gate* gate, int x, int y);

    /*!
     * \brief Build the map object for this area, making use of the grid, key events, gates and links.
     * \return
     */
    Map buildMap();

    bool operator==(const Area rhs);
    bool operator!=(const Area rhs) { return !(*this == rhs); }

    /*!
     * \brief Set the location of this area. Do not change area location if it exists in a space already.
     * \param location The location of the area.
     */
    void setLocation(QPoint location) { this->location = location; }

    /*!
     * \brief addWall Add a wall along the given side of this area, at the given x,y relative location.
     * \param x The X location, relative to this area's origin.
     * \param y The Y location, relative to this area's origin.
     * \param direction Direction representing the side of the tile to add the wall.
     */
    void addWall(int x, int y, Direction direction);

private:
    void cpy(const Area& param);                /*!< Internal copying function. */

    void initGrid(int width, int height);       /*!< Initializes the grid with the given width and height values. */

    QString zoneName;                           /*!< Name of the zone this area should point to (only use this when parsing!) */
    Zone* zone;                                 /*!< The zone this area belongs to. */
    QPoint location;                            /*!< The grid location of this area. */

    Grid grid;                                  /*!< The grid of cells in this area. */

    int height, width;                          /*!< The height and width of this area. */
    int stageID;                                /*!< The mission stage that this area belongs to. */
    QList<Key*> keyEvents;                      /*!< The key events that take place in this area. */
    QList<Gate*> gates;                         /*!< The gates contained in this area. */
    QList<Link*> links;                         /*!< This area's links to other areas. */
};

/*!
 * \brief The AreaFactory class contains some helper functions to generate areas.
 */
class AreaFactory
{
public:
    virtual ~AreaFactory() { }

    /*!
     * \brief RandomArea Generate a random area.
     * \param zone The zone to assign to the area.
     * \param minX The minimum x location of the area.
     * \param maxX The maximum x location of the area.
     * \param minY The minimum y location of the area.
     * \param maxY The maximum y location of the area.
     * \param options The space generation options to use.
     */
    static Area RandomArea(int stageID, Zone* zone, int minX, int maxX, int minY, int maxY, const SpaceGenerationOptions& options);

    /*!
     * \brief RandomArea
     * \param zone The zone to assign to the area.
     * \param x The x location of the area.
     * \param y The y location of the area.
     * \param options The space generation options to use.
     * \return
     */
    static Area RandomArea(int stageID, Zone* zone, int x, int y, const SpaceGenerationOptions& options);

private:
    AreaFactory() { }
};

#endif // AREA_H
