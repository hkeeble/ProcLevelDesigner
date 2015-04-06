#include "space.h"

static const int SIDE_MAP_TRANSPORTER_SIZE = 16; /*!< The size that every side map transporter must be. */

static QPoint worldToArea(Area* area, const QPoint& coord)
{
    int relativeX = coord.x() - area->getLocation().x();
    int relativeY = coord.y() - area->getLocation().y();
    return QPoint(relativeX, relativeY);
}

void SpaceReceiver::missionUpdated()
{
    space->missionUpdated();
}

Space::Space()
{
    observer = new SpaceObserver();
    receiver = nullptr;
}

Space::~Space()
{

}

void Space::generateLinks(Area& area)
{
    for(int x = area.getLocation().x(); x < area.getLocation().x() + area.getWidth(); x++)
    {
        for(int y = area.getLocation().y(); y < area.getLocation().y() + area.getHeight(); y++)
        {
            if(y != 0)
            {
                if(cells[x][y-1].containsArea())
                {
                    if(areas.find(cells[x][y-1].getAreaOrigin()).value().getStageID() != area.getStageID())
                        area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), NORTH);
                    if(areas.find(cells[x][y-1].getAreaOrigin()).value() != area)
                        area.addLink(QPoint(x - area.getLocation().x(), y - area.getLocation().y()), cells[x][y-1].getAreaOrigin(), NORTH);
                }
                else
                    area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), NORTH);
            }
            else
                area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), NORTH);

            if(x != 0)
            {
                if(cells[x-1][y].containsArea())
                {
                    if(areas.find(cells[x-1][y].getAreaOrigin()).value().getStageID() != area.getStageID())
                        area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), WEST);
                    if(areas.find(cells[x-1][y].getAreaOrigin()).value() != area)
                        area.addLink(QPoint(x - area.getLocation().x(), y - area.getLocation().y()), cells[x-1][y].getAreaOrigin(), WEST);
                }
                else
                    area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), WEST);
            }
            else
                area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), WEST);

            if(y < cells[x].count()-1)
            {
                if(cells[x][y+1].containsArea())
                {
                    if(areas.find(cells[x][y+1].getAreaOrigin()).value().getStageID() != area.getStageID())
                        area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), SOUTH);
                    if(areas.find(cells[x][y+1].getAreaOrigin()).value() != area)
                        area.addLink(QPoint(x - area.getLocation().x(), y - area.getLocation().y()), cells[x][y+1].getAreaOrigin(), SOUTH);
                }
                else
                    area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), SOUTH);
            }
            else
                area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), SOUTH);

            if(x < cells.count()-1)
            {
                if(cells[x+1][y].containsArea())
                {
                    if(areas.find(cells[x+1][y].getAreaOrigin()).value().getStageID() != area.getStageID())
                        area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), EAST);
                    if(areas.find(cells[x+1][y].getAreaOrigin()).value() != area)
                        area.addLink(QPoint(x - area.getLocation().x(), y - area.getLocation().y()), cells[x+1][y].getAreaOrigin(), EAST);
                }
                else
                    area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), EAST);
            }
            else
                area.addWall(x - area.getLocation().x(), y - area.getLocation().y(), EAST);
        }
    }
}

Zone* Space::randomZone()
{
    auto iter = zones.begin();
    iter += rand.randomInteger(0, zones.count()-1);
    return &iter.value();
}

