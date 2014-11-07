#include "map.h"

Map::Map()
{
    name = DEFAULT_MAP_NAME;
    world = DEFAULT_MAP_WORLD;
    width = height = DEFAULT_MAP_SIZE;
    tileSize = DEFAULT_TILE_SIZE;
    tileSet = DEFAULT_MAP_TILESET;
    music = DEFAULT_MAP_MUSIC;
}

Map::Map(int width, int height) : Map()
{
    this->width = width;
    this->height = height;
    clear();
}

Map::Map(QString name, int width, int height, int tileSize, QString tileSet, QString music, QString world) :
    name(name), width(width), height(height), tileSet(tileSet), music(music), world(world), tileSize(tileSize)
{
    clear();
}

Map::~Map()
{

}

void Map::setTile(int x, int y, const MapTile& tile)
{
    tiles[x][y] = tile;
}

const MapTile& Map::getTile(int x, int y)
{
    return tiles[x][y];
}

Map Map::parse(QString name, Table* data)
{
    Map map;

    Object* properties = data->getObject(OBJ_PROPERTIES);
    if(properties == nullptr)
        return Map(); // If properties not found, return a blank map
    else
    {
        try {
            QString tileSet = properties->find(ELE_TILESET).value();
            QString music = properties->find(ELE_MUSIC).value();
            QString world = properties->find(ELE_WORLD).value();

            int width = properties->find(ELE_WIDTH).value().toInt();
            int height = properties->find(ELE_HEIGHT).value().toInt();
            int tileSize = properties->find(ELE_TILE_SIZE).value().toInt();

            map = Map(name, width, height, tileSize, tileSet, music, world);
        }
        catch(...) {
            qDebug() << "Error! Could not read all properties of map file " << name << ".\n";
        }

        // Read in the tile grid, and put into the internal collection of tiles
        QList<Object*> mapTiles = data->getObjectsOfName(OBJ_TILE);
        for(auto t : mapTiles)
        {
            MapTile tile = MapTile::parse(t);
            map.setTile(tile.getX()/map.getTileSize(), tile.getY()/map.getTileSize(), tile);
        }
    }
}

void Map::clear()
{
    // Create a blank tile grid
    tiles = QVector<QVector<MapTile>>();
    for(int y = 0; y < height; y++)
    {
        tiles.insert(tiles.size(), QVector<MapTile>());
        for(int x = 0; x < width; x++)
            tiles[y].insert(tiles[y].size(), MapTile());
    }
}

void Map::build(Table* table)
{
    table->clear(); // Clear the existing table

    // Construct the properties object
    Object properties = Object();
    properties.insert(ELE_X, QString::number(0));
    properties.insert(ELE_Y, QString::number(0));
    properties.insert(ELE_WIDTH, QString::number(width));
    properties.insert(ELE_HEIGHT, QString::number(height));
    properties.insert(ELE_WORLD, world);
    properties.insert(ELE_TILESET, tileSet);
    properties.insert(ELE_MUSIC, music);

    // Add properties to the table
    table->addObject(OBJ_PROPERTIES, properties);

    // Construct all tile objects, and add them to the table
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
            table->addObject(OBJ_TILE, MapTile::build(tiles[x][y]));
    }
}

MapTile MapTile::parse(Object* object)
{
    MapTile mapTile;

    try {
        mapTile.setSize(object->find(ELE_WIDTH).value().toInt());
        mapTile.setX(object->find(ELE_X).value().toInt());
        mapTile.setY(object->find(ELE_Y).value().toInt());
        mapTile.setLayer(object->find(ELE_LAYER).value().toInt());
        mapTile.setPattern(object->find(ELE_PATTERN).value().toInt());
    }
    catch (...) {
        qDebug() << "Error! Failed to read tile data.";
    }

    return mapTile;
}

Object MapTile::build(MapTile tile)
{
    Object object = Object();

    object.insert(ELE_HEIGHT, QString::number(tile.getSize()));
    object.insert(ELE_WIDTH, QString::number(tile.getSize()));
    object.insert(ELE_PATTERN, QString::number(tile.getPattern()));
    object.insert(ELE_X, QString::number(tile.getX() * tile.getSize()));
    object.insert(ELE_Y, QString::number(tile.getY() * tile.getSize()));
    object.insert(ELE_LAYER, QString::number(tile.getLayer()));

    return object;
}
