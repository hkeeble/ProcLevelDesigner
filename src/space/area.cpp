#include "area.h"

Cell Cell::Parse(Object* obj, QList<Key*> keys, QList<Gate*> gates)
{
    Cell cell;

    QString gateName = obj->find(ELE_GATE_NAME, NULL_ELEMENT);
    QString keyName = obj->find(ELE_KEY_NAME, NULL_ELEMENT);

    if(gateName != NULL_ELEMENT)
    {
        for(Gate* gate : gates)
        {
            if(gate->getName() == keyName)
                cell.gate = gate;
        }
    }
    if(keyName != NULL_ELEMENT)
    {
        for(Key* key : keys)
        {
            if(key->getName() == keyName)
                cell.key = key;
        }
    }

    cell.traversable = (obj->find(ELE_TRAVERSABLE, "true") == "true" ? true : false);

    int x = obj->find(ELE_X, "0").toInt();
    int y = obj->find(ELE_Y, "0").toInt();
    cell.location = QPoint(x, y);

    return cell;
}

void Cell::build(Object* obj)
{
    obj->insert(ELE_X, QString::number(location.x()));
    obj->insert(ELE_Y, QString::number(location.y()));
    obj->insert(ELE_TRAVERSABLE, (traversable == true ? "true" : false));

    if(hasKey())
        obj->insert(ELE_KEY_NAME, key->getName());
    if(hasGate())
        obj->insert(ELE_GATE_NAME, gate->getName());
}

Area::Area()
{
    up = left = right = down = nullptr;
    zone = nullptr;
}

Area::~Area()
{
    clearAllLinks();
}

Area::Area(const Area& param)
{
    cpy(param);
}

Area& Area::operator=(const Area& param)
{
    if(this == &param)
        return *this;
    else
    {
        cpy(param);
        return *this;
    }
}

Area::Area(Zone* zone, QPoint location, int width, int height, QList<Key*> keyEvents)
    : Area()
{
    this->zone = zone;
    this->location = location;
    this->keyEvents = keyEvents;
    this->width = width;
    this->height = height;

    initGrid(width, height);

    this->zoneName = zone->getName();
}

Area Area::Parse(Object* obj, QString filePath, QList<Key*> keys, QList<Gate*> gates)
{
    Area area;
    int x = obj->find(ELE_X, "0").toInt();
    int y = obj->find(ELE_Y, "0").toInt();
    area.location = QPoint(x, y);

    QStringList keyList = obj->find(ELE_KEYS, NULL_ELEMENT).split(LIST_DELIM);

    // Search through list of keys, apply them
    for(Key* key : keys)
    {
        if(keyList.contains(key->getName()))
            area.keyEvents.append(key);
    }

    area.zoneName = obj->find(ELE_ZONE, NULL_ELEMENT);
    area.width = obj->find(ELE_WIDTH, "1").toInt();
    area.height = obj->find(ELE_HEIGHT, "1").toInt();

    // Initialize and parse individual cell data
    area.parseGrid(filePath + QDir::separator() + QString::number(x) + QString::number(y) + DAT_EXT, keys, gates);

    return area;
}

void Area::build(Object* obj, QString filePath)
{
    obj->data.clear(); // Clear any existing data

    obj->insert(ELE_X, QString::number(location.x()));
    obj->insert(ELE_Y, QString::number(location.y()));

    obj->insert(ELE_WIDTH, QString::number(width));
    obj->insert(ELE_HEIGHT, QString::number(height));

    QString keyList = "";
    for(Key* key : keyEvents)
        keyList += key->getName() + LIST_DELIM;
    keyList.remove(keyList.length()-1, 1);

    obj->insert(ELE_KEYS, keyList);
    obj->insert(ELE_CELL_FILE, QString::number(location.x()) + QString::number(location.y()) + DAT_EXT);

    if(zone)
        obj->insert(ELE_ZONE, zone->getName());

    buildGrid(filePath + QDir::separator() + QString::number(location.x()) + QString::number(location.y()) + ".dat");
}

void Area::parseGrid(QString filePath, QList<Key*> keys, QList<Gate*> gates)
{
    initGrid(width, height);
    QScopedPointer<Table> cellTable(new Table(filePath));
    QList<Object*> cellObjects = cellTable->getObjectsOfName(OBJ_CELL);
    for(Object* cell : cellObjects)
    {
        int x = cell->find(ELE_X).toInt();
        int y = cell->find(ELE_Y).toInt();
        grid[x][y] = Cell::Parse(cell, keys, gates);
    }
}

void Area::buildGrid(QString filePath)
{
    QScopedPointer<Table> cellTable(new Table(filePath));
    for(int x = 0; x < grid.length(); x++)
    {
        for(int y = 0; y < grid[0].length(); y++)
        {
            Object object;
            grid[x][y].build(&object);
            cellTable.data()->addObject(OBJ_CELL, object);
        }
    }
    cellTable.data()->saveToDisk();
}