void Space::generateGates(Mission& mission, QMultiMap<Area*,Area*> stageLinks)
{
    for(QMultiMap<Area*,Area*>::iterator iter = stageLinks.begin(); iter != stageLinks.end(); iter++)
    {
        Area* gateArea = iter.key();
        Area* targetArea = iter.value();

        Gate* gate;
        QList<Stage*> stages = mission.getStages();

        // Find the gate we need, this is the exit gate of the target area's stage's previous stage...
        int targetAreaID = targetArea->getStageID();
        for(Stage* stage : stages)
        {
            if(stage->getID() == targetAreaID-1)
            {
                gate = stage->getExitGate();
            }
        }

        bool linkFound = false; // If a link is found, this is used to break the loops

        for(int x = gateArea->getLocation().x(); x < gateArea->getLocation().x() + gateArea->getWidth() && !linkFound; x++)
        {
            for(int y = gateArea->getLocation().y(); y < gateArea->getLocation().y() + gateArea->getHeight() && !linkFound; y++)
            {
                // Get X and Y relative location inside the current gate area
                QPoint areaRelative = worldToArea(gateArea, QPoint(x,y));

                // CHECK WEST
                if(x != 0)
                {
                    int targetRelativeX = (x-1) - targetArea->getLocation().x();
                    int targetRelativeY = y - targetArea->getLocation().y();
                    GridCell& cell = cells[x-1][y];
                    if(cell.containsArea() && cell.getAreaOrigin() == targetArea->getLocation()) // We need to place a gate here
                    {
                        linkFound = true;
                        QPoint cellPos = QPoint(AREA_TILE_SIZE*areaRelative.x(),
                                                ((AREA_TILE_SIZE*areaRelative.y())+AREA_TILE_SIZE) - (AREA_TILE_SIZE/2));
                        gateArea->getGrid()->setCellTraversable(true, cellPos.x(), cellPos.y());
                        gateArea->addGate(gate, cellPos.x(), cellPos.y());
                        targetArea->getGrid()->setCellTraversable(true, (AREA_TILE_SIZE*targetRelativeX) + (AREA_TILE_SIZE-1),
                                                                  ((AREA_TILE_SIZE*targetRelativeY)+AREA_TILE_SIZE) - (AREA_TILE_SIZE/2));
                        break;
                    }
                }

                // CHECK NORTH
                if(y != 0)
                {
                    int targetRelativeX = x - targetArea->getLocation().x();
                    int targetRelativeY = (y-1) - targetArea->getLocation().y();
                    GridCell& cell = cells[x][y-1];
                    if(cell.containsArea() && cell.getAreaOrigin() == targetArea->getLocation())
                    {
                        linkFound = true;
                        QPoint cellPos = QPoint(((AREA_TILE_SIZE*areaRelative.x())+AREA_TILE_SIZE) - (AREA_TILE_SIZE/2),
                                                AREA_TILE_SIZE*areaRelative.y());
                        gateArea->getGrid()->setCellTraversable(true, cellPos.x(), cellPos.y());
                        gateArea->addGate(gate, cellPos.x(), cellPos.y());
                        targetArea->getGrid()->setCellTraversable(true, ((AREA_TILE_SIZE*targetRelativeX)+AREA_TILE_SIZE) - (AREA_TILE_SIZE/2),
                                                                  (AREA_TILE_SIZE*targetRelativeY) + (AREA_TILE_SIZE-1));
                        break;
                    }
                }

                // CHECK EAST
                if(x < cells.count()-1)
                {
                    int targetRelativeX = (x+1) - targetArea->getLocation().x();
                    int targetRelativeY = y - targetArea->getLocation().y();
                    GridCell& cell = cells[x+1][y];
                    if(cell.containsArea() && cell.getAreaOrigin() == targetArea->getLocation())
                    {
                        linkFound = true;
                        QPoint cellPos = QPoint((AREA_TILE_SIZE*areaRelative.x()) + (AREA_TILE_SIZE-1),
                                                ((AREA_TILE_SIZE*areaRelative.y())+AREA_TILE_SIZE) - (AREA_TILE_SIZE/2));
                        gateArea->getGrid()->setCellTraversable(true, cellPos.x(), cellPos.y());
                        gateArea->addGate(gate, cellPos.x(), cellPos.y());
                        targetArea->getGrid()->setCellTraversable(true, (AREA_TILE_SIZE*targetRelativeX),
                                                                  ((AREA_TILE_SIZE*targetRelativeY)+AREA_TILE_SIZE) - (AREA_TILE_SIZE/2));
                        break;
                    }
                }

                // CHECK SOUTH
                if(y < cells[x].count()-1)
                {
                    int targetRelativeX = x - targetArea->getLocation().x();
                    int targetRelativeY = (y+1) - targetArea->getLocation().y();
                    GridCell& cell = cells[x][y+1];
                    if(cell.containsArea() && cell.getAreaOrigin() == targetArea->getLocation())
                    {
                        linkFound = true;
                        QPoint cellPos = QPoint(((AREA_TILE_SIZE*areaRelative.x())+AREA_TILE_SIZE) - (AREA_TILE_SIZE/2),
                                                (AREA_TILE_SIZE*areaRelative.y() + (AREA_TILE_SIZE-1)));
                        gateArea->getGrid()->setCellTraversable(true, cellPos.x(), cellPos.y());
                        gateArea->addGate(gate, cellPos.x(), cellPos.y());
                        targetArea->getGrid()->setCellTraversable(true, ((AREA_TILE_SIZE*targetRelativeX)+AREA_TILE_SIZE) - (AREA_TILE_SIZE/2),
                                                                  (AREA_TILE_SIZE*targetRelativeY));
                        break;
                    }
                }
            }
        }
    }
}

