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
    ui->tilesetSelectedView->setMouseTracking(true);
    ui->tilesetSelectedView->setScene(tilesetScene);

    // Consruct list view for tile sets
    QList<Tileset*> list = quest->getTilesetList();
    tilesetModel = new QStandardItemModel();

    for(Tileset* set : list)
    {
        tilesetModel->appendRow(new QStandardItem(set->getName()));
    }

    ui->tilesetsList->setModel(tilesetModel);
    ui->tilesetsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

QuestDatabase::~QuestDatabase()
{
    delete ui;

    for(int i = 0; i < tileSetList.length(); i++)
    {
        if(tileSetList[i])
            delete tileSetList[i];
    }
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
        addTileset(set);
        openTileSets.append(quest->getTileset(set.getName()));
    }

    delete dialog;
}

void QuestDatabase::addTileset(Tileset tileSet)
{
    tileSetList.append(new QStandardItem(tileSet.getName()));
    tilesetModel->appendRow(tileSetList.last());
}

void QuestDatabase::on_removeTilesetButton_clicked()
{

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
