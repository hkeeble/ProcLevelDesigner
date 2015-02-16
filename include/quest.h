#ifndef QUEST_H
#define QUEST_H

#include <QDir>
#include <QFileSystemModel>
#include <QMap>
#include <QSharedPointer>
#include <QList>

#include "filetools.h"
#include "map.h"
#include "mission.h"
#include "space.h"
#include "teletransporter.h"
#include "gamemanagerscript.h"

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

    QFileSystemModel* getFSModel();     /*!< Retrieves the main file system model representing this quest. */
    QFileSystemModel* getScriptModel(); /*!< Retrieves the file system model for scripts. */
    QFileSystemModel* getMapModel();    /*!< Retrieves the file system model for maps. */

    QDir getRootDir() const; /*!< Retrieves the root directory for this quest. */

    QDir getExecutableDir() const; /*!< Retrieves the directory from which this quest is executable through Solarus. */

    bool Init(); /*!< Initializes the quest. */

    /*!
     * \brief Builds space and mission into tables, ready for saving to disk.
     */
    void build();

    /*!
     * \brief Builds all map data.
     */
    void buildMaps();

    /*!
     * \brief Saves all currently built map data to disk.
     */
    void saveMaps();

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
    void saveData();

    /*!
     * \brief Clears all quest data, making this object a blank quest.
     */
    void clear();

    /*!
     * \brief getTileset Gets the tileset of the given name.
     * \param name The name of the tileset to retrieve.
     * \return Pointer to the tilset requested. If not found, returns null.
     */
    Tileset* getTileset(QString name);

    /*!
     * \brief getTilesets Gets the set of tilesets contained in this quest.
     */
    QMap<QString,Tileset>* getTilesets();

    /*!
     * \brief getTilesetList Gets the tilests contained in this quest as a list.
     */
    QList<Tileset*> getTilesetList();

    /*!
     * \brief removeTileset Removes the tileset with the given name from this quest.
     * \return Returns true if tileset was removed successfully. Returns false if the tileset was not found.
     */
    bool removeTileset(QString name);

    /*!
     * \brief addTileSet Adds the given tileset to the quest.
     * \param tileset The tileset to add to the quest.
     */
    void addTileSet(Tileset tileset);

    /*!
     * \brief checkForChanges Use this function to check all loaded data for differences on the disk. If there are differences, or data in the quest
     *                        does not exist on the disk, returns true.
     * \return True if changes need to be saved, false if not.
     */
    bool checkForChanges();

    // The components of this quest:
    Mission mission; /*!< The mission contained within this quest. */
    Space space;     /*!< The space contained within this quest. */

    void buildManagerScript();

    Hero* getHero() { return &hero; }

private:
    QDir rootDir;
    QFileSystemModel* fsModel;     /*!< The main file system model representing this quest. */
    QFileSystemModel* scriptModel; /*!< The file system model representing scripts. */
    QFileSystemModel* mapModel;    /*!< The file system model representing maps. */

    void initFilters();

    GameManagerScript managerScript;

    QMap<QString,QSharedPointer<Table>> data; /*!< Map containing all the currently loaded data for this quest. */

    QMap<QString,Tileset> tileSets; /*!< The tilesets contained within this quest. */

    QList<Map> maps; /*!< The maps currently built for this quest. */

    Hero hero; /*!< The hero for this quest. Contains the initial hero state. */

    void cpy(const Quest& param);
};

#endif // QUEST_H
