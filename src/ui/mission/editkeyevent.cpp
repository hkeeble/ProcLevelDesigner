#include "editkeyevent.h"
#include "ui_editkeyevent.h"

EditKeyEvent::EditKeyEvent(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditKeyEvent)
{
    ui->setupUi(this);

    for(int i = 0; i < Key::Type::COUNT; i++)
        ui->comboBoxType->addItem(KEY_TYPE_STRINGS[i]);
}

EditKeyEvent::EditKeyEvent(Key* key, QWidget *parent) :
    EditKeyEvent(parent)
{
    ui->nameEdit->setText(key->getName());
    ui->comboBoxType->setCurrentIndex(key->getKeyType());
    ui->messageEdit->setPlainText(key->getMessage());
}

EditKeyEvent::~EditKeyEvent()
{
    delete ui;
}

void EditKeyEvent::on_okButton_clicked()
{
    QRegularExpression reg = QRegularExpression("^[A-Za-z0-9]+$");
    if(ui->nameEdit->text().length() == 0 || !reg.match(ui->nameEdit->text()).hasMatch())
        QMessageBox::warning(this, "Error", "Please enter a valid event name. Must be more than 0 characters, and contain only letters and numbers", QMessageBox::Ok);
    else
    {
        name = ui->nameEdit->text();
        message = ui->messageEdit->toPlainText();
        type = static_cast<Key::Type>(ui->comboBoxType->currentIndex());
        accept();
    }
}

void EditKeyEvent::on_cancelButton_clicked()
{
    reject();
}
