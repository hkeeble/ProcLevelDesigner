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

Map::Map(QString name, int width, int height, int tileSize, QString music, QString world, Tileset* tileset) :
    Map(name, width, height, tileSize, music, world)
{
    this->tileSet = tileset;
}

Map::~Map()
{
    for(int i = 0; i < entities.length(); i++)
        delete entities[i];
}

Map& Map::operator=(const Map& rhs)
{
    if(this == &rhs)
        return *this;
    else
    {
        // Delete existing entities to prevent leaks
        for(MapEntity* entity : entities)
        {
            delete entity;
        }

        copy(rhs);
        return *this;
    }
}

Map::Map(const Map& param)
{
    copy(param);
}

void Map::copy(const Map& param)
{
    this->width = param.width;
    this->height = param.height;
    this->name = param.name;
    this->world = param.world;
    this->music = param.music;
    this->tileSet = param.tileSet;
    this->tiles = param.tiles;
    this->tileSize = param.tileSize;
    this->script = param.script;

    // Deep copy entities
    entities.clear(); // Clear existing entities
    for(MapEntity* entity : param.entities)
        this->entities.append(entity->clone());
}

void Map::setTile(int x, int y, const MapTile& tile)
{
    tiles[x][y] = tile;
}

const MapTile& Map::getTile(int x, int y)
{
    return tiles[x][y];
}

void Map::addEntity(MapEntity* entity)
{
    entities.append(entity);
}

void Map::addSwitch(SwitchEntity* switchEntity)
{
    addEntity(switchEntity);
    script.addSwitch(*switchEntity);
}

void Map::addDoor(Door* door)
{
    addEntity(door);
    script.addDoor(*door);
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

        // Read in all doors
        QList<Object*> doors = data->getObjectsOfName(OBJ_DOOR);
        for(auto d : doors)
        {
            Door* door = new Door(Door::Parse(d));
            map.addEntity(door);
        }

        // Read in all switches
        QList<Object*> switches = data->getObjectsOfName(OBJ_SWITCH);
        for(auto s : switches)
        {
            SwitchEntity* switchEntity = new SwitchEntity(SwitchEntity::Parse(s));
            map.addEntity(switchEntity);
        }

        // Read in all teletransporters
        QList<Object*> transporters = data->getObjectsOfName(OBJ_TELETRANSPORTER);
        for(auto t : transporters)
        {
            Teletransporter* transporter = new Teletransporter(Teletransporter::Parse(t));
            map.addEntity(transporter);
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

    // Construct all entities and add them to the table
    for(int i = 0; i < entities.length(); i++)
    {
        QScopedPointer<Object> obj(new Object());
        entities[i]->build(obj.data());
        table->addObject(entities[i]->getEntityName(), *obj.data());
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

Object Map::getDatabaseObject()
{
    Object obj = Object();

    obj.insert(ELE_ID, name.toLower());
    obj.insert(ELE_DESCRIPTION, name);

    return obj;
}
