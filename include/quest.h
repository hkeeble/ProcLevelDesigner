#ifndef QUEST_H
#define QUEST_H

#include <QDir>
#include <QFileSystemModel>

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

private:
    QDir rootDir;
    QFileSystemModel* fsModel; /*!< The file system model representing this quest. */

    void cpy(const Quest& param);
};

#endif // QUEST_H
