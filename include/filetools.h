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
     * \brief Retrieve an objects collection of elements and their values.
     * \param objectName The name of the object to retrieve from the table.
     * \return
     */
    QMap<QString, QString> getObject(QString objectName);

    /*!
     * \brief Get the value of an element from an object.
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

private:
    struct ParseState; // Forward Declaration

    /*! State callbacks. */
    void beginRead();
    void findObj();
    void readObj();
    void readElements();
    void readValue();
    void readFail();
    void buildObject();

    /*!
     * \brief readUntil Read a given text stream until the delimiter value is found.
     * \param delim The delimiter value.
     * \return Value read until the given delimiter value.
     */
    QString readUntil(QTextStream& stream, QVector<QChar> delims);

    QString currentObjectName, currentElement, currentObject; /*!< The current object and element in the parser state. */

    QMap<QString, QString> curObjectData; /*!< Data stored by the current object. */
    QFile file;                           /*!< The file currently being used for reading. */
    QTextStream in;                       /*!< The stream currently being used for reading. */

    QString filePath;
    QMultiMap<QString, QMap<QString, QString> > objects; /*!< Map of all objects, containing a map of respective elements. */

};

#endif // FILETOOLS_H
