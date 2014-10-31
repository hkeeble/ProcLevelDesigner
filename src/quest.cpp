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
    Table* quest = getData("quest");
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
    Table* quest = getData("quest");
    if(quest != nullptr)
        return quest->getElementValue("quest", "title_bar");
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
        fsModel = NULL;
}

Table* Quest::getData(QString filePath)
{
    QString absolutePath = getRootDir().absolutePath() + QDir::separator() + filePath + ".dat";

    auto iter = data.find(absolutePath);

    if(iter != data.end())
        return iter.value().data();
    else
    {
        Table* table = new Table(absolutePath);
        if(table->isEmpty())
        {
            delete table;
            return nullptr;
        }
        else
            data.insert(filePath, QSharedPointer<Table>(table));

        return table;
    }
}