void Area::cpy(const Area& param)
{
    this->zoneName = param.zoneName;
    this->location = param.location;
    this->keyEvents = param.keyEvents;
    this->zone = param.zone;
    this->width = param.width;
    this->height = param.height;
    this->grid = param.grid;

    this->right = this->left = this->down = this->up = nullptr;

    if(param.right)
        this->right = new Link(*param.right);
    if(param.left)
        this->left = new Link(*param.left);
    if(param.down)
        this->down = new Link(*param.down);
    if(param.up)
        this->up = new Link(*param.up);
}

void Area::clearAllLinks()
{
    removeRightLink();
    removeLeftLink();
    removeDownLink();
    removeUpLink();
}

void Area::removeRightLink()
{
    if(right)
        delete right;

    right = nullptr;
}

void Area::removeLeftLink()
{
    if(left)
        delete left;

    left = nullptr;
}

void Area::removeDownLink()
{
    if(down)
        delete down;

    down = nullptr;
}

void Area::removeUpLink()
{
    if(up)
        delete up;

    up = nullptr;
}

void Area::initGrid(int width, int height)
{
    grid = QVector<QVector<Cell>>(width*AREA_TILE_SIZE);

    for(int x = 0; x < width*AREA_TILE_SIZE; x++)
        grid[x] = QVector<Cell>(height*AREA_TILE_SIZE);

    for(int x = 0; x < width*AREA_TILE_SIZE; x++)
    {
        for(int y = 0; y < height*AREA_TILE_SIZE; y++)
            grid[x][y] = Cell(QPoint(x,y), true);
    }
}

Map Area::buildMap()
{
    Map map(QString::number(location.x()) + QString::number(location.y()),
            width*AREA_TILE_SIZE, height*AREA_TILE_SIZE, zone->getTileset()->getTileSize(), "village", "", zone->getTileset());

    // For now, the blocked tiles are the first impassable pattern found -- TESTING ONLY CODE
    QList<TilePattern*> patterns = map.getTileSet()->getPatternList();
    int blocked = 0;
    for(int i = 0; i < patterns.length(); i++)
    {
        if(!patterns[i]->traversable)
            blocked = i;
    }

    for(int x = 0; x < map.getWidth(); x++)
    {
        for(int y = 0; y < map.getHeight(); y++)
        {
            MapTile tile = MapTile(0, x, y, map.getTileSize(), 0);

            if(!grid[x][y].isTraversable())
            {
                tile.setPattern(blocked);
            }
            map.setTile(x, y, MapTile(tile));
        }
    }

    // Add key entities
    for(Key* key : keyEvents)
    {
        // If the key is a switch, add a switch to the map
        if(key->getKeyType() == Key::Type::Switch)
        {
            SwitchEntity* switchEntity = new SwitchEntity(0, (map.getWidth() * map.getTileSet()->getTileSize())/2 - 8,
                                                          (map.getHeight()*map.getTileSet()->getTileSize())/2 - 8, key->getName(),
                                                          SwitchEntity::SubType::SOLID, SPR_SWITCH);
            map.addSwitch(switchEntity);
        }
    }

    // Add gate entities
    if(up != nullptr && up->getGate() != nullptr)
    {
        Gate* gate = up->getGate();
        if(gate->getType() == Gate::Type::Door)
        {
            Door* door = new Door(0, (map.getWidth()*map.getTileSize())/2 - 8, 0, gate->getName(), Direction::EAST, Door::OpeningMethod::None, SPR_DOOR);
            map.addDoor(door, gate->getKeys());
        }
    }
    if(left != nullptr && left->getGate() != nullptr)
    {
        Gate* gate = left->getGate();
        if(gate->getType() == Gate::Type::Door)
        {
            Door* door = new Door(0, 0, (map.getHeight()*map.getTileSize())/2 - 8, gate->getName(), Direction::EAST, Door::OpeningMethod::None, SPR_DOOR);
            map.addDoor(door, gate->getKeys());
        }
    }
    if(right != nullptr && right->getGate() != nullptr)
    {
        Gate* gate = right->getGate();
        if(gate->getType() == Gate::Type::Door)
        {
            Door* door = new Door(0, map.getWidth()*map.getTileSize(), (map.getWidth()*map.getTileSize())/2 - 8, gate->getName(), Direction::EAST, Door::OpeningMethod::None, SPR_DOOR);
            map.addDoor(door, gate->getKeys());
        }
    }
    if(down != nullptr && down->getGate() != nullptr)
    {
        Gate* gate = down->getGate();
        if(gate->getType() == Gate::Type::Door)
        {
            Door* door = new Door(0, (map.getWidth()*map.getTileSize())/2 - 8, map.getHeight()*map.getTileSize(), gate->getName(), Direction::EAST, Door::OpeningMethod::None, SPR_DOOR);
            map.addDoor(door, gate->getKeys());
        }
    }

    return map;
}


bool Area::operator==(const Area rhs)
{
    return (location == rhs.location && zoneName == rhs.zoneName && width == rhs.width && height == rhs.height && grid == rhs.grid &&
            keyEvents == rhs.keyEvents && up == rhs.up && down == rhs.down && left == rhs.left && right == rhs.right && zone == rhs.zone);
}

void Area::addKeyEvent(Key* key)
{
    keyEvents.append(key);
}
