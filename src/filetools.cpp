#include "filetools.h"

Table::Table()
{
    objects = QMap<QString, QMap<QString, QString> >();
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

QMap<QString, QString> Table::getObject(QString objectName)
{
    return objects.find(objectName).value();
}

QString Table::getElementValue(QString objectName, QString elementName)
{
    return objects.find(objectName).value().find(elementName).value();
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
    curObjectData = QMap<QString, QString>();

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

void Table::readFail()
{

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
