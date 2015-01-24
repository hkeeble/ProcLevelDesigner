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
    zoneModel = nullptr;
    runningGame = nullptr;

    // Enable key and gate drag and drop
    ui->keyEventList->setDragEnabled(true);
    ui->gateList->setDragEnabled(true);

    // Enable the structure view to accept drops
    ui->structureView->viewport()->setAcceptDrops(true);

    setWindowTitle("ProcLevelDesigner");

    // Parse preferences file (save in case a new one was created)
    Table* prefTable = new Table(DAT_PREFERENCES);
    preferences = Preferences::Parse(prefTable);
    prefTable->saveToDisk();

    preferences.setAsRecentQuestManager(this, ui->menuOpen_Recent, SLOT(on_actionOpen_Recent_Quest_triggered(QAction*)));

    missionStructureScene = new MissionStructureScene();
    ui->structureView->setScene(missionStructureScene);

    spaceScene = new SpaceScene();
    ui->spaceView->setScene(spaceScene);
}

EditorWindow::~EditorWindow()
{
    if(keyEventModel)
        delete keyEventModel;
    if(gateModel)
        delete gateModel;
    if(zoneModel)
        delete zoneModel;
    if(missionStructureScene)
        delete missionStructureScene;
    if(spaceScene)
        delete spaceScene;

    clearRunningGame();

    delete ui;
}

// Close Event, ensures changes can be saved
void EditorWindow::closeEvent(QCloseEvent *event)
{
    // Save preferences
    Table* prefTable = new Table(DAT_PREFERENCES);
    preferences.build(prefTable);
    prefTable->saveToDisk();

    // Build quest
    quest.build();

    // Check to see if built changes need to be saved to disk
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
    map.setTileSet(quest.getTileset("field"));

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
    writeToFile(QFileInfo(data->getFilePath()).absoluteDir().absolutePath(), "second_map.lua", ""); // Write map script file

    Table* database = quest.getData(DAT_DATABASE);
    database->addObject(OBJ_MAP, map.getDatabaseObject());
    quest.saveData();
}

void EditorWindow::on_actionOpen_Quest_triggered()
{
    ui->actionClose->trigger(); // Close the current quest

    OpenQuestDialog* dialog = new OpenQuestDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        openQuest(dialog->getFolderPath() + QDir::separator() + "data" + QDir::separator());
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
            createNewQuest(dialog->getQuestName(), dialog->getFolderPath() + QDir::separator() + "data" + QDir::separator());
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
    // Clear the running game
    clearRunningGame();

    // Build all maps
    quest.buildMaps();
    quest.saveMaps();

    // Run the game
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
    }

    delete dialog;
}

void EditorWindow::on_actionOpen_Recent_Quest_triggered(QAction* action)
{
    if(!openQuest(action->data().toString()))
    {
        QMessageBox::information(this, "Quest No Longer Exists", "This quest no longer exists at this location, removing it from recent list.", QMessageBox::Ok);
        preferences.removeRecentQuestPath(action->data().toString());
    }
}

/* ------------------------------------------------------------------
 *  KEY BUTTON ACTIONS
 * ------------------------------------------------------------------*/
