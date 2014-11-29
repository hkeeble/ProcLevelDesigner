#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDir>
#include <QFileSystemModel>
#include <QList>
#include <QStandardItem>
#include <QStandardItemModel>

#include "common.h"
#include "preferences.h"
#include "newquestdialog.h"
#include "openquestdialog.h"
#include "solarusdirectorydialog.h"
#include "editkeyevent.h"
#include "questdatabase.h"
#include "quest.h"
#include "filetools.h"
#include "applicationdispatcher.h"

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
    void on_actionOpen_Quest_triggered();
    void on_actionExit_triggered();
    void on_actionNew_Quest_triggered();
    void on_actionSave_Quest_triggered();
    void on_actionClose_triggered();
    void on_actionNew_Map_triggered();
    void on_actionQuest_Database_triggered();
    void on_newKeyEventButton_clicked();
    void on_actionRun_triggered();

    void on_actionSet_Solarus_Directory_triggered();

private:
    void build(); /*!< Builds and outputs data files. */
    void run(); /*!< Runs the currently output quest. */

    void initQuestUI(); /*!< Initializes the user interface, filling it with all data loaded about the current quest. */

    void setQuestOnlyUIEnabled(bool enabled);

    void clearRunningGame(); /*!< Clears the running game process if neccesary. */

    QProcess* runningGame; /*!< The currently running game process. */

    Ui::EditorWindow *ui;   /*!< Pointer to the window UI elements. */
    QString appDir;         /*!< The location of the application. */

    Preferences preferences;

    Quest quest; /*!< The currently loaded quest. */

    QList<QAction*> questOnlyActions; /*!< List of actions only available when a quest is loaded. */
    QList<QWidget*> questOnlyWidgets; /*!< List of widgets only available when a quest is loaded. */

    QStandardItemModel* keyEventModel; /*!< Model used to represent key events. */
    QStandardItemModel* gateModel; /*! Model used to represent gates. */
};

#endif // EDITORWINDOW_H
