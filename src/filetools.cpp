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
    QFile file(filePath);
    QTextStream stream(&file);
    if(currentFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Begin Parse
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

void read(ParseState state)
{
    if(state.state == State::R_OBJECT)
    {
        // Reading an object

        QString current = "";
        QString concat = "";
        do{
        current = currentStream.read(1);
        if(current != "{")
            concat.append(current);
        } while(current != "{");

        // Remove all unneccesary whitespace (ignores newlines)
        concat = concat.simplified();

    }
    else if(state.state == State::R_ELEMENT)
    {
    }
    else if(state.state == State::R_VALUE)
    {
    }
    else if(state.state == State::FAILED)
    {
    }
}
