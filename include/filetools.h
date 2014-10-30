#ifndef FILETOOLS_H
#define FILETOOLS_H

#include <functional>
#include <QMap>
#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QVector>
#include <QChar>

const QVector<QChar> FIND_OBJ_DELIMS =  {'{'};
const QVector<QChar> OBJ_DELIMS =       {'}'};
const QVector<QChar> ELEM_DELIMS =      {'='};
const QVector<QChar> VAL_DELIMS =       {','};

typedef QMap<QString,QString> Object;

/*!
 * \brief Class representing a table of data from a .dat file.
 */
class Table
{
public:
    Table();
    Table(QString filePath);
    virtual ~Table();

    /*!
     * \brief Parses a given file into the table.
     * \param filePath The filepath to read into the table.
     */
    void parse(QString filePath);

    /*!
     * \brief Retrieve an object's collection of elements and their values. If multiple objects of the same name are found, returns
     *        the last object added to the table.
     * \param objectName The name of the object to retrieve from the table.
     * \return
     */
   Object getObject(QString objectName);

    /*!
     * \brief Get the value of an element from an object. If multiple objects of the same name are found, returns
     *        the value from the last object added to the table.
     * \param objectName The name of the object.
     * \param elementName The name of the element.
     * \return
     */
    QString getElementValue(QString objectName, QString elementName);

    /*!
     * \brief Get the file path associated with this table.
     * \return The file path associated with this table.
     */
    QString getFilePath() const;

    /*!
     * \brief Retrieves a QList of all objects with the given name.
     * \param objectName The name of the objects to retrieve.
     * \return List of all objects with the given name.
     */
    QList<Object> getObjectsOfName(QString objectName);

    /*!
     * \brief Searches the table for the first object with the given element set to the given value.
     * \param objectName The object name to search under.
     * \param elementName The element to search for.
     * \param value The value to search for.
     * \return
     */
    Object getObjectWithValue(QString objectName, QString elementName, QString value);


private:
    void beginRead();
    void findObj();
    void readObj();
    void readElements();

    /*!
     * \brief readUntil Read a given text stream until the delimiter value is found.
     * \param delim The delimiter value.
     * \return Value read until the given delimiter value.
     */
    QString readUntil(QTextStream& stream, QVector<QChar> delims);

    QString currentObjectName, currentElement, currentObject; /*!< The current object and element in the parser state. */

    Object curObjectData; /*!< Data stored by the current object. */
    QFile file;                           /*!< The file currently being used for reading. */
    QTextStream in;                       /*!< The stream currently being used for reading. */

    QString filePath;
    QMultiMap<QString, Object> objects; /*!< Map of all objects, containing a map of respective elements. */

};

#endif // FILETOOLS_H
