#include "editorwindow.h"
#include "ui_editorwindow.h"


EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);

    // Compile list of actions only available when a quest is loaded
    questOnlyActions = QList<QAction*>();
    questOnlyActions.insert(questOnlyActions.size(), ui->actionClose);
    questOnlyActions.insert(questOnlyActions.size(), ui->actionQuest_Options);
    questOnlyActions.insert(questOnlyActions.size(), ui->actionSave_Quest);

    setWindowTitle("ProcLevelDesigner");
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::build()
{
    // Build maps
}

void EditorWindow::run()
{
    // Run Solarus
}

void EditorWindow::on_actionOpen_Quest_triggered()
{
    ui->actionClose->trigger(); // Close the current quest

    OpenQuestDialog* dialog = new OpenQuestDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        quest = Quest(dialog->getFolderPath());

        if(quest.Init()) // Attempt to initialize quest from the given path
        {
            // Populate the trees and set window title
            populateScriptView(quest.getFSModel(), dialog->getFolderPath());
            populateMapView(quest.getFSModel(), dialog->getFolderPath());

            setWindowTitle("ProcLevelDesigner - " + quest.getName());

            // Enable actions
            for(QAction* action : questOnlyActions)
                action->setEnabled(true);
        }
        else
            QMessageBox::warning(this, "Error", "No valid quest was found in this folder.", QMessageBox::Ok);
    }
}

void EditorWindow::on_actionNew_Quest_triggered()
{
    ui->actionClose->trigger(); // Close the current quest

    NewQuestDialog* dialog = new NewQuestDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        if(!QDir(dialog->getFolderPath()).exists()) // Create the directory if neccesary
            QDir().mkpath(dialog->getFolderPath());

        // Check for existing data in the specific file path
        QFileInfoList existingFiles = QDir(dialog->getFolderPath() + QDir::separator()).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        if(existingFiles.size() != 0)
            QMessageBox::warning(this, "Error", "Existing data found in specified file path. Please use an empty directory, or specify one that does not yet exist.",
                                 QMessageBox::Ok);
        else
        {
            quest = Quest(dialog->getFolderPath());

            copyFolder(QDir().currentPath() + QDir::separator() + "game_data" + QDir::separator(),
                       dialog->getFolderPath());

            // Modify the quest object
            Table* questData = quest.getData(DAT_QUEST);
            questData->setElementValue(OBJ_QUEST, ELE_NAME, dialog->getQuestName());
            questData->setElementValue(OBJ_QUEST, ELE_WRT_DIR, dialog->getQuestName());
            questData->setElementValue(OBJ_QUEST, ELE_SOL_VERS, SOLARUS_VERSION);

            // Save out all modified quest data
            quest.saveData();

            // Populate the trees and set window title
            populateScriptView(quest.getFSModel(), dialog->getFolderPath());
            populateMapView(quest.getFSModel(), dialog->getFolderPath());

            setWindowTitle("ProcLevelDesigner - " + quest.getName());
        }
    }
    delete dialog;
}

void EditorWindow::on_actionExit_triggered()
{
    this->close();
}

void EditorWindow::populateScriptView(QFileSystemModel* model, QString rootDir)
{
    // Populate the tree view with quest file model
    ui->scriptsView->setModel(model);
    ui->scriptsView->setRootIndex(quest.getFSModel()->index(rootDir + QDir::separator() + "scripts" + QDir::separator()));

    // Hide uneccesary columns
    ui->scriptsView->hideColumn(1);
    ui->scriptsView->hideColumn(2);
    ui->scriptsView->hideColumn(3);
}

void EditorWindow::populateMapView(QFileSystemModel* model, QString rootDir)
{
    // Populate the tree view with quest file model
    ui->mapsView->setModel(model);
    ui->mapsView->setRootIndex(quest.getFSModel()->index(rootDir + QDir::separator() + "maps" + QDir::separator()));

    // Hide uneccesary columns
    ui->mapsView->hideColumn(1);
    ui->mapsView->hideColumn(2);
    ui->mapsView->hideColumn(3);
}

void EditorWindow::on_actionSave_Quest_triggered()
{
    quest.saveData();
}

void EditorWindow::on_actionClose_triggered()
{
    quest.clear();

    // Disable menu items
    for(QAction* action : questOnlyActions)
        action->setEnabled(false);

    setWindowTitle("ProcLevelDesigner");
}

void EditorWindow::on_actionQuest_Options_triggered()
{
    QuestOptionsDialog* dialog = new QuestOptionsDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        quest.getData(DAT_QUEST)->setElementValue(OBJ_QUEST, ELE_NAME, dialog->getName());
        setWindowTitle("ProcLevelDesigner - " + quest.getName());
    }
    delete dialog;
}

void EditorWindow::on_mapsView_doubleClicked(const QModelIndex &index)
{
    // Open up the map!
}