void Space::generate(Mission& mission)
{
    clear();

    QList<Stage*> stages = mission.getStages(); // Stages to generate areas for
    QMap<Stage*,QList<Area*>> generatedAreas = QMap<Stage*,QList<Area*>>(); // The areas that are generated here, mapped to the stages they were generated for
    QMultiMap<Area*,Area*> stageLinks = QMap<Area*,Area*>(); // Maps areas that need to contain gates together

    for(Stage* stage : stages)
    {
        QList<Area*> stageAreas = QList<Area*>(); // Will contain all areas generated for this stage

        // Assign a random zone for this stage
        Zone* zone = randomZone();

        // Determine first area location (this is based on a previously generated set of areas, if it exists)
        Area firstArea;
        Area* baseArea = nullptr;

        if(generatedAreas.count() > 0)
        {
            Direction dir;
            // Pick a random stage, area and direction to expand
            do {
                QMap<Stage*,QList<Area*>>::iterator iter = generatedAreas.begin();
                iter += rand.randomInteger(0, generatedAreas.count()-1);
                Stage* baseStage = iter.key();
                QList<Area*> areaSet = generatedAreas.find(baseStage).value();

                baseArea = areaSet.at(rand.randomInteger(0, areaSet.count()-1));
                dir = static_cast<Direction>(rand.randomInteger(0, Direction::COUNT));
                firstArea = Area(stage->getID(), zone, randomAreaWidth(), randomAreaHeight());
            } while(!placeInDirection(*baseArea, firstArea, dir));
        }
        else // If this is the first stage, just pick a random location
            firstArea = AreaFactory::RandomArea(stage->getID(), zone, 0, 10, 0, 10, options);

        stageAreas.append(&placeArea(firstArea)); // We must always place new areas, for space checking to work

        // Determine number of areas for this stage
        int areaCount = rand.randomInteger(options.getMinimumAreasPerStage(), options.getMaximumAreasPerStage());

        // Generate the remaining areas
        for(int i = 1; i < areaCount; i++)
        {
            Area newArea;
            Area* baseArea;
            Direction dir;

            // A bit hacky, but limit the number of iterations we attempt at each area
            int loopLimit = 100;
            int loopCount = 0;
            bool canFit = true;

            do {
                loopCount++;
                if(loopCount > loopLimit)
                {
                    canFit = false;
                    break;
                }

                baseArea = stageAreas[rand.randomInteger(0, stageAreas.count()-1)];
                dir = static_cast<Direction>(rand.randomInteger(0, Direction::COUNT));
                newArea = Area(stage->getID(), zone, randomAreaWidth(), randomAreaHeight());
            } while(!placeInDirection(*baseArea, newArea, dir));

            if(canFit)
                stageAreas.append(&placeArea(newArea)); // We must always place new areas, for space checking to work
        }

        // Randomly arrange key events in the stage
        for(Key* key : stage->getKeys())
        {
            Area* area = stageAreas[rand.randomInteger(0, stageAreas.count()-1)];
            QPoint position(0,0);
            Cell cell = area->getCell(0,0);

            do {
                position.setX(rand.randomInteger(2, area->getGrid()->getWidth()-2));
                position.setY(rand.randomInteger(2, area->getGrid()->getHeight()-2));
                cell = area->getCell(position.x(), position.y());
            } while(cell.isTraversable() == false || cell.hasGate() || cell.hasKey());

            area->addKeyEvent(key, position.x(), position.y());
        }

        generatedAreas.insert(stage, stageAreas);

        if(baseArea != nullptr)
            stageLinks.insert(baseArea, stageAreas.first());
    }

    // Generate walls, gates and then connect all areas
    for(auto iter = areas.begin(); iter != areas.end(); iter++)
    {
        Area& area = iter.value();
        generateLinks(area);
        generateGates(mission, stageLinks);
    }

    // Select a starting area in stage 1
    QList<Area*> firstStageAreas = generatedAreas.find(stages.first()).value();
    Area* firstArea = firstStageAreas[rand.randomInteger(0, firstStageAreas.length()-1)];
    QPoint position;
    do {
        position = QPoint(rand.randomInteger(2, (firstArea->getWidth()*AREA_TILE_SIZE)-2),
                          rand.randomInteger(2, (firstArea->getHeight()*AREA_TILE_SIZE)-2));
    } while(firstArea->getCell(position.x(), position.y()).hasGate() &&
            firstArea->getCell(position.x(), position.y()).hasKey() &&
            !firstArea->getCell(position.x(), position.y()).isTraversable());
    setStartingArea(firstArea->getLocation(), position);

    emitUpdate();
}


