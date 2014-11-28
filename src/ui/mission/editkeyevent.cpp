#include "editkeyevent.h"
#include "ui_editkeyevent.h"

EditKeyEvent::EditKeyEvent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditKeyEvent)
{
    ui->setupUi(this);
}

EditKeyEvent::EditKeyEvent(QString name, QWidget *parent) :
    EditKeyEvent(parent)
{
    // Set parameters here for when editting
}

EditKeyEvent::~EditKeyEvent()
{
    delete ui;
}

void EditKeyEvent::on_okButton_clicked()
{
    accept();
}

void EditKeyEvent::on_cancelButton_clicked()
{
    reject();
}
