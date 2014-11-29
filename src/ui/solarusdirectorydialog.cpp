#include "solarusdirectorydialog.h"
#include "ui_solarusdirectorydialog.h"

SolarusDirectoryDialog::SolarusDirectoryDialog(QString currentPath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolarusDirectoryDialog)
{
    ui->setupUi(this);

    if(currentPath == "DEFAULT")
    {
        setPathEnabled(false);
        def = true;
        ui->environmentPATHButton->setChecked(true);
    }
    else
    {
        setPathEnabled(true);
        def = false;
        ui->customDirectoryButton->setChecked(true);
        ui->pathEdit->setText(currentPath);
    }
}

SolarusDirectoryDialog::~SolarusDirectoryDialog()
{
    delete ui;
}

void SolarusDirectoryDialog::on_cancelButton_clicked()
{
    reject();
}

void SolarusDirectoryDialog::on_OKButton_clicked()
{
    if(ui->environmentPATHButton->isChecked())
        def = true;
    else
        path = ui->pathEdit->text();

    accept();
}

void SolarusDirectoryDialog::on_environmentPATHButton_toggled(bool checked)
{
    if(checked)
        setPathEnabled(false);
}

void SolarusDirectoryDialog::on_customDirectoryButton_toggled(bool checked)
{
    if(checked)
        setPathEnabled(true);
}

void SolarusDirectoryDialog::setPathEnabled(bool enabled)
{
    ui->label->setEnabled(enabled);
    ui->pathEdit->setEnabled(enabled);
    ui->browseButton->setEnabled(enabled);

    if(enabled)
        def = false;
}

void SolarusDirectoryDialog::on_browseButton_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setFilter(QDir::Executable | QDir::Files);
    ui->pathEdit->setText(fileDialog.getOpenFileName(this));
}
