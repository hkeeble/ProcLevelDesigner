#ifndef MAP_H
#define MAP_H

#include <QString>
#include <QVector>
#include <QDebug>

#include "filetools.h"
#include "tileset.h"
#include "mapentity.h"
#include "door.h"
#include "mapscript.h"

const int DEFAULT_TILE_SIZE = 32;
const int DEFAULT_MAP_SIZE = DEFAULT_TILE_SIZE * 10;
const QString DEFAULT_MAP_NAME = "NULL_MAP";
const QString DEFAULT_MAP_TILESET = "main";
const QString DEFAULT_MAP_MUSIC = "village";
const QString DEFAULT_MAP_WORLD = "outside";

/*!
 * \brief Represents a tile within a map.
 */
struct MapTile
{
public:
    MapTile() { }
    MapTile(int layer, int X, int Y, int size, int pattern) :
        layer(layer), x(X), y(Y), size(size), pattern(pattern) { }

    static MapTile parse(Object* object); /*!< Parses a map tile from a given data object. */
    static Object build(MapTile tile); /*!< builds an object from a map tile. */

    // Getters
    inline int getLayer() const        { return layer; }
    inline int getX() const            { return x; }
    inline int getY() const            { return y; }
    inline int getSize() const         { return size; }
    inline int getPattern() const      { return pattern; }

    // Setters
    inline void setLayer(int layer)     { this->layer = layer; }
    inline void setX(int x)             { this->x = x; }
    inline void setY(int y)             { this->y = y; }
    inline void setSize(int size)       { this->size = size; }
    inline void setPattern(int pattern) { this->pattern = pattern; }

private:
    int layer, x, y, size, pattern;
};

/*!
 * \brief The Map class. Represents a game map internally.
 */
class Map
{
public:
    Map();
    Map(int tileSize, int width, int height);
    Map(QString name, int width, int height, int tileSize, QString music, QString world);
    Map(QString name, int width, int height, int tileSize, QString music, QString world, Tileset* tileset);

    virtual ~Map();

    Map& operator=(const Map& rhs);
    Map(const Map& param);

    /*!
     * \brief Parses a map from the table into this object.
     * \param data The table containing the map data.
     * \param tileSet The data for the tileset to be used by this map.
     * \return Copy of the map that was created.
     */
    static Map parse(QString name, Table* data);

    /*!
     * \brief build the map, creating a table containing all of it's contents.
     * \param table The pointer to the table to build the map data into.
     */
    void build(Table* table);

    inline QString getName() const     { return name; }
    inline Tileset* getTileSet() const  { return tileSet; }
    inline QString getMusic() const    { return music; }

    inline int getWidth() const         { return width; }
    inline int getHeight() const        { return height; }
    inline int getTileSize() const      { return tileSize; }

    inline void setName(const QString& name)          { this->name = name; }
    inline void setTileSet(Tileset* tileSet)          { this->tileSet = tileSet; }
    inline void setMusic(const QString& music)        { this->music = music; }
    inline void setTileSize(const int& size)          { this->tileSize = size; }

    void setTile(int x, int y, const MapTile& tile);
    const MapTile& getTile(int x, int y);

    /*!
     * \brief Adds an entity to the map. The map object takes ownership of the entity.
     * \param The entity to add to the map.
     */
    void addEntity(MapEntity* entity);

    void initTiles();

    /*!
     * \brief Returns the object for this map (used in the project database)
     */
    Object getDatabaseObject();

private:
    void copy(const Map& param); /*!< Internal deep copy function. */

    int width, height, tileSize;
    QString name, world, music;
    Tileset* tileSet;                /*!< The tileset used by this map. */
    QVector<QVector<MapTile>> tiles; /*!< The tiles contained in this map. */
    QVector<MapEntity*> entities; /*!< The entities contained within this map. */
};

#endif // MAP_H
