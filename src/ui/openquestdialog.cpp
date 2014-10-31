#include "openquestdialog.h"
#include "ui_openquestdialog.h"

OpenQuestDialog::OpenQuestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenQuestDialog)
{
    ui->setupUi(this);
}

OpenQuestDialog::~OpenQuestDialog()
{
    delete ui;
}

void OpenQuestDialog::on_browseButton_clicked()
{
    ui->qlocationEdit->setText(QFileDialog::getExistingDirectory(this, "Choose a Folder"));
}

QString OpenQuestDialog::getFolderPath() const
{
    return folderPath;
}

void OpenQuestDialog::on_okButton_clicked()
{
    if(ui->qlocationEdit->text().length() != 0)
    {
        folderPath = ui->qlocationEdit->text();
        accept();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Please enter a valid file path.", QMessageBox::Ok);
    }
}

void OpenQuestDialog::on_cancelButton_clicked()
{
    reject();
}
