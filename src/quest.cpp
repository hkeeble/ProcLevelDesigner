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
    QStringList files = rootDir.entryList(QDir::Files);

    if(files.contains("quest.dat") && files.contains("project_db.dat") && files.contains("main.lua"))
    {
        // Read quest name, solarus version etc.
        return true;
    }
    else
        return false;

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