Space Space::Parse(Table* data, QList<Gate*> gates, QList<Key*> keys, QList<Tileset*> tilesets)
{
    Space space;

    // Read in all zones
    QList<Object*> zoneObjects = data->getObjectsOfName(OBJ_ZONE);
    for(Object* obj : zoneObjects)
    {
        Zone zone = Zone::Parse(obj, tilesets);
        space.zones.insert(zone.getName(), zone);
    }

    // Read in all areas (areas subvert the quest data system, they must load their cells from a table that they manage,
    //                      therefore they are given the filepath of the space data to use as a basis for loading this table)
    QList<Object*> areaObjects = data->getObjectsOfName(OBJ_AREA);
    for(Object* obj : areaObjects)
        space.placeArea(Area::Parse(obj, QFileInfo(data->getFilePath()).absoluteDir().absolutePath(), keys, gates));

    // Read in all links
    QList<Link> linkList;
    QList<Object*> linkObjects = data->getObjectsOfName(OBJ_LINK);
    for(Object* obj : linkObjects)
    {
        Link link = Link::Parse(obj);
        linkList.append(link);
    }

    // Point areas to their zones
    QMap<QPoint,Area>* areas = space.getAreas();
    for(auto iter = areas->begin(); iter != areas->end(); iter++)
    {
        Zone* zone = &space.zones.find(iter.value().getZoneName()).value();
        if(zone)
            iter.value().setZone(zone);
    }

    // Place all links into their correct areas
    for(Link& link : linkList)
    {
        Area* origin = &areas->find(link.getOrigin()).value();
        origin->addLink(link);
    }

    // Read in options
    QScopedPointer<Object> obj(new Object());
    space.options = SpaceGenerationOptions::Parse(obj.data());

    return space;
}

