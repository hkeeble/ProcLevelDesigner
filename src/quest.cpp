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

    maps = QMap<QString,Map>();
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

        // Load all existing maps
        QDir mapDir = QDir(rootDir.absolutePath() + QDir::separator() + "maps" + QDir::separator());
        mapDir.setNameFilters(filters);
        QFileInfoList mapData = mapDir.entryInfoList();

        for(QFileInfo f : mapData)
        {
            Table* data = getData(QString("maps") + QDir::separator() + f.baseName());
            maps.insert(f.baseName(), Map::parse(f.baseName(), data));
        }

        // Initialize the mission
        Table* missionData = getData(DAT_MISSION);
        mission = Mission::Parse(missionData);

        // Initialize the space
        Table* spaceData = getData(DAT_SPACE);
        space = Space::Parse(spaceData);

        return true;
    }
}

void Quest::build()
{
    mission.build(getData(DAT_MISSION));
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
}

void Quest::saveData()
{
    build();

    // Loop through all loaded data tables
    for(auto iter : data.toStdMap())
    {
        iter.second.data()->saveToDisk();
    }
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

    fsModel = scriptModel = mapModel = nullptr;

}

Map* Quest::getMap(QString name)
{
    return &(*maps.find(name));
}

QMap<QString,Map>* Quest::getMaps()
{
    return &maps;
}

QList<Map*> Quest::getMapList()
{
    QList<Map*> mapList = QList<Map*>();
    for(QMap<QString,Map>::iterator iter = maps.begin(); iter != maps.end(); iter++)
    {
        mapList.append(&iter.value());
    }
    return mapList;
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
