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

Object Table::getObject(QString objectName)
{
    auto iter = objects.find(objectName); // find object

    if(iter != objects.end())
        return iter.value();
    else
        return QMap<QString,QString>();
}

QString Table::getElementValue(QString objectName, QString elementName)
{
    Object obj = getObject(objectName);

    if(!obj.isEmpty())
    {
        auto elemIter = obj.find(elementName); // Find element
        if(elemIter != obj.end())
            return elemIter.value();
        else
            return "NULL_ELEMENT"; // Element not found
    }
    else
        return "NULL_OBJECT"; // Object was not found

    return objects.find(objectName).value().find(elementName).value();
}

QList<Object> Table::getObjectsOfName(QString objectName)
{
    return objects.values(objectName);
}

Object Table::getObjectWithValue(QString objectName, QString elementName, QString value)
{
    auto objs = getObjectsOfName(objectName); // First, retrieve list of all objects

    // Search for first object with given element/value pair
    for(int i = 0; i < objs.size(); i++)
    {
        Object::const_iterator iter = objs[i].find(elementName);

        if(iter != objs[i].end())
        {
            if(iter.value() == value)
                return objs[i];
        }
    }

    // if not found, return an empty object.
    return Object();
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
