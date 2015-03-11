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
            if(gate->getName() == gateName)
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
    obj->insert(ELE_TRAVERSABLE, (traversable == true ? "true" : "false"));

    if(hasKey())
        obj->insert(ELE_KEY_NAME, key->getName());
    if(hasGate())
        obj->insert(ELE_GATE_NAME, gate->getName());
}

Grid::Grid()
{
    width = 0;
    height = 0;

    cells = QVector<QVector<Cell>>();
}

Grid::Grid(int width, int height)
{
    this->width = width;
    this->height = height;

    this->cells = QVector<QVector<Cell>>();

    for(int x = 0; x < width; x++)
    {
        cells.append(QVector<Cell>());
        for(int y = 0; y < height; y++)
            cells[x].append(Cell(QPoint(x, y)));
    }
}

Grid::Grid(QVector<QVector<Cell>> cells)
{
    width = cells.size();
    height = cells[0].size();
    this->cells = cells;
}

bool Grid::operator==(const Grid& rhs)
{
    if(width != rhs.width || height != rhs.height)
        return false;
    else
    {
        for(int x = 0; x < width; x++)
        {
            for(int y = 0; y < height; y++)
            {
                if(!(cells[x][y] == rhs.cells[x][y]))
                   return false;
            }
        }

        return true;
    }
}

void Grid::build(Table* table)
{
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            Object object;
            cells[x][y].build(&object);
            table->addObject(OBJ_CELL, object);
        }
    }
}

Grid Grid::Parse(Table* table, int width, int height, QList<Key*> keys, QList<Gate*> gates)
{
    Grid grid = Grid(width, height);

    QList<Object*> cellObjects = table->getObjectsOfName(OBJ_CELL);
    for(Object* cell : cellObjects)
    {
        int x = cell->find(ELE_X).toInt();
        int y = cell->find(ELE_Y).toInt();
        grid.cells[x][y] = Cell::Parse(cell, keys, gates);
    }

    return grid;
}

Area::Area()
{
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

Area::Area(int stageID, Zone* zone, QPoint location, int width, int height)
    : Area()
{
    this->zone = zone;
    this->location = location;
    this->keyEvents = keyEvents;
    this->width = width;
    this->height = height;

    this->grid = Grid(width*AREA_TILE_SIZE, height*AREA_TILE_SIZE);

    this->zoneName = zone->getName();

    this->stageID = stageID;
}

Area::Area(int stageID, Zone* zone, int width, int height) :
    Area(stageID, zone, QPoint(0,0), width, height)
{

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
    area.stageID = obj->find(ELE_ID, "0").toInt();

    // Initialize and parse grid data
    QScopedPointer<Table> gridTable(new Table(filePath + QDir::separator() + QString::number(x) + QString::number(y) + DAT_EXT));
    area.grid = Grid::Parse(gridTable.data(), area.getWidth() * AREA_TILE_SIZE, area.getHeight() * AREA_TILE_SIZE, keys, gates);

    return area;
}

void Area::build(Object* obj, QString filePath)
{
    obj->data.clear(); // Clear any existing data

    obj->insert(ELE_X, QString::number(location.x()));
    obj->insert(ELE_Y, QString::number(location.y()));

    obj->insert(ELE_WIDTH, QString::number(width));
    obj->insert(ELE_HEIGHT, QString::number(height));

    obj->insert(ELE_ID, QString::number(stageID));

    QString keyList = "";
    for(Key* key : keyEvents)
        keyList += key->getName() + LIST_DELIM;
    keyList.remove(keyList.length()-1, 1);

    obj->insert(ELE_KEYS, keyList);
    obj->insert(ELE_CELL_FILE, QString::number(location.x()) + QString::number(location.y()) + DAT_EXT);

    if(zone)
        obj->insert(ELE_ZONE, zone->getName());

    // Build the grid (and save it to disk, as it is not managed by the quest data management system as of yet)
    QScopedPointer<Table> cellTable(new Table(filePath + QDir::separator() + QString::number(location.x()) + QString::number(location.y()) + ".dat"));
    grid.build(cellTable.data());
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
    this->stageID = param.stageID;

    QList<Link*>::const_iterator iter = param.links.begin();
    while(iter != param.links.end())
    {
        Link* l = new Link((*iter)->getOrigin(), (*iter)->getOriginRelative(),
                           (*iter)->getTarget(), (*iter)->getDirection());
        this->links.append(l);
        iter++;
    }
}

void Area::addLink(const QPoint& location, const QPoint& targetOrigin, const Direction& direction)
{
    links.append(new Link(this->location, location, targetOrigin, direction));
}


void Area::addLink(const Link& link)
{
    links.append(new Link(link));
}

void Area::clearAllLinks()
{
    auto iter = links.begin();
    while(iter != links.end())
    {
        delete (*iter);
        iter++;
    }

    links.clear();
}

Map Area::buildMap()
{
    Map map((location.x()*AREA_TILE_SIZE)*zone->getTileset()->getTileSize(), (location.y()*AREA_TILE_SIZE)*zone->getTileset()->getTileSize(),
            "x" + QString::number(location.x()) + "y" + QString::number(location.y()), width*AREA_TILE_SIZE, height*AREA_TILE_SIZE,
            zone->getTileset()->getTileSize(), "village", "", zone->getTileset());

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

            if(!grid.isCellTraversable(x, y))
            {
                tile.setPattern(blocked);
            }
            map.setTile(x, y, MapTile(tile));

            // Add key or gate entity to this tile
            if(grid.cellHasKey(x, y))
            {
                Key* key = grid.getCellKey(x, y);

                // If the key is a switch, add a switch to the map
                if(key->getKeyType() == Key::Type::Switch)
                {
                    SwitchEntity* switchEntity = new SwitchEntity(0, map.getTileSize()*x, map.getTileSize()*y, key->getName(),
                                                                  SwitchEntity::SubType::SOLID, SPR_SWITCH);
                    map.addSwitch(switchEntity);
                }
            }
            else if(grid.cellHasGate(x, y))
            {
                Gate* gate = grid.getCellGate(x, y);

                if(gate->getType() == Gate::Type::Door)
                {
                    Door* door = new Door(0, map.getTileSize()*x, map.getTileSize()*y, gate->getName(), Direction::EAST,
                                          Door::OpeningMethod::None, SPR_DOOR);
                    map.addDoor(door, gate->getKeys());
                }
            }
        }
    }

    return map;
}

