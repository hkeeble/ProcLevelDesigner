#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDir>
#include <QFileSystemModel>
#include <QList>
#include <QStringListModel>

#include "common.h"
#include "preferences.h"
#include "newquestdialog.h"
#include "openquestdialog.h"
#include "solarusdirectorydialog.h"
#include "editkeyevent.h"
#include "editgatedialog.h"
#include "editzonedialog.h"
#include "questdatabase.h"
#include "quest.h"
#include "filetools.h"
#include "applicationdispatcher.h"
#include "missionstructurescene.h"
#include "spacescene.h"

namespace Ui {
class EditorWindow;
}

/*! \brief Main Editor Window Class.
 *
 *  Contains all main program functionality.
  */
class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

private slots:
    // Menu Actions
    void on_actionOpen_Quest_triggered();
    void on_actionExit_triggered();
    void on_actionNew_Quest_triggered();
    void on_actionSave_Quest_triggered();
    void on_actionClose_triggered();
    void on_actionNew_Map_triggered();
    void on_actionQuest_Database_triggered();
    void on_actionRun_triggered();
    void on_actionSet_Solarus_Directory_triggered();
    void on_actionOpen_Recent_Quest_triggered(QAction* action);

    // Key Event Buttons
    void on_newKeyEventButton_clicked();
    void on_editKeyEventButton_clicked();
    void on_removeKeyEventButton_clicked();

    // Gate Buttons
    void on_newGateButton_clicked();
    void on_editGateButton_clicked();
    void on_removeGateButton_clicked();

    // Generate mission button
    void on_generateMissionButton_clicked();

    // Zone Buttons
    void on_newZoneButton_clicked();
    void on_editZoneButton_clicked();
    void on_removeZoneButton_clicked();

protected:
    void closeEvent(QCloseEvent *event) override final;

private:
    bool openQuest(QString path); /*!< Open quest at the specified path. Returns true if quest was opened successfully. */
    void createNewQuest(QString name, QString folderPath); /*!< Create a new quest at the given path. */

    void addRecentQuestPath(QString path); /*!< Adds a recently opened quest to the menu. */
    void removeRecentQuestPath(QString path); /*!< Removes a recently opened questt from the menu. */

    void initQuestUI(); /*!< Initializes the user interface, filling it with all data loaded about the current quest. */

    void updateKeyList();   /*!< Updates the event list with all data currently in the quest's mission. */
    void updateGateList(); /*!< Updates the gate list with all data currently in the quest's mission. */
    void updateZoneList(); /*!< Updates the zone list with all data currently in the quest's space. */

    Key* getSelectedKey();
    Gate* getSelectedGate();
    Zone* getSelectedZone();

    void setQuestOnlyUIEnabled(bool enabled);

    void clearRunningGame(); /*!< Clears the running game process if neccesary. */

    QProcess* runningGame; /*!< The currently running game process. */

    Ui::EditorWindow *ui;   /*!< Pointer to the window UI elements. */
    QString appDir;         /*!< The location of the application. */

    Preferences preferences; /*!< Saved user preferences (also manages quest history). */

    Quest quest; /*!< The currently loaded quest. */

    QList<QAction*> questOnlyActions; /*!< List of actions only available when a quest is loaded. */
    QList<QWidget*> questOnlyWidgets; /*!< List of widgets only available when a quest is loaded. */

    // Key/Gate models and data
    QStringListModel* keyEventModel; /*!< Model used to represent key events. */
    QStringListModel* gateModel;     /*! Model used to represent gates. */
    QStringListModel* zoneModel;
    QStringList keyData;
    QStringList gateData;
    QStringList zoneData;

    // Mission scene
    MissionStructureScene* missionStructureScene;

    // Space scene
    SpaceScene* spaceScene;

    // Recent quest array
    QStringList recentQuests;
};

#endif // EDITORWINDOW_H