void EditorWindow::on_newKeyEventButton_clicked()
{
    EditKeyEvent* dialog = new EditKeyEvent(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        quest.mission.addKeyEvent(dialog->getName(), Key(dialog->getName(), dialog->getType(), dialog->getMessage()));
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
        if(QMessageBox::question(this, "Removing Key Event", "Are you sure you wish to remove this key event from the mission? It will be removed from all gates requiring it.",
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if(!quest.mission.removeKeyEvent(removeKey->getName()))
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
    EditGateDialog* dialog = new EditGateDialog(quest.mission.getKeyEventNameList(), this);
    if(dialog->exec() == QDialog::Accepted)
    {
        QList<Key*> keys;
        for(QString keyName : dialog->getKeys())
            keys.append(quest.mission.getKeyEvent(keyName));

        quest.mission.addGate(dialog->getName(),
                                          Gate(dialog->getName(), dialog->getType(), keys, dialog->isTriggered()));
        updateGateList();
    }
    delete dialog;
}

void EditorWindow::on_editGateButton_clicked()
{
    QVariant selected = ui->gateList->currentIndex().data();
    if(!selected.isNull())
    {
        Gate* selectedGate = quest.mission.getGate(selected.toString());
        if(selectedGate != nullptr)
        {
            EditGateDialog* dialog = new EditGateDialog(selectedGate, quest.mission.getKeyEventNameList(), this);
            if(dialog->exec() == QDialog::Accepted)
            {
                QList<Key*> keys;
                for(QString keyName : dialog->getKeys())
                    keys.append(quest.mission.getKeyEvent(keyName));

                selectedGate->setKeys(keys);

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
            if(!quest.mission.removeGate(removeGate->getName()))
                QMessageBox::warning(this, "Error", "Could not remove gate, not found in item collection!", QMessageBox::Ok);
            else
                updateGateList();
        }
    }
    else
        QMessageBox::warning(this, "Error", "Cannot remove gate, no gate selected.", QMessageBox::Ok);
}

void EditorWindow::on_generateMissionButton_clicked()
{
    quest.mission.generate(); // Generate the mission here
}

void EditorWindow::on_newZoneButton_clicked()
{
    QList<Tileset*> tilesets = quest.getTilesetList();
    if(tilesets.count() > 0)
    {
        EditZoneDialog* dialog = new EditZoneDialog(quest.getTilesetList());
        if(dialog->exec() == QDialog::Accepted)
        {
            quest.space.addZone(dialog->getName(), Zone(dialog->getName(), dialog->getAreaCount(),
                                                        quest.getTileset(dialog->getTileset()), dialog->getColor()));
            updateZoneList();
        }
        delete dialog;
    }
    else
        QMessageBox::warning(this, "Warning", "You must have at least one tileset loaded to create a zone. Go to Tools > Quest Database to load one.",
                             QMessageBox::Ok);
}

void EditorWindow::on_editZoneButton_clicked()
{
    Zone* zone = getSelectedZone();

    if(zone != nullptr)
    {
        EditZoneDialog* dialog = new EditZoneDialog(quest.getTilesetList(), zone, this);
        if(dialog->exec() == QDialog::Accepted)
        {
            zone->setTileset(quest.getTileset(dialog->getTileset()));
            zone->setName(dialog->getName());
            zone->setAreaCount(dialog->getAreaCount());
            zone->setColor(dialog->getColor());
            updateZoneList();
        }
        delete dialog;
    }
    else
        QMessageBox::warning(this, "Error", "Cannot edit zone, no zone selected.", QMessageBox::Ok);
}

void EditorWindow::on_removeZoneButton_clicked()
{
    Zone* zone = getSelectedZone();

    if(zone != nullptr)
    {
        if(QMessageBox::warning(this, "Removing Zone", "Are you sure you wish to remove this zone from the quest?", QMessageBox::Yes | QMessageBox::No)
                == QMessageBox::Yes)
        {
            quest.space.removeZone(zone->getName());
            updateZoneList();
        }
    }
}

void EditorWindow::on_generateSpaceButton_clicked()
{
    quest.space.generate(quest.mission);
}

/* ------------------------------------------------------------------
 *  HELPER FUNCTIONS
 * ------------------------------------------------------------------*/
bool EditorWindow::openQuest(QString path)
{
    quest.clear();
    quest = Quest(path);

    if(quest.Init()) // Attempt to initialize quest from the given path
    {
        // Populate the trees and set window title
        setWindowTitle("ProcLevelDesigner - " + quest.getName());

        // Initialize the UI with quest data
        initQuestUI();

        // Enable all quest only UI elements
        setQuestOnlyUIEnabled(true);

        // Update recent quests opened
        preferences.addRecentQuestPath(quest.getRootDir().absolutePath());

        // Set the mission for the mission scene to monitor for updates
        missionStructureScene->setMission(&quest.mission);

        // Set the space for the space scene to monitor for updates
        spaceScene->setSpace(&quest.space);

        return true;
    }
    else
    {
        QMessageBox::warning(this, "Error", "No valid quest was found in this folder.", QMessageBox::Ok);
        return false;
    }
}

void EditorWindow::createNewQuest(QString name, QString folderPath)
{
    Quest newQuest = Quest(folderPath);

    copyFolder(QDir().currentPath() + QDir::separator() + "game_data" + QDir::separator(),
               folderPath);

    // Create paths
    QDir dir;
    dir.mkpath(folderPath + QDir::separator() + "tilesets");
    dir.mkpath(folderPath + QDir::separator() + "maps");
    dir.mkpath(folderPath + QDir::separator() + "maps");

    // Modify the quest object
    Table* questData = newQuest.getData(DAT_QUEST);
    questData->setElementValue(OBJ_QUEST, ELE_NAME, name);
    questData->setElementValue(OBJ_QUEST, ELE_WRT_DIR, name);
    questData->setElementValue(OBJ_QUEST, ELE_SOL_VERS, SOLARUS_VERSION);

    // Save out all modified quest data
    newQuest.saveData();

    // Open the newly created quest
    openQuest(folderPath);
}

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
        keyEventModel = new QStringListModel();
    else
        keyEventModel->setStringList(QStringList());

    if(!gateModel)
        gateModel = new QStringListModel();
    else
        gateModel->setStringList(QStringList());

    if(!zoneModel)
        zoneModel = new QStringListModel();
    else
        zoneModel->setStringList(QStringList());

    // Initialize data
    keyData = QStringList();
    gateData = QStringList();
    zoneData = QStringList();

    updateKeyList();
    updateGateList();
    updateZoneList();

    // Set view models
    ui->keyEventList->setModel(keyEventModel);
    ui->gateList->setModel(gateModel);
    ui->zoneList->setModel(zoneModel);

    // Disable edit triggers for views
    ui->keyEventList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->gateList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->zoneList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void EditorWindow::updateKeyList()
{
    keyData.clear();
    QList<Key*> keys = quest.mission.getKeyEventList();
    for(Key* key : keys)
        keyData.append(key->getName());
    keyEventModel->setStringList(keyData);
}

void EditorWindow::updateGateList()
{
    gateData.clear();
    QList<Gate*> gates = quest.mission.getGateList();
    for(Gate* gate : gates)
        gateData.append(gate->getName());
    gateModel->setStringList(gateData);
}

void EditorWindow::updateZoneList()
{
    zoneData.clear();
    QList<Zone*> zones = quest.space.getZoneList();
    for(Zone* zone : zones)
        zoneData.append(zone->getName());
    zoneModel->setStringList(zoneData);
}

Key* EditorWindow::getSelectedKey()
{
    QVariant selectedKeyID = ui->keyEventList->currentIndex().data();
    QString keyName = selectedKeyID.toString();
    return quest.mission.getKeyEvent(keyName);
}

Gate* EditorWindow::getSelectedGate()
{
    QVariant selectedGateID = ui->gateList->currentIndex().data();
    QString gateName = selectedGateID.toString();
    return quest.mission.getGate(gateName);
}

Zone* EditorWindow::getSelectedZone()
{
    QVariant selectedZoneID = ui->zoneList->currentIndex().data();
    QString zoneName = selectedZoneID.toString();
    return quest.space.getZone(zoneName);
}
