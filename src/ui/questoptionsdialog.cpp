#include "questoptionsdialog.h"
#include "ui_questoptionsdialog.h"

QuestOptionsDialog::QuestOptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestOptionsDialog)
{
    ui->setupUi(this);
}

QuestOptionsDialog::~QuestOptionsDialog()
{
    delete ui;
}

void QuestOptionsDialog::on_okButton_clicked()
{
    name = ui->nameEdit->text();

    if(name > 0)
        accept();
    else
        QMessageBox::warning(this, "Error", "Please enter a valid quest name.", QMessageBox::Ok);
}

void QuestOptionsDialog::on_cancelButton_clicked()
{
    reject();
}

QString QuestOptionsDialog::getName() const
{
    return name;
}