bool Area::operator==(const Area rhs)
{
    return (location == rhs.location && zoneName == rhs.zoneName && width == rhs.width && height == rhs.height && grid == rhs.grid &&
            keyEvents == rhs.keyEvents);
}

bool Area::addKeyEvent(Key* key, int x, int y)
{
    if(x < 0 || y < 0 || x > grid.getWidth()-1 || y > grid.getHeight()-1 )
        return false;

   grid.setCellKey(key, x, y);
   keyEvents.append(key);

   return true;
}

bool Area::addGate(Gate* gate, int x, int y)
{
    if(x < 0 || y < 0 || x > grid.getWidth()-1 || y > grid.getHeight()-1 )
        return false;

    grid.setCellGate(gate, x, y);

    return true;
}

void Area::addWall(int x, int y, Direction direction)
{
    if(x > grid.getWidth()/AREA_TILE_SIZE || y > grid.getHeight()/AREA_TILE_SIZE || x < 0 || y < 0)
        return;

    int xPos, yPos;

    switch(direction)
    {
    case NORTH:
        yPos = y*AREA_TILE_SIZE;
        for(xPos = x*AREA_TILE_SIZE; xPos < (x*AREA_TILE_SIZE) + AREA_TILE_SIZE; xPos++)
            grid.getCell(xPos, yPos).setTraversable(false);
        break;

    case SOUTH:
        yPos = (y*AREA_TILE_SIZE) + (AREA_TILE_SIZE-1);
        for(xPos = x*AREA_TILE_SIZE; xPos < (x*AREA_TILE_SIZE) + AREA_TILE_SIZE; xPos++)
            grid.getCell(xPos, yPos).setTraversable(false);
        break;

    case EAST:
        xPos = (x*AREA_TILE_SIZE) + (AREA_TILE_SIZE-1);
        for(yPos = y*AREA_TILE_SIZE; yPos < (y*AREA_TILE_SIZE) + AREA_TILE_SIZE; yPos++)
            grid.getCell(xPos, yPos).setTraversable(false);
        break;

    case WEST:
        xPos = x*AREA_TILE_SIZE;
        for(yPos = y*AREA_TILE_SIZE; yPos < (y*AREA_TILE_SIZE) + AREA_TILE_SIZE; yPos++)
            grid.getCell(xPos, yPos).setTraversable(false);
        break;
    }
}

Area AreaFactory::RandomArea(int stageID, Zone* zone, int minX, int maxX, int minY, int maxY, const SpaceGenerationOptions& options)
{
    RandomEngine rand;

    int x = rand.randomInteger(minX, maxX);
    int y = rand.randomInteger(minY, maxY);

    return AreaFactory::RandomArea(stageID, zone, x, y, options);
}

Area AreaFactory::RandomArea(int stageID, Zone* zone, int x, int y, const SpaceGenerationOptions& options)
{
    RandomEngine rand;

    int width = rand.randomInteger(options.getMinimumAreaWidth(), options.getMaximumAreaWidth());
    int height = rand.randomInteger(options.getMinimumAreaHeight(), options.getMaximumAreaHeight());

    return Area(stageID, zone, QPoint(x, y), width, height);
}
