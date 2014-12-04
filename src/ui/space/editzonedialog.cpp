#include "editzonedialog.h"
#include "ui_editzonedialog.h"

EditZoneDialog::EditZoneDialog(QList<Tileset*> tilesets, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditZoneDialog)
{
    ui->setupUi(this);

    for(Tileset* tileset : tilesets)
        ui->tilesetComboBox->addItem(tileset->getName());
}


EditZoneDialog::EditZoneDialog(QList<Tileset*> tilesets, Zone zone, QWidget *parent)
    : EditZoneDialog(tilesets, parent)
{
    ui->nameEdit->setText(zone.getName());
    ui->areaCountSpinBox->setValue(zone.getAreaCount());

    for(int i = 0; i < ui->tilesetComboBox->count(); i++)
    {
        if(ui->tilesetComboBox->itemText(i) == zone.getTileset()->getName())
            ui->tilesetComboBox->setCurrentIndex(i);
    }
}

EditZoneDialog::~EditZoneDialog()
{
    delete ui;
}

void EditZoneDialog::on_OKButton_clicked()
{
    accept();
}

void EditZoneDialog::on_cancelButton_clicked()
{
    reject();
}
