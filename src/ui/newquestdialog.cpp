#include "newquestdialog.h"
#include "ui_newquestdialog.h"

NewQuestDialog::NewQuestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewQuestDialog)
{
    ui->setupUi(this);

    ui->qnameEdit->setText("My Quest");
    ui->qlocationEdit->setText(QDir().currentPath() + QDir::separator() + "My Quests");
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
    if(ui->qlocationEdit->text().length() != 0 && ui->qnameEdit->text().length() != 0)
    {
        // Grab the text from the UI elements
        questName = ui->qnameEdit->text();
        folderPath = ui->qDestination->text();

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


void NewQuestDialog::updateDestination() const
{
    ui->qDestination->setText(ui->qlocationEdit->text() + QDir::separator() + ui->qnameEdit->text());
}

void NewQuestDialog::on_qnameEdit_textChanged(const QString &arg1)
{
    updateDestination();
}

void NewQuestDialog::on_qlocationEdit_textChanged(const QString &arg1)
{
    updateDestination();
}

void NewQuestDialog::on_qlocationEdit_editingFinished()
{
    QString text = ui->qlocationEdit->text();

    // Cut out any trailing separators
    if(text.at(text.length()-1) == QDir::separator())
        text.remove(text.length()-1, 1);

    ui->qlocationEdit->setText(text);
    updateDestination();
}
