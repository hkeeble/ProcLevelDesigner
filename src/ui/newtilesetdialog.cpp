#include "newtilesetdialog.h"
#include "ui_newtilesetdialog.h"

NewTilesetDialog::NewTilesetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTilesetDialog)
{
    ui->setupUi(this);
    scene = nullptr;
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

    tileset = QPixmap(ui->fileEdit->text());

    if(scene)
        delete scene;
    scene = new QGraphicsScene(QRect(0,0,ui->tileSetView->width(), ui->tileSetView->height()));
    scene->addPixmap(tileset);

    ui->tileSetView->setScene(scene);
}
