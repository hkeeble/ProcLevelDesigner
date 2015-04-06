#include "editkeyevent.h"
#include "ui_editkeyevent.h"

EditKeyEvent::EditKeyEvent(QList<Key*> existingKeys, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditKeyEvent)
{
    ui->setupUi(this);

    for(int i = 0; i < Key::Type::COUNT; i++)
        ui->comboBoxType->addItem(KEY_TYPE_STRINGS[i]);

    this->existingKeys = existingKeys;

    givenKey = nullptr;
}

EditKeyEvent::EditKeyEvent(Key* key, QList<Key*> existingKeys, QWidget *parent) :
    EditKeyEvent(existingKeys, parent)
{
    ui->nameEdit->setText(key->getName());
    ui->comboBoxType->setCurrentIndex(key->getKeyType());
    ui->messageEdit->setPlainText(key->getMessage());

    givenKey = key;
}

EditKeyEvent::~EditKeyEvent()
{
    delete ui;
}

void EditKeyEvent::on_okButton_clicked()
{
    QRegularExpression reg = QRegularExpression("^[A-Za-z0-9]+$");
    if(ui->nameEdit->text().length() == 0 || !reg.match(ui->nameEdit->text()).hasMatch())
    {
        QMessageBox::warning(this, "Error", "Please enter a valid event name. Must be more than 0 characters, and contain only letters and numbers",
                             QMessageBox::Ok);
        return;
    }

    if((givenKey != nullptr ? ui->nameEdit->text() != givenKey->getName() : true))
    {
        for(Key* key : existingKeys)
        {
            if(key->getName() == ui->nameEdit->text())
            {
                QMessageBox::warning(this, "Error", "This key name already exists, please enter a unique name.",
                                     QMessageBox::Ok);
                return;
            }
        }
    }

    name = ui->nameEdit->text();
    message = ui->messageEdit->toPlainText();
    type = static_cast<Key::Type>(ui->comboBoxType->currentIndex());
    accept();
}

void EditKeyEvent::on_cancelButton_clicked()
{
    reject();
}
