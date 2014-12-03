#include "filetools.h"

void copyFolder(QString sourceDir, QString destinationDir)
{
    QDir srcDir(sourceDir);
    if(!srcDir.exists())
        return;

    QDir destDir(destinationDir);
    if(!destDir.exists())
        QDir().mkdir(destinationDir);

    QStringList files = srcDir.entryList(QDir::Files); // Retrieve all files in current folder
    for(int i = 0; i < files.size(); i++) // Copy all files
    {
        QString srcName = sourceDir + QDir::separator() + files[i];
        QString destName = destinationDir + QDir::separator() + files[i];
        QFile::copy(srcName, destName);
    }
    files.clear();

    files = srcDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot); // Retrieve all folders in current folder
    for(int i = 0; i < files.size(); i++) // Recursively copy all folders
    {
        QString srcName = sourceDir + QDir::separator() + files[i];
        QString destName = destinationDir + QDir::separator() + files[i];
        copyFolder(srcName, destName);
    }
}

void writeToFile(QString dirPath, QString fileName, QString fileContents)
{
    if(!QDir().exists(dirPath))
        QDir().mkpath(dirPath);

    QFile file(dirPath + QDir::separator() + fileName);
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);
    out << fileContents;
    out.flush();

    file.close();
}

QString Object::find(QString element, QString defaultVal)
{
    ObjectData::iterator iter = data.find(element);
    if(iter == data.end())
        return defaultVal;
    else
        return iter.value();
}

void Object::insert(QString element, QString value)
{
    data.insert(element, value);
}

bool Object::operator==(const Object& param) const
{
    return (data == param.data);
}

bool Object::operator!=(const Object& param) const
{
    return !(*this == param);
}

Table::Table()
{
    objects = QMap<QString, Object>();
    filePath = QString();
}

Table::Table(QString filePath)
{
    this->filePath = filePath;
    parse(filePath);
}

Table::~Table()
{

}

void Table::parse(QString filePath)
{
    // Open file and stream for reading
    file.setFileName(filePath);
    if(file.open(QIODevice::ReadOnly)) // Begin parse
    {
        in.setDevice(&file);

        // Begin parse
        beginRead();

        // Close file
        in.flush();
        file.close();
    }
    else
        return;
}

void Table::addObject(QString name, Object object)
{
    objects.insert(name, object);
}

Object* Table::getObject(QString objectName)
{
    QMap<QString,Object>::iterator iter = objects.find(objectName); // find object

    if(iter != objects.end())
        return &iter.value();
    else
        return nullptr;
}

QString Table::getElementValue(QString objectName, QString elementName)
{
    Object* obj = getObject(objectName);

    if(obj != nullptr)
       return obj->find(elementName, NULL_ELEMENT); // Find element
    else
        return NULL_OBJECT; // Object was not found
}

QList<Object*> Table::getObjects()
{
    QList<Object*> list;

    QMap<QString,Object>::iterator iter;
    for(iter = objects.begin(); iter != objects.end(); iter++)
            list.insert(list.size(), &iter.value());

    return list;
}

QList<Object*> Table::getObjectsOfName(QString objectName)
{
    QList<Object*> list;
    QMap<QString,Object>::iterator iter;
    for(iter = objects.begin(); iter != objects.end(); iter++)
    {
        if(iter.key() == objectName)
            list.insert(list.size(), &iter.value());
    }

    return list;
}

Object* Table::getObjectWithValue(QString objectName, QString elementName, QString value)
{
    auto objs = getObjectsOfName(objectName); // First, retrieve list of all objects

    // Search for first object with given element/value pair
    for(int i = 0; i < objs.size(); i++)
    {
        ObjectData::const_iterator iter = objs[i]->data.find(elementName);

        if(iter != objs[i]->data.end())
        {
            if(iter.value() == value)
                return objs[i];
        }
    }

    // if not found, return null
    return nullptr;
}

void Table::beginRead()
{
    while(file.isOpen() && !in.atEnd())
         findObj();
}

void Table::findObj()
{
    currentObjectName = readUntil(in, FIND_OBJ_DELIMS).simplified();

    if(!in.atEnd())
        readObj();
    else
        return;
}

void Table::readObj()
{
    currentObject = readUntil(in ,OBJ_DELIMS).simplified();

    if(!in.atEnd())
        readElements();
    else
        return;
}

void Table::readElements()
{
    curObjectData = Object();

    QTextStream str(&currentObject);
    while(!str.atEnd())
    {
        QString element = readUntil(str, ELEM_DELIMS).simplified();
        QString value = readUntil(str, VAL_DELIMS).simplified().remove('"');
        curObjectData.data.insert(element, value);
    }

    // build the object
    objects.insert(currentObjectName, curObjectData);
}

QString Table::readUntil(QTextStream& stream, QVector<QChar> delims)
{
    QString current = " ", concat = " ";
    bool stop = false;

    while(!stop && !stream.atEnd()){
        current = stream.read(1);

        for(int i = 0; i < delims.size(); i++)
        {
            if(delims[i] == current)
                stop = true;
        }

        if(!stop)
            concat.append(current);
    }

    return concat;
}

void Table::setFilePath(QString filePath)
{
    this->filePath = filePath;
}

QString Table::getFilePath() const
{
    return filePath;
}

bool Table::setElementValue(QString objectName, QString elementName, QString value)
{
    Object* obj = getObject(objectName);
    if(obj != nullptr)
    {
        ObjectData::iterator iter = obj->data.find(elementName);
        if(iter != obj->data.end())
        {
            iter.value() = value;
            return true;
        }

        return false; // Element not found
    }

    return false; // Object not found

}

bool Table::isEmpty() const
{
    return objects.isEmpty();
}

void Table::saveToDisk()
{
    file.setFileName(filePath);
    if(file.open(QIODevice::WriteOnly)) // Begin write
    {
        out.setDevice(&file);

        // Write out data
        beginWrite();

        out.flush();
        file.close();
    }
}


void Table::beginWrite()
{
    for(auto obj = objects.begin(); obj != objects.end(); obj++)
        writeObj(obj.key(), obj.value());
}

void Table::writeObj(QString objectName, Object object)
{
    out << objectName << "{";
    for(auto element = object.data.begin(); element != object.data.end(); element++)
        out << element.key() << " = \"" << element.value() << "\", ";
    out << "}\n";
}

bool Table::existsOnDisk()
{
    return QFileInfo(filePath).exists();
}

bool Table::operator==(const Table& param) const
{
    return (objects == param.objects);

}

bool Table::operator!=(const Table& param) const
{
    return !(*this == param);
}

void Table::clear()
{
    objects.clear();
}
