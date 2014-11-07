#ifndef QUEST_H
#define QUEST_H

#include <QDir>
#include <QFileSystemModel>
#include <QMap>
#include <QSharedPointer>

#include "filetools.h"
#include "map.h"

// The solarus version supported by this quest object
const QString SOLARUS_VERSION = "1.3";

/*!
 * \brief The Quest class. Represents a quest.
 */
class Quest
{
public:
    Quest();

    /*!
     * \brief Quest Constructor
     * \param dirPath The directory path in which the quest data resides.
     */
    Quest(QString dirPath);
    virtual ~Quest();

    Quest& operator=(const Quest& param);
    Quest(const Quest& param);

    QFileSystemModel* getFSModel(); /*!< Retrieves the file system model representing this quest. */
    QDir getRootDir() const; /*!< Retrieves the root directory for this quest. */

    bool Init(); /*!< Initializes the quest. */

    /*!
     * \brief Retrieves a table from the given filepath. If no data exists, creates a blank table to be written to the
     *        specified path.
     * \param filePath The filepath of the .dat file. Filepath should be relative to the quest directory.
     * \return Pointer to the table containing the data.
     */
    Table* getData(QString filePath);

    /*!
     * \brief Get the name of the quest if loaded.
     * \return The name of the quest. Empty string if no quest is loaded.
     */
    QString getName();

    /*!
     * \brief Writes out all currently loaded data to the disk. Should save any changes that have been made to data in program memory.
     */
    void saveData() const;

    /*!
     * \brief Clears all quest data, making this object a blank quest.
     */
    void clear();

private:
    QDir rootDir;
    QFileSystemModel* fsModel; /*!< The file system model representing this quest. */

    QMap<QString,QSharedPointer<Table>> data; /*!< Map containing all the currently loaded data for this quest. */

    QVector<Map> maps; /*!< The maps contained within this quest. */

    void cpy(const Quest& param);
};

#endif // QUEST_H
