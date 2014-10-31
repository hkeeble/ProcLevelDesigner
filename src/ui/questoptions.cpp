#include "questoptions.h"
#include "ui_questoptions.h"

QuestOptions::QuestOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestOptions)
{
    ui->setupUi(this);
}

QuestOptions::~QuestOptions()
{
    delete ui;
}

void QuestOptions::on_okButton_clicked()
{
    name = ui->nameEdit->text();

    if(name > 0)
        accept();
    else
        QMessageBox::warning(this, "Error", "Please enter a valid quest name.", QMessageBox::Ok);
}

void QuestOptions::on_cancelButton_clicked()
{
    reject();
}
