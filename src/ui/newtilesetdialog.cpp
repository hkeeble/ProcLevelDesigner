#include "newtilesetdialog.h"
#include "ui_newtilesetdialog.h"

NewTilesetDialog::NewTilesetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTilesetDialog)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
}

NewTilesetDialog::~NewTilesetDialog()
{
    delete ui;
    if(scene)
        delete scene;
}

void NewTilesetDialog::on_buttonOK_clicked()
{
    if(ui->nameEdit->text().length() != 0 && ui->fileEdit->text().length() != 0)
    {
        name = ui->nameEdit->text();
        filePath = ui->fileEdit->text();
        tileSize = ui->tilesizeEdit->value();
        accept();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Please enter a valid file path and tileset name.", QMessageBox::Ok);
    }
}

void NewTilesetDialog::on_buttonCancel_clicked()
{
    reject();
}

void NewTilesetDialog::on_buttonBrowse_clicked()
{
    ui->fileEdit->setText(QFileDialog::getOpenFileName(this, "Open Image", QString(), "Image Files (*.png)"));
    QFileInfo fileLoaded = QFileInfo(ui->fileEdit->text());
    QString path = fileLoaded.absoluteFilePath();
    tileset = QPixmap(path);

    scene->clear();
    scene->setSceneRect(QRect(0, 0, tileset.width(), tileset.height()));
    scene->addPixmap(tileset);

    ui->tileSetView->setScene(scene);
}
