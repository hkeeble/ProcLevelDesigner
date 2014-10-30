#ifndef FILETOOLS_H
#define FILETOOLS_H

#include <functional>
#include <QMap>
#include <QString>
#include <QFile>
#include <QTextStream>

QFile currentFile; /*!< The file currently being parsed. */
QTextStream currentStream;

struct ParseState; // Forward Declaration

/*!
 * \brief Parsing function commands.
 */
enum class State
{
    R_OBJECT,   /*!< Search for an object name. */
    R_ELEMENT,  /*!< Search for an element name. */
    R_VALUE,    /*!< Search for a value name. */
    FAILED      /*!< The error state. */
};

/*!
 * \brief Reads a section of the opened file, under a given command.
 * \param command
 */
void read(ParseState state);

/*!
 * \brief State machine implementation for file parsing.
 */
struct ParseState
{
    State state, stateIfSucceed, stateIfFailed;
    std::function<void(ParseState)> action;
} actionTable[] {
    {State::R_OBJECT,   State::R_ELEMENT, State::FAILED,   read}, // Initial state
    {State::R_ELEMENT,  State::R_VALUE,   State::R_OBJECT, read},
    {State::R_VALUE,    State::R_ELEMENT, State::FAILED,   read},
    {State::FAILED,     State::R_ELEMENT, State::FAILED,   read}
};

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

private:
    QString filePath;
    QMap<QString, QMap<QString, QString> > objects; /*!< Map of all objects, containing a map of respective elements. */

};

#endif // FILETOOLS_H
