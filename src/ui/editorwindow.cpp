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
    questOnlyActions.insert(questOnlyActions.size(), ui->actionNew_Map);
    questOnlyActions.insert(questOnlyActions.size(), ui->actionNew_Script);

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
            populateTreeViews(dialog->getFolderPath());
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
            populateTreeViews(dialog->getFolderPath());

            setWindowTitle("ProcLevelDesigner - " + quest.getName());
        }
    }
    delete dialog;
}

void EditorWindow::on_actionExit_triggered()
{
    this->close();
}

void EditorWindow::populateTreeViews(QString rootDir)
{
    // Populate the script view
    ui->scriptsView->setModel(quest.getScriptModel());
    ui->scriptsView->setRootIndex(quest.getScriptModel()->index(rootDir));

    // Populate the tree view with quest file model
    ui->mapsView->setModel(quest.getMapModel());
    ui->mapsView->setRootIndex(quest.getMapModel()->index(rootDir + QDir::separator() + "maps" + QDir::separator()));

    // Hide uneccesary columns
    ui->scriptsView->hideColumn(1);
    ui->scriptsView->hideColumn(2);
    ui->scriptsView->hideColumn(3);
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
    if(index.isValid())
    {
        QString mapPath = quest.getMapModel()->fileName(index);
        mapPath.remove(mapPath.length()-4, 4);
        Table* table = quest.getData(QString("maps") + QDir::separator() + mapPath);
        Map map = Map::parse(mapPath, table);
    }
}

void EditorWindow::on_actionNew_Map_triggered()
{
    Map map = Map(32, 320, 320);
    map.setMusic(DEFAULT_MAP_MUSIC);
    map.setName("second_map");
    map.setTileSet("castle");

    for(int x = 0; x < map.getWidth(); x++)
    {
        for(int y = 0; y < map.getHeight(); y++)
        {
            MapTile tile = MapTile(0, x, y, map.getTileSize(), 0);
            map.setTile(x, y, MapTile(tile));
        }
    }

    Table* data = quest.getData(QString("maps") + QDir::separator() + map.getName());
    map.build(data);
}
