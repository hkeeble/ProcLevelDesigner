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
    questOnlyActions.append(ui->actionRun);

    questOnlyWidgets = QList<QWidget*>();
    questOnlyWidgets.append(ui->tabView);

    setQuestOnlyUIEnabled(false);

    keyEventModel = nullptr;
    gateModel = nullptr;
    runningGame = nullptr;

    setWindowTitle("ProcLevelDesigner");

}

EditorWindow::~EditorWindow()
{
    if(keyEventModel)
        delete keyEventModel;
    if(gateModel)
        delete gateModel;

    clearRunningGame();

    delete ui;
}

// Close Event, ensures changes can be saved
void EditorWindow::closeEvent(QCloseEvent *event)
{
    if(quest.checkForChanges())
    {
        int result = QMessageBox::warning(this, "Unsaved Changes", "There are unsaved changes in this quest. Do you wish to save before closing?",
                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if(result == QMessageBox::Yes)
            quest.saveData();
        if(result != QMessageBox::Cancel)
            event->accept();
        else
            event->ignore();
    }
    else
        event->accept();
}

/* ------------------------------------------------------------------
 *  MENU ACTIONS
 * ------------------------------------------------------------------*/
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

void EditorWindow::on_actionNew_Map_triggered()
{
    Map map = Map(32, 40, 40);
    map.setMusic(DEFAULT_MAP_MUSIC);
    map.setName("second_map");
    map.setTileSet(quest.getTileset("main"));

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

void EditorWindow::on_actionQuest_Database_triggered()
{
    QuestDatabase* dialog = new QuestDatabase(&quest, this);
    dialog->exec();
    setWindowTitle("ProcLevelDesigner - " + quest.getData(DAT_QUEST)->getElementValue(OBJ_QUEST, ELE_TITLE_BAR));
    delete dialog;
}

void EditorWindow::on_actionRun_triggered()
{
    clearRunningGame();
    runningGame = ApplicationDispatcher::RunThroughTerminal(this, "solarus",
                                                            QStringList() << quest.getExecutableDir().absolutePath(), true);
}

void EditorWindow::on_actionSet_Solarus_Directory_triggered()
{
    SolarusDirectoryDialog* dialog = new SolarusDirectoryDialog(preferences.getSolarusPath(), this);
    if(dialog->exec() == QDialog::Accepted)
    {
        if(dialog->isDefault())
            preferences.setSolarusPath("DEFAULT");
        else
            preferences.setSolarusPath(dialog->getPath());

        preferences.saveToDisk();
    }

    delete dialog;
}

/* ------------------------------------------------------------------
 *  KEY BUTTON ACTIONS
 * ------------------------------------------------------------------*/
void EditorWindow::on_newKeyEventButton_clicked()
{
    EditKeyEvent* dialog = new EditKeyEvent(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        MissionItemCollection* items = quest.mission.getItems();
        items->AddKeyEvent(dialog->getName(), Key(dialog->getName(), dialog->getType(), dialog->getMessage()));
        updateKeyList();
    }
    delete dialog;
}


void EditorWindow::on_editKeyEventButton_clicked()
{
    Key* editKey = getSelectedKey();

    if(editKey != nullptr)
    {
        EditKeyEvent* dialog = new EditKeyEvent(editKey, this);
        if(dialog->exec() == QDialog::Accepted)
        {
            editKey->setName(dialog->getName());
            editKey->setType(dialog->getType());
            editKey->setMessage(dialog->getMessage());
            updateKeyList();
        }

        delete dialog;
    }
    else
        QMessageBox::warning(this, "Error", "Cannot edit key, no key selected.", QMessageBox::Ok);
}

void EditorWindow::on_removeKeyEventButton_clicked()
{
    Key* removeKey = getSelectedKey();

    if(removeKey != nullptr)
    {
        if(QMessageBox::question(this, "Removing Key Event", "Are you sure you wish to remove this key event from the mission?",
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if(!quest.mission.getItems()->RemoveKeyEvent(removeKey->getName()))
                QMessageBox::warning(this, "Error", "Could not remove key, not found in item collection!", QMessageBox::Ok);
            else
                updateKeyList();
        }
    }
    else
        QMessageBox::warning(this, "Error", "Cannot remove key, no key selected.", QMessageBox::Ok);
}

void EditorWindow::on_newGateButton_clicked()
{
    EditGateDialog* dialog = new EditGateDialog(quest.mission.getItems()->getKeyEventNameList(), this);
    if(dialog->exec() == QDialog::Accepted)
    {
        quest.mission.getItems()->AddGate(dialog->getName(),
                                          Gate(dialog->getName(), dialog->getType(), dialog->getKeys(), dialog->isTriggered()));
        updateGateList();
    }
    delete dialog;
}

void EditorWindow::on_editGateButton_clicked()
{
    QVariant selected = ui->gateList->currentIndex().data();
    if(!selected.isNull())
    {
        Gate* selectedGate = quest.mission.getItems()->getGate(selected.toString());
        if(selectedGate != nullptr)
        {
            EditGateDialog* dialog = new EditGateDialog(selectedGate, quest.mission.getItems()->getKeyEventNameList(), this);
            if(dialog->exec() == QDialog::Accepted)
            {
                selectedGate->setKeys(dialog->getKeys());
                selectedGate->setName(dialog->getName());
                selectedGate->setTriggered(dialog->isTriggered());
                selectedGate->setType(dialog->getType());

                updateGateList();
            }
            delete dialog;
        }
        else
            QMessageBox::warning(this, "Error", "Cannot edit gate, not found in list.", QMessageBox::Ok);
    }
    else
        QMessageBox::warning(this, "Error", "Cannot edit gate, no gate selected.", QMessageBox::Ok);


}

void EditorWindow::on_removeGateButton_clicked()
{
    Gate* removeGate = getSelectedGate();

    if(removeGate != nullptr)
    {
        if(QMessageBox::question(this, "Removing Gate", "Are you sure you wish to remove this gate from the mission?",
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if(!quest.mission.getItems()->RemoveGate(removeGate->getName()))
                QMessageBox::warning(this, "Error", "Could not remove gate, not found in item collection!", QMessageBox::Ok);
            else
                updateGateList();
        }
    }
    else
        QMessageBox::warning(this, "Error", "Cannot remove gate, no gate selected.", QMessageBox::Ok);
}

/* ------------------------------------------------------------------
 *  HELPER FUNCTIONS
 * ------------------------------------------------------------------*/
void EditorWindow::clearRunningGame()
{
    if(runningGame)
    {
        if(runningGame->isOpen())
            runningGame->close();
        delete runningGame;
    }

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

void EditorWindow::initQuestUI()
{
    // Initialize and clear models
    if(!keyEventModel)
        keyEventModel = new QStringListModel;
    else
        keyEventModel->setStringList(QStringList());

    if(!gateModel)
        gateModel = new QStringListModel;
    else
        gateModel->setStringList(QStringList());

    // Initialize data
    keyData = QStringList();
    gateData = QStringList();

    updateKeyList();
    updateGateList();

    // Set view models
    ui->keyEventList->setModel(keyEventModel);
    ui->gateList->setModel(gateModel);

    // Disable edit triggers for views
    ui->keyEventList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->gateList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void EditorWindow::updateKeyList()
{
    keyData.clear();
    QList<Key*> keys = quest.mission.getItems()->getKeyEventList();
    for(Key* key : keys)
        keyData.append(key->getName());
    keyEventModel->setStringList(keyData);

    quest.mission.getItems()->Build(quest.getData(DAT_MISSION_ITEMS));
}

void EditorWindow::updateGateList()
{
    gateData.clear();
    QList<Gate*> gates = quest.mission.getItems()->getGateList();
    for(Gate* gate : gates)
        gateData.append(gate->getName());
    gateModel->setStringList(gateData);

    quest.mission.getItems()->Build(quest.getData(DAT_MISSION_ITEMS));
}


Key* EditorWindow::getSelectedKey()
{
    QVariant selectedKeyID = ui->keyEventList->currentIndex().data();
    QString keyName = selectedKeyID.toString();
    return quest.mission.getItems()->getKeyEvent(keyName);
}

Gate* EditorWindow::getSelectedGate()
{
    QVariant selectedGateID = ui->keyEventList->currentIndex().data();
    QString gateName = selectedGateID.toString();
    return quest.mission.getItems()->getGate(gateName);
}
