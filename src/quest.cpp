#include "quest.h"

Quest::Quest()
{
    fsModel = nullptr;
    scriptModel = nullptr;
    mapModel = nullptr;
}

Quest::Quest(QString dirPath)
{
    // Set up file system models
    rootDir = QDir(dirPath);
    fsModel = new QFileSystemModel();
    scriptModel = new QFileSystemModel();
    mapModel = new QFileSystemModel();

    tileSets = QMap<QString,Tileset>();

    fsModel->setRootPath(rootDir.absolutePath());
    scriptModel->setRootPath(rootDir.absolutePath());
    mapModel->setRootPath(rootDir.absolutePath() + QDir::separator() + "maps" + QDir::separator());

    initFilters();
}

bool Quest::Init()
{
    Table* quest = getData(DAT_QUEST);

    if(quest->isEmpty())
        return false;
    else
    {
        // Load all existing tilesets
        QDir tileSetDir = QDir(rootDir.absolutePath() + QDir::separator() + "tilesets" + QDir::separator());
        QStringList filters;
        filters << "*.dat";
        tileSetDir.setNameFilters(filters);
        QFileInfoList tilesetData = tileSetDir.entryInfoList();

        for(QFileInfo f : tilesetData)
        {
            Table* data = getData(QString("tilesets") + QDir::separator() + f.baseName());
            tileSets.insert(f.baseName(), Tileset::parse(f.baseName(), data));
        }

        // Initialize the mission
        Table* missionData = getData(DAT_MISSION);
        mission = Mission::Parse(missionData);

        // Initialize the space
        Table* spaceData = getData(DAT_SPACE);
        space = Space::Parse(spaceData, mission.getGateList(), mission.getKeyEventList(), getTilesetList());

        // Space should observe mission for changes
        space.setMission(&mission);

        // Parse current hero data
        Table* heroData = getData(DAT_HERO);
        hero = Hero::Parse(heroData);
        hero.build(heroData); // Build the hero data (in case it has just been created)

        return true;
    }
}

void Quest::buildMaps()
{
    maps = space.buildMaps();
}

void Quest::saveMaps()
{
    Table* database = getData(DAT_DATABASE);

    for(Map& map : maps)
    {
        Table* data = getData(QString("maps") + QDir::separator() + map.getName());
        map.build(data);

        // Write out the script file
        QFile file(QFileInfo(data->getFilePath()).absoluteDir().absolutePath() + QDir::separator() + map.getName() + ".lua");
        map.saveScript(file);

        // Add map to quest database
        database->addObject(OBJ_MAP, map.getDatabaseObject());

        data->saveToDisk();
    }

    database->saveToDisk();
}

void Quest::build()
{
    mission.build(getData(DAT_MISSION));
    space.build(getData(DAT_SPACE));
}

Quest::~Quest()
{
    clear();
}

QFileSystemModel* Quest::getFSModel()
{
    return fsModel;
}

QFileSystemModel* Quest::getScriptModel()
{
    return scriptModel;
}

QFileSystemModel* Quest::getMapModel()
{
    return mapModel;
}

QDir Quest::getRootDir() const
{
    return rootDir;
}

QDir Quest::getExecutableDir() const
{
    QDir dir = rootDir;
    dir.cdUp();
    return dir;
}

QString Quest::getName()
{
    Table* quest = getData(DAT_QUEST);
    if(quest != nullptr)
    {
        Object* obj = quest->getObject(OBJ_QUEST);
        return obj->find(ELE_TITLE_BAR, "NULL_NAME");
    }

    return NULL_ELEMENT;
}

void Quest::saveData()
{
    // Do some clean-up on the space data files before saving
    deleteFolderContents(rootDir.absolutePath() + QDir::separator() + QString("proc_designer_data") +
                         QDir::separator() + QString("space_data") + QDir::separator());

    build(); // Build mission and space structures

    // Loop through all loaded data tables
    for(auto iter : data.toStdMap())
        iter.second.data()->saveToDisk();

    data.clear(); // clear data in memory
}

Quest& Quest::operator=(const Quest& param)
{
    if(this == &param)
        return *this;
    else
    {
        cpy(param);
        return *this;
    }
}

