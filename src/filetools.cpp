#include "filetools.h"

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
    file.setFileName(QDir::currentPath() + "/" + filePath);
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

    if(!obj->isEmpty())
    {
        auto elemIter = obj->find(elementName); // Find element
        if(elemIter != obj->end())
            return elemIter.value();
        else
            return "NULL_ELEMENT"; // Element not found
    }
    else
        return "NULL_OBJECT"; // Object was not found
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
        Object::const_iterator iter = objs[i]->find(elementName);

        if(iter != objs[i]->end())
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
        curObjectData.insert(element, value);
    }

    // Build the object
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

void Table::setElementValue(QString objectName, QString elementName, QString value)
{
    getObject(objectName)->find(elementName).value() = value;
}

void Table::saveToDisk()
{
    file.setFileName(QDir::currentPath() + "/" + filePath);
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
    for(auto obj : objects.toStdMap())
        writeObj(obj.first, obj.second);
}

void Table::writeObj(const QString& objectName, const Object& object)
{
    out << objectName << "{\n";
    for(auto element : object.toStdMap())
        out << element.first << " = \"" << element.second << "\",\n";
    out << "}\n\n";
}
