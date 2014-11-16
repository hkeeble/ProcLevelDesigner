#include "map.h"

Map::Map()
{
    name = DEFAULT_MAP_NAME;
    world = DEFAULT_MAP_WORLD;
    width = height = DEFAULT_MAP_SIZE;
    tileSize = DEFAULT_TILE_SIZE;
    music = DEFAULT_MAP_MUSIC;
}

Map::Map(int tileSize, int width, int height) : Map()
{
    this->width = width;
    this->height = height;
    this->tileSize = tileSize;
    initTiles();
}

Map::Map(QString name, int width, int height, int tileSize, QString music, QString world) :
    name(name), width(width), height(height), music(music), world(world), tileSize(tileSize)
{
    initTiles();
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
        // Read in map properties (sets defaults if not found)
        map = Map(properties->find(ELE_TILE_SIZE, QString::number(DEFAULT_TILE_SIZE)).toInt(),
                  properties->find(ELE_WIDTH, QString::number(DEFAULT_MAP_SIZE)).toInt(),
                  properties->find(ELE_HEIGHT, QString::number(DEFAULT_MAP_SIZE)).toInt());
        map.setName(name);
        map.setMusic(properties->find(ELE_MUSIC, DEFAULT_MAP_MUSIC));


        // Read in the tile grid, and put into the internal collection of tiles
        QList<Object*> mapTiles = data->getObjectsOfName(OBJ_TILE);
        for(auto t : mapTiles)
        {
            MapTile tile = MapTile::parse(t);
            map.setTile(tile.getX()/map.getTileSize(), tile.getY()/map.getTileSize(), tile);
        }

        return map;
    }
}

void Map::initTiles()
{
    tiles = QVector<QVector<MapTile>>(width, QVector<MapTile>(height));
}

void Map::build(Table* table)
{
    table->clear(); // Clear the existing table

    // Construct the properties object
    Object properties = Object();
    properties.insert(ELE_X, QString::number(0));
    properties.insert(ELE_Y, QString::number(0));
    properties.insert(ELE_WIDTH, QString::number(width * tileSize));
    properties.insert(ELE_HEIGHT, QString::number(height * tileSize));
    properties.insert(ELE_WORLD, world);
    properties.insert(ELE_MUSIC, music);
    properties.insert(ELE_TILESET, tileSet->getName());

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

    mapTile.setSize     (object->find(ELE_WIDTH, QString::number(DEFAULT_TILE_SIZE)).toInt());
    mapTile.setX        (object->find(ELE_X, "0").toInt());
    mapTile.setY        (object->find(ELE_Y, "0").toInt());
    mapTile.setLayer    (object->find(ELE_LAYER, "0").toInt());
    mapTile.setPattern  (object->find(ELE_PATTERN, "0").toInt());

    return mapTile;
}

Object MapTile::build(MapTile tile)
{
    Object object = Object();

    object.insert(ELE_HEIGHT,   QString::number(tile.getSize()));
    object.insert(ELE_WIDTH,    QString::number(tile.getSize()));
    object.insert(ELE_PATTERN,  QString::number(tile.getPattern()));
    object.insert(ELE_X,        QString::number(tile.getX() * tile.getSize()));
    object.insert(ELE_Y,        QString::number(tile.getY() * tile.getSize()));
    object.insert(ELE_LAYER,    QString::number(tile.getLayer()));

    return object;
}

Object Map::getObject()
{
    Object obj = Object();

    obj.insert(ELE_ID, name.toLower());
    obj.insert(ELE_DESCRIPTION, name);

    return obj;
}