void Space::build(Table *data)
{
    data->clear(); // Clear any existing data

    // Build all zones
    for(QMap<QString,Zone>::iterator iter = zones.begin(); iter != zones.end(); iter++)
    {
        Object obj;
        iter.value().build(&obj);
        data->addObject(OBJ_ZONE, obj);
    }

    // Build all areas and compile a list of the links that need building. As with parsing,
    // areas currently subvert the quest data system as they manage their own tables for cell
    // data. Therefore the location of the space data is given to the build function in order
    // to manage this.
    QList<Link*> linkList;
    for(auto iter = areas.begin(); iter != areas.end(); iter++)
    {
        Object obj;
        iter.value().build(&obj, QFileInfo(data->getFilePath()).absoluteDir().absolutePath());
        data->addObject(OBJ_AREA, obj);

        linkList.append(iter.value().getLinks());
    }

    // Build all links
    for(Link* link : linkList)
    {
        Object obj;
        link->build(&obj);
        data->addObject(OBJ_LINK, obj);
    }

    // Build options
    QScopedPointer<Object> obj(new Object());
    options.build(obj.data());
    data->addObject(OBJ_SPACE_OPTIONS, *obj.data());
}

bool Space::addZone(QString name, Zone zone)
{
    if(!zones.contains(name))
    {
        zones.insert(name, zone);
        return true;
    }
    else
        return false;
}

bool Space::removeZone(QString name)
{
    if(zones.contains(name))
    {
        zones.remove(name);
        return true;
    }
    else
        return false;
}

Zone* Space::getZone(QString name)
{
    auto iter = zones.find(name);
    if(iter == zones.end())
        return nullptr;
    else
        return &iter.value();
}

QList<Zone*> Space::getZoneList()
{
    QList<Zone*> zoneList;
    for(QMap<QString,Zone>::iterator iter = zones.begin(); iter != zones.end(); iter++)
        zoneList.append(&iter.value());
    return zoneList;
}

Area& Space::placeArea(Area area)
{
    // Ensure there is enough room in the grid for the new area.
    // - Expand X direction
    if(cells.length() < area.getLocation().x() + area.getWidth())
    {
        while(cells.length() < area.getLocation().x() + area.getWidth())
        {
            cells.append(QVector<GridCell>());
            while(cells[0].length() > cells.last().length())
                cells.last().append(GridCell());
        }
    }
    // - Expand Y direction
    if(cells[0].length() < area.getLocation().y() + area.getHeight())
    {
        for(int x = 0; x < cells.length(); x++)
        {
            while(cells[x].length() < area.getLocation().y() + area.getHeight())
                cells[x].append(GridCell());
        }
    }

    // Assign grid cells new ID
    for(int i = area.getLocation().x(); i < area.getLocation().x() + area.getWidth(); i++)
    {
        for(int j = area.getLocation().y(); j < area.getLocation().y() + area.getHeight(); j++)
            cells[i][j] = GridCell(area.getLocation());
    }

    // Insert area into area map
    return areas.insert(area.getLocation(), area).value();
}

bool Space::areaFits(Area area)
{
    return areaFits(area.getLocation(), area.getWidth(), area.getHeight());
}

bool Space::areaFits(const QPoint& location, int width, int height)
{
    // If area is out of bounds, return false
    if(location.x() < 0 || location.y() < 0)
        return false;

    // The location may take the area off-grid (out of bounds), but this does not mean it does not fit, therefore only go as far as we can.
    int xLimit = location.x() + width;
    int yLimit = location.y() + height;
    if(xLimit > cells.count()-1) {
        xLimit = cells.count()-1;
    }
    if(yLimit > cells[0].count()-1) {
        yLimit = cells[0].count()-1;
    }

    for(int x = location.x(); x < xLimit; x++)
    {
        for(int y = location.y(); y < yLimit; y++)
        {
            // If cell contains an area, area will not fit into the map
            if(cells[x][y].containsArea())
                return false;
        }
    }

    return true;
}

