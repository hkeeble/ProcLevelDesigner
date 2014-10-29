#include "newquestdialog.h"
#include "ui_newquestdialog.h"

NewQuestDialog::NewQuestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewQuestDialog)
{
    ui->setupUi(this);
}

NewQuestDialog::~NewQuestDialog()
{
    delete ui;
}

void NewQuestDialog::on_toolButton_clicked()
{
    ui->qlocationEdit->setText(QFileDialog::getExistingDirectory(this, "Choose a Folder"));
}

void NewQuestDialog::on_okButton_clicked()
{
    if(ui->qlocationEdit->text().length() != 0 != 0 && ui->qnameEdit->text().length() != 0)
    {
        // Grab the text from the UI elements
        questName = ui->qnameEdit->text();
        folderPath = ui->qlocationEdit->text();

        accept();
    }
    else
        QMessageBox::warning(this, "Error", "Please enter a valid quest name and path.", QMessageBox::Ok);
}

void NewQuestDialog::on_cancelButton_clicked()
{
    reject();
}

QString NewQuestDialog::getQuestName() const
{
    return questName;
}

QString NewQuestDialog::getFolderPath() const
{
    return folderPath;
}
