#include "questdatabase.h"
#include "ui_questdatabase.h"

QuestDatabase::QuestDatabase(Quest* quest, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestDatabase)
{
    ui->setupUi(this);

    setMouseTracking(true);

    this->quest = quest;

    setWindowTitle("Quest Database - " + quest->getName());

    initInfoTab();
    initTilesetTab();

}

void QuestDatabase::initInfoTab()
{
    Table* qData = quest->getData(DAT_QUEST);
    ui->questNameEdit->setText(qData->getElementValue(OBJ_QUEST, ELE_NAME));
}

void QuestDatabase::initTilesetTab()
{
    selectedTileset = nullptr;
    openTileSets = QList<Tileset*>();
    tilesetScene = new TilesetView(this);
    tilesetModel = new QStandardItemModel();

    // UI Initialization
    ui->tilesetsList->setModel(tilesetModel);
    ui->tilesetsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tilesetSelectedView->setMouseTracking(true);
    ui->tilesetSelectedView->setScene(tilesetScene);

    // Update the model (initialize it)
    updateTilesetModel();

}

QuestDatabase::~QuestDatabase()
{
    delete ui;

    if(tilesetModel)
        delete tilesetModel;
    if(tilesetScene)
        delete tilesetScene;
}

void QuestDatabase::on_tilesetsList_doubleClicked(const QModelIndex &index)
{
    selectedTileset = quest->getTileset(index.data().toString());

    if(selectedTileset)
    {
        if(!openTileSets.contains(selectedTileset))
            openTileSets.append(selectedTileset);

        ui->setLabel->setEnabled(true);
        ui->setLabel->setText("Current Tileset: " + index.data().toString());

        tilesetScene->setTileset(selectedTileset);
    }
    else
    {
        QMessageBox::warning(this, "Error", "The selected tileset is no longer part of this quest. Removing from list.", QMessageBox::Ok);
        tilesetModel->removeRow(index.row());
    }
}

void QuestDatabase::on_addTilesetButton_clicked()
{
    NewTilesetDialog* dialog = new NewTilesetDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        // Copy the image file into tilesets directory (create a dummy entity set for now)
        QFile::copy(dialog->getFilePath(), quest->getRootDir().absolutePath() + QDir::separator() + "tilesets" +
            QDir::separator() + dialog->getName() + ".tiles.png");
        QFile::copy(dialog->getFilePath(), quest->getRootDir().absolutePath() + QDir::separator() + "tilesets" +
            QDir::separator() + dialog->getName() + ".entities.png");

        // Create a new .dat file
        Table* data = quest->getData(QString("tilesets") + QDir::separator() + dialog->getName());
        Tileset set = Tileset::create(dialog->getName(), dialog->getFilePath(), data, dialog->getTileSize());
        quest->addTileSet(set);
        updateTilesetModel();
    }

    delete dialog;
}

void QuestDatabase::updateTilesetModel()
{
    // Consruct list view for tile sets
    QList<Tileset*> list = quest->getTilesetList();
    tilesetModel->clear();

    tilesetModel->setRowCount(list.length());
    for(int i = 0; i < list.length(); i++)
        tilesetModel->setItem(i, new QStandardItem(list[i]->getName()));
}

void QuestDatabase::on_removeTilesetButton_clicked()
{
    if(QMessageBox::warning(this, "Warning", "Removing tileset " + selectedTileset->getName() +
                         " will delete all local .dat and image files. Are you sure you wish to do this?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        Tileset* set = quest->getTileset(selectedTileset->getName());
        openTileSets.removeOne(set);
        quest->removeTileset(set->getName());

        updateTilesetModel();
    }
}

void QuestDatabase::on_OKButton_clicked()
{
    // Validate all inputs here
    if(validate())
    {
        // Save modified quest info
        Table* qData = quest->getData(DAT_QUEST);
        qData->setElementValue(OBJ_QUEST, ELE_NAME, ui->questNameEdit->text());
        qData->saveToDisk();

        // Save all opened tilesets (assumption is that all have been modified)
        for(Tileset* set : openTileSets)
            Tileset::build(*set);

        accept();
    }
    else
        QMessageBox::warning(this, "Error", "Please ensure all quest information is valid before continuing.", QMessageBox::Ok);
}

void QuestDatabase::on_questNameEdit_editingFinished()
{
    ui->questNameEdit->setText(ui->questNameEdit->text().replace(' ', '_'));
}

void QuestDatabase::closeEvent(QCloseEvent* event)
{
    ui->OKButton->click();

    if(validate())
        reject();
    else
        event->ignore();
}

bool QuestDatabase::validate()
{
    return ui->questNameEdit->text().length() != 0;
}