bool Space::placeInDirection(const Area& baseArea, Area& newArea, Direction direction)
{
    QPoint currentLoc;

    int x = 0;
    int y = 0;

    bool validFound = false;

    switch(direction)
    {

    case SOUTH: // Placing SOUTH
        y = baseArea.getLocation().y() + baseArea.getHeight();
        for(x = baseArea.getLocation().x(); x < baseArea.getLocation().x() + baseArea.getWidth(); x++)
        {
            currentLoc.setX(x);
            currentLoc.setY(y);
            if(areaFits(currentLoc, newArea.getWidth(), newArea.getHeight()))
            {
                validFound = true;
                break;
            }
        }
        break;

    case EAST: // Placing EAST
        x = baseArea.getLocation().x() + baseArea.getWidth();
        for(y = baseArea.getLocation().y(); y < baseArea.getLocation().y() + baseArea.getHeight(); y++)
        {
            currentLoc.setX(x);
            currentLoc.setY(y);
            if(areaFits(currentLoc, newArea.getWidth(), newArea.getHeight()))
            {
                validFound = true;
                break;
            }
        }
        break;

    case NORTH: // Placing NORTH
        y = baseArea.getLocation().y() - newArea.getHeight();
        for(x = baseArea.getLocation().x(); x < baseArea.getLocation().x() + baseArea.getWidth(); x++)
        {
            currentLoc.setX(x);
            currentLoc.setY(y);
            if(areaFits(currentLoc, newArea.getWidth(), newArea.getHeight()))
            {
                validFound = true;
                break;
            }
        }

        break;
    case WEST: // Placing WEST
        x = baseArea.getLocation().x() - newArea.getWidth();
        for(y = baseArea.getLocation().y(); y < baseArea.getLocation().y() + baseArea.getHeight(); y++)
        {
            currentLoc.setX(x);
            currentLoc.setY(y);
            if(areaFits(currentLoc, newArea.getWidth(), newArea.getHeight()))
            {
                validFound = true;
                break;
            }
        }

        break;

    default:
        break;
    }

    if(validFound)
        newArea.setLocation(currentLoc);

    return validFound;
}

bool Space::areNeighbours(const Area& baseArea, const Area& area, Direction& out)
{
    int yTop = baseArea.getLocation().y() - 1;
    int yBottom = baseArea.getLocation().y() + baseArea.getHeight() + 1;
    int xLeft = baseArea.getLocation().x() - 1;
    int xRight = baseArea.getLocation().x() + baseArea.getWidth() + 1;

    // Check possibilities
    bool northPossible = true, southPossible = true, eastPossible = true, westPossible = true;
    if(yTop < 0)
        northPossible = false;
    if(yBottom > cells[0].count()-1)
        southPossible = false;
    if(xLeft < 0)
        westPossible = false;
    if(xRight > cells.count()-1)
        eastPossible = false;

    // Check north and south
    if(northPossible || southPossible)
    {
        for(int x = baseArea.getLocation().x(); x < baseArea.getLocation().x() + baseArea.getWidth(); x++)
        {
            if(northPossible && cells[x][yTop].getAreaOrigin() == area.getLocation())
            {
                out = Direction::NORTH;
                return true;
            }
            else if(southPossible && cells[x][yBottom].getAreaOrigin() == area.getLocation())
            {
                out = Direction::SOUTH;
                return true;
            }
        }
    }

    // Check east and west
    if(westPossible || eastPossible)
    {
        for(int y = baseArea.getLocation().y(); y < baseArea.getLocation().y() + baseArea.getHeight(); y++)
        {
            if(eastPossible && cells[xRight][y].getAreaOrigin() == area.getLocation())
            {
                out = Direction::EAST;
                return true;
            }
            else if(westPossible && cells[xLeft][y].getAreaOrigin() == area.getLocation())
            {
                out = Direction::WEST;
                return true;
            }
        }
    }

    return false;
}

