#include "questdatabase.h"
#include "ui_questdatabase.h"

QuestDatabase::QuestDatabase(Quest* quest, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestDatabase)
{
    ui->setupUi(this);

    setMouseTracking(true);

    applyEnabled = false;

    this->quest = quest;

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
    tilesetScene = new TilesetView(this);
    ui->tilesetSelectedView->setMouseTracking(true);
    tilesetScene->setSceneRect(QRect(0, 0, ui->tilesetSelectedView->width(), ui->tilesetSelectedView->height()));
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

void QuestDatabase::enableApply()
{
    applyEnabled = true;
    ui->applyButton->setEnabled(true);
}

void QuestDatabase::disableApply()
{
    applyEnabled = false;
    ui->applyButton->setEnabled(false);
}

void QuestDatabase::on_tilesetsList_doubleClicked(const QModelIndex &index)
{
    QString setName = index.data().toString();

    selectedTileset = quest->getTileset(setName);

    if(selectedTileset)
    {
        tilesetScene->addPixmap(selectedTileset->getImage());
        tilesetScene->setTileset(selectedTileset);
    }
    else
    {
        QMessageBox::warning(this, "Error", "The selected tileset is no longer part of this quest. Removing from list.", QMessageBox::Ok);
    }
}

void QuestDatabase::on_addTilesetButton_clicked()
{
    NewTilesetDialog* dialog = new NewTilesetDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        // Copy the image file into tilesets directory
        QFile::copy(dialog->getFilePath(), quest->getRootDir().absolutePath() + QDir::separator() + "tilesets" +
            QDir::separator() + dialog->getName() + ".tiles.png");

        // Create a new .dat file
        Table* data = quest->getData(QString("tilesets") + QDir::separator() + dialog->getName());
        Tileset set = Tileset::create(dialog->getName(), dialog->getFilePath(), data, dialog->getTileSize());
        quest->addTileSet(set);
        addTileset(set);
        enableApply();
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

void QuestDatabase::on_cancelButton_clicked()
{
    if(applyEnabled)
    {
        if(QMessageBox::warning(this, "Unsaved Changes", "You've made changes to this quest's data. Do you wish to save before exitting?",
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            ui->OKButton->clicked();
        }
        else
            reject();
    }
    else
        reject();
}

void QuestDatabase::on_OKButton_clicked()
{
    if(applyEnabled)
        ui->applyButton->click();

    accept();
}

void QuestDatabase::on_applyButton_clicked()
{
    Table* qData = quest->getData(DAT_QUEST);
    qData->setElementValue(OBJ_QUEST, ELE_NAME, ui->questNameEdit->text());
    quest->saveData();
    disableApply();
}

void QuestDatabase::on_questNameEdit_editingFinished()
{
    ui->questNameEdit->setText(ui->questNameEdit->text().replace(' ', '_'));
    enableApply();
}
