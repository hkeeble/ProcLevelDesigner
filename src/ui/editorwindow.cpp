#include "editorwindow.h"
#include "ui_editorwindow.h"


EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);

    // Compile list of actions only available when a quest is loaded
    questOnlyActions = QList<QAction*>();
    questOnlyActions.append(ui->actionClose);
    questOnlyActions.append(ui->actionQuest_Options);
    questOnlyActions.append(ui->actionSave_Quest);
    questOnlyActions.append(ui->actionNew_Map);
    questOnlyActions.append(ui->actionNew_Script);
    questOnlyActions.append(ui->actionNew_Tileset);
    questOnlyActions.append(ui->actionQuest_Database);

    questOnlyWidgets = QList<QWidget*>();
    questOnlyWidgets.append(ui->tabView);

    setQuestOnlyUIEnabled(false);

    keyEventModel = nullptr;
    gateModel = nullptr;

    setWindowTitle("ProcLevelDesigner");
}

EditorWindow::~EditorWindow()
{
    if(keyEventModel)
        delete keyEventModel;
    if(gateModel)
        delete gateModel;

    delete ui;
}


void EditorWindow::setQuestOnlyUIEnabled(bool enabled)
{
    // Actions
    for(QAction* action : questOnlyActions)
        action->setEnabled(enabled);

    // Widgets
    for(QWidget* widget : questOnlyWidgets)
        widget->setEnabled(enabled);
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
        quest = Quest(dialog->getFolderPath() + QDir::separator() + "data" + QDir::separator());

        if(quest.Init()) // Attempt to initialize quest from the given path
        {
            // Populate the trees and set window title
            setWindowTitle("ProcLevelDesigner - " + quest.getName());

            // Initialize the UI with quest data
            initQuestUI();

            setQuestOnlyUIEnabled(true);
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
            QString questPath = dialog->getFolderPath() + QDir::separator() + "data" + QDir::separator();
            quest = Quest(questPath);

            copyFolder(QDir().currentPath() + QDir::separator() + "game_data" + QDir::separator(),
                       questPath);

            // Modify the quest object
            Table* questData = quest.getData(DAT_QUEST);
            questData->setElementValue(OBJ_QUEST, ELE_NAME, dialog->getQuestName());
            questData->setElementValue(OBJ_QUEST, ELE_WRT_DIR, dialog->getQuestName());
            questData->setElementValue(OBJ_QUEST, ELE_SOL_VERS, SOLARUS_VERSION);

            // Save out all modified quest data
            quest.saveData();

            setWindowTitle("ProcLevelDesigner - " + quest.getName());

            setQuestOnlyUIEnabled(true);
        }
    }
    delete dialog;
}

void EditorWindow::initQuestUI()
{
    // Initialize lists of key events and gates
    if(!keyEventModel)
        keyEventModel = new QStandardItemModel;
    else
        keyEventModel->clear();

    if(!gateModel)
        gateModel = new QStandardItemModel;
    else
        gateModel->clear();

    QList<Gate*> gates = quest.getGateList();
    QList<Key*> keyEvents = quest.getKeyEventList();

    gateModel->setRowCount(gates.length());
    keyEventModel->setRowCount(keyEvents.length());

    for(int i = 0; i < gates.length(); i++)
        gateModel->setItem(i, new QStandardItem(gates[i]->getName()));

    for(int i = 0; i < keyEvents.length(); i++)
        keyEventModel->setItem(i, new QStandardItem(keyEvents[i]->getName()));

    ui->keyEventList->setModel(keyEventModel);
    ui->gateList->setModel(gateModel);

    ui->keyEventList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->gateList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void EditorWindow::on_actionExit_triggered()
{
    this->close();
}

void EditorWindow::on_actionSave_Quest_triggered()
{
    quest.saveData();
}

void EditorWindow::on_actionClose_triggered()
{
    quest.clear();

    setQuestOnlyUIEnabled(false);

    setWindowTitle("ProcLevelDesigner");
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
    Map map = Map(32, 40, 40);
    map.setMusic(DEFAULT_MAP_MUSIC);
    map.setName("second_map");
    map.setTileSet(quest.getTileset("field"));

    for(int x = 0; x < map.getWidth(); x++)
    {
        for(int y = 0; y < map.getHeight(); y++)
        {
            MapTile tile = MapTile(0, x, y, map.getTileSize(), ((double) rand() / (RAND_MAX)) + 1);
            map.setTile(x, y, MapTile(tile));
        }
    }

    Table* data = quest.getData(QString("maps") + QDir::separator() + map.getName());
    map.build(data);
    writeToFile(QFileInfo(data->getFilePath()).absoluteDir().absolutePath(), "second_map.lua", ""); // Write map script file

    Table* database = quest.getData(DAT_DATABASE);
    database->addObject(OBJ_MAP, map.getObject());
    quest.saveData();
}

void EditorWindow::on_actionQuest_Database_triggered()
{
    QuestDatabase* dialog = new QuestDatabase(&quest, this);
    dialog->exec();
    setWindowTitle("ProcLevelDesigner - " + quest.getData(DAT_QUEST)->getElementValue(OBJ_QUEST, ELE_NAME));
    delete dialog;
}

void EditorWindow::on_newKeyEventButton_clicked()
{
    EditKeyEvent* dialog = new EditKeyEvent(this);
    if(dialog->exec() == QDialog::Accepted)
    {

    }
}