Quest::Quest(const Quest& param)
{
    cpy(param);
}

void Quest::cpy(const Quest& param)
{
    if(param.fsModel)
    {
        rootDir = param.rootDir;

        fsModel = new QFileSystemModel();
        fsModel->setRootPath(rootDir.absolutePath());

        scriptModel = new QFileSystemModel();
        scriptModel->setRootPath(rootDir.absolutePath());

        mapModel = new QFileSystemModel();
        mapModel->setRootPath(rootDir.absolutePath() + QDir::separator() + "maps" + QDir::separator());

        initFilters();
    }
    else
        fsModel = nullptr;
}

void Quest::initFilters()
{
    scriptModel->setNameFilters(QStringList()<<"*.lua");
    mapModel->setNameFilters(QStringList()<<"*.dat");

    scriptModel->setNameFilterDisables(false);
    mapModel->setNameFilterDisables(false);
}

Table* Quest::getData(QString filePath)
{
    QString absolutePath = getRootDir().absolutePath() + QDir::separator() + filePath + DAT_EXT;

    auto iter = data.find(filePath);

    if(iter != data.end()) // If data already loaded, return the table stored in memory
        return iter.value().data();
    else
    {
        Table* table = new Table(absolutePath); // Parse existing data, or create new table
        data.insert(filePath, QSharedPointer<Table>(table)); // Insert into map of loaded data
        return table;
    }
}

void Quest::clear()
{
    data.clear();
    rootDir = "";

    if(fsModel)
        delete fsModel;
    if(scriptModel)
        delete scriptModel;
    if(mapModel)
        delete mapModel;

    tileSets.clear();
    maps.clear();

    fsModel = scriptModel = mapModel = nullptr;

}

Tileset* Quest::getTileset(QString name)
{
    return &(*tileSets.find(name));
}

QMap<QString,Tileset>* Quest::getTilesets()
{
    return &tileSets;
}

QList<Tileset*> Quest::getTilesetList()
{
    QList<Tileset*> tilesetList = QList<Tileset*>();
    for(QMap<QString,Tileset>::iterator iter = tileSets.begin(); iter != tileSets.end(); iter++)
    {
        tilesetList.append(&iter.value());
    }
    return tilesetList;
}

void Quest::addTileSet(Tileset tileset)
{
    tileSets.insert(tileset.getName(), tileset);
}

bool Quest::removeTileset(QString name)
{
    Tileset* set = getTileset(name);
    if(set == nullptr)
        return false;
    else
    {
        data.remove(QString("tilesets") + QDir::separator() + name + DAT_EXT); // Remove from loaded data (to prevent recreating when we save)
        tileSets.remove(name); // Remove from tilesets

        // Calculate file paths
        QString datFile = rootDir.absolutePath() + QDir::separator() + "tilesets" + QDir::separator() + name + DAT_EXT;
        QString tilesFile = rootDir.absolutePath() + QDir::separator() + "tilesets" + QDir::separator() + name + ".tiles.png";
        QString entitiesFile = rootDir.absolutePath() + QDir::separator() + "tilesets" + QDir::separator() + name + ".entities.png";

        // Remove all files, return the result
        return QFile::remove(datFile) && QFile::remove(tilesFile) & QFile::remove(entitiesFile);
    }
}

bool Quest::checkForChanges()
{
    QMap<QString,QSharedPointer<Table>>::iterator iter;

    // Loop through all loaded data, loading in tables from the disk with the same file names. Check them for equality.
    for(iter = data.begin(); iter != data.end(); iter++)
    {
        Table onDisk(iter.value().data()->getFilePath());

        // If the data does not exist on disk, or the data in memory differs from that on the disk, changes were made.
        if((*iter.value().data()) != onDisk || !onDisk.existsOnDisk())
            return true;
    }

    return false;
}

void Quest::buildManagerScript()
{
    hero.setStartingMap("\"x" + QString::number(space.getStartingArea().x()) + "y" + QString::number(space.getStartingArea().y()) + "\"");

    managerScript.setHero(this->hero);

    QFile file(rootDir.absolutePath() + QDir::separator() + "scripts" + QDir::separator() + "game_manager.lua");
    if(file.exists())
        file.remove();

    managerScript.writeToFile(file);
}
