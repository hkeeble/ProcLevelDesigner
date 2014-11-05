#include "quest.h"

Quest::Quest()
{
    fsModel = NULL;
}

Quest::Quest(QString dirPath)
{
    rootDir = QDir(dirPath);
    fsModel = new QFileSystemModel();
    fsModel->setRootPath(rootDir.absolutePath());
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
    if(fsModel)
        delete fsModel;
}

QFileSystemModel* Quest::getFSModel()
{
    return fsModel;
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
    }
    else
        fsModel = nullptr;
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
    delete fsModel;
    fsModel = nullptr;
}