QList<QPoint> Space::getNeighbours(const Area& area)
{
    QList<QPoint> neighbourLocations = QList<QPoint>();

    int yTop = area.getLocation().y() - 1;
    int yBottom = area.getLocation().y() + area.getHeight() + 1;
    int xLeft = area.getLocation().x() - 1;
    int xRight = area.getLocation().x() + area.getWidth() + 1;

    // Check possibilities
    bool northPossible = true, southPossible = true, eastPossible = true, westPossible = true;
    if(yTop < 0)
        northPossible = false;
    if(yBottom > cells[0].count()-1)
        southPossible = false;
    if(xLeft < 0)
        westPossible = false;
    if(xRight > cells.count()-1)
        eastPossible = false;

    // Check north and south
    if(northPossible || southPossible)
    {
        for(int x = area.getLocation().x(); x < area.getLocation().x() + area.getWidth(); x++)
        {
            if(northPossible && cells[x][yTop].containsArea())
            {
                if(!neighbourLocations.contains(cells[x][yTop].getAreaOrigin()))
                    neighbourLocations.append(cells[x][yTop].getAreaOrigin());
            }
            else if(southPossible && cells[x][yBottom].containsArea())
            {
                if(!neighbourLocations.contains(cells[x][yBottom].getAreaOrigin()))
                    neighbourLocations.append(cells[x][yBottom].getAreaOrigin());
            }
        }
    }

    // Check east and west
    if(westPossible || eastPossible)
    {
        for(int y = area.getLocation().y(); y < area.getLocation().y() + area.getHeight(); y++)
        {
            if(eastPossible && cells[xRight][y].containsArea())
            {
                if(!neighbourLocations.contains(cells[xRight][y].getAreaOrigin()))
                    neighbourLocations.append(cells[xRight][y].getAreaOrigin());
            }
            else if(westPossible && cells[xLeft][y].containsArea())
            {
                if(!neighbourLocations.contains(cells[xLeft][y].getAreaOrigin()))
                    neighbourLocations.append(cells[xLeft][y].getAreaOrigin());
            }
        }
    }

    return neighbourLocations;
}

Direction Space::getDirection(const Area& baseArea, const Area& otherArea)
{
    QPoint base = baseArea.getLocation();
    QPoint other = otherArea.getLocation();

    QPoint dir = other - base;

    if(dir.x() != 0)
    {
        if(dir.x() < 0)
            return Direction::WEST;
        else
            return Direction::EAST;
    }
    else
    {
        if(dir.y() < 0)
            return Direction::NORTH;
        else
            return Direction::SOUTH;
    }
}

bool Space::removeArea(int x, int y)
{
    if(cells[x][y].containsArea())
    {
        Area* area = &areas.find(QPoint(x,y)).value();

        for(int i = x; i < area->getWidth(); i++)
        {
            for(int j = y; j < area->getHeight(); j++)
            {
                cells[i][j].removeArea();
            }
        }

        areas.remove(QPoint(x,y));

        return true;
    }

    return false;
}

bool Space::removeArea(Area area)
{
    for(auto iter = areas.begin(); iter != areas.end(); iter++)
    {
        if(iter.value() == area)
        {
            areas.remove(iter.key());
            return true;
        }
    }

    return false;
}

GridCell* Space::getCell(int x, int y)
{
    if(x < cells.length() && y < cells[x].length())
        return &cells[x][y];

    return nullptr;
}

void Space::clear()
{
    areas.clear();
    for(int x = 0; x < cells.size(); x++)
    {
        for(int y = 0; y < cells[0].size(); y++)
        {
            cells[x][y] = GridCell();
        }
    }
}

bool Space::setStartingArea(QPoint areaOrigin, QPoint position)
{
    // Ensure this is a valid starting area before assigning it
    if(!areas.contains(areaOrigin))
        return false;

    Area* area = &areas.find(areaOrigin).value();

    if(position.x() < 0 && position.y() < 0 &&
            position.x() > area->getWidth() * AREA_TILE_SIZE && position.y() > area->getHeight() * AREA_TILE_SIZE)
        return false;

    startingArea = areaOrigin;
    startingLocation = position;

    return true;
}

