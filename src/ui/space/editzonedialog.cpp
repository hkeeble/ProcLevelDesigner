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
    QRegularExpression reg = QRegularExpression("^[A-Za-z0-9]+$");
    if(ui->nameEdit->text().length() == 0 || !reg.match(ui->nameEdit->text()).hasMatch())
        QMessageBox::warning(this, "Error", "Please enter a valid event name. Must be more than 0 characters, and contain only letters and numbers",
                             QMessageBox::Ok);
    else
    {
        name = ui->nameEdit->text();
        tileset = ui->tilesetComboBox->currentText();
        areaCount = ui->areaCountSpinBox->value();
        accept();
    }
}

void EditZoneDialog::on_cancelButton_clicked()
{
    reject();
}
