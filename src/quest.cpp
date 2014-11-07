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

    fsModel->setRootPath(rootDir.absolutePath());
    scriptModel->setRootPath(rootDir.absolutePath());
    mapModel->setRootPath(rootDir.absolutePath() + QDir::separator() + "maps" + QDir::separator());

    initFilters();
}

bool Quest::Init()
{
    Table* quest = getData(DAT_QUEST);
    if(quest == nullptr)
        return false;
    else
    {


        return true;
    }
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

QString Quest::getName()
{
    Table* quest = getData(DAT_QUEST);
    if(quest != nullptr)
        return quest->getElementValue(OBJ_QUEST, ELE_NAME);
    else
        return "NULL_NAME";
}

void Quest::saveData() const
{
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