QList<Map> Space::buildMaps()
{
    // Build all areas into maps.
    QMap<QPoint, Map> maps = QMap<QPoint, Map>();
    for(auto iter = areas.begin(); iter != areas.end(); iter++)
    {
        maps.insert(iter.key(), iter.value().buildMap());

        Map* map = &maps.last();

        if(iter.key() == startingArea)
            map->addEntity(new Destination(0, startingLocation.x() * map->getTileSize(), startingLocation.y() * map->getTileSize(), "start", Direction::EAST));
    }

    for(QMap<QPoint,Area>::iterator iter = areas.begin(); iter != areas.end(); iter++)
    {
        Map* map = &maps.find(iter.value().getLocation()).value();
        QVector<MapEntity*> transporters = QVector<MapEntity*>(); // collection of transporters that need adding to the map

        // Iterate over all links and create the appropriate entities
        QList<Link*> links = iter.value().getLinks();
        for(Link* link : links)
        {
            Map* destination = &maps.find(link->getTarget()).value();
            Direction dir = link->getDirection();
            QPoint relativeLocation = link->getOriginRelative();

            // Determine position and dimensions of the transporter entity
            int xPos, yPos, width, height;
            switch(dir)
            {
            case WEST:
                yPos = (AREA_TILE_SIZE*map->getTileSize()) * relativeLocation.y();
                xPos = ((AREA_TILE_SIZE*map->getTileSize()) * relativeLocation.x()) - SIDE_MAP_TRANSPORTER_SIZE;
                width = SIDE_MAP_TRANSPORTER_SIZE;
                height = AREA_TILE_SIZE*map->getTileSize();
                break;

            case EAST:
                yPos = (AREA_TILE_SIZE*map->getTileSize()) * relativeLocation.y();
                xPos = ((AREA_TILE_SIZE*map->getTileSize()) * relativeLocation.x()) + (AREA_TILE_SIZE*map->getTileSize());
                width = SIDE_MAP_TRANSPORTER_SIZE;
                height = AREA_TILE_SIZE*map->getTileSize();
                break;

            case NORTH:
                yPos = ((AREA_TILE_SIZE*map->getTileSize()) * relativeLocation.y()) - SIDE_MAP_TRANSPORTER_SIZE;
                xPos = (AREA_TILE_SIZE*map->getTileSize()) * relativeLocation.x();
                width = AREA_TILE_SIZE*map->getTileSize();
                height = SIDE_MAP_TRANSPORTER_SIZE;
                break;

            case SOUTH:
                yPos = ((AREA_TILE_SIZE*map->getTileSize()) * relativeLocation.y()) + (AREA_TILE_SIZE*map->getTileSize());
                xPos = (AREA_TILE_SIZE*map->getTileSize()) * relativeLocation.x();
                width = AREA_TILE_SIZE*map->getTileSize();
                height = SIDE_MAP_TRANSPORTER_SIZE;
                break;

            default:
                break;

            }

            transporters.append(new Teletransporter(xPos, yPos, width, height, destination->getName(), "_side",
                                                    Teletransporter::Transition::Scroll));
        }


        for(MapEntity* entity : transporters)
            map->addEntity(entity);
    }

    QList<Map> mapList = QList<Map>();
    for(auto iter = maps.begin(); iter != maps.end(); iter++)
    {
        mapList.append(iter.value());
    }
    return mapList;
}

void Space::missionUpdated()
{
    clear();
    emitUpdate();
}

void Space::setMission(Mission* mission)
{
    receiver = new SpaceReceiver(mission, this);
}

int Space::randomAreaWidth()
{
    return rand.randomInteger(options.getMinimumAreaWidth(), options.getMaximumAreaWidth());
}

int Space::randomAreaHeight()
{
    return rand.randomInteger(options.getMinimumAreaHeight(), options.getMaximumAreaHeight());
}
