#ifndef EDITGATEDIALOG_H
#define EDITGATEDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QMessageBox>
#include <QListWidget>
#include <QList>
#include <QListWidgetItem>

#include "gate.h"

namespace Ui {
class EditGateDialog;
}

class EditGateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditGateDialog(QStringList keys, QWidget *parent = 0);
    explicit EditGateDialog(Gate* gate, QStringList keys, QWidget* parent = 0);
    ~EditGateDialog();

    QString getName()     { return name; }
    Gate::Type getType()  { return type; }
    QStringList getKeys() { return keys; }
    bool isTriggered()    { return triggered; }

private slots:
    void on_addKeyButton_clicked();
    void on_removeKeyButton_clicked();

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    QList<QListWidgetItem*> getAllItems(QListWidget* widget);
    void removeKeyFromList(QString key, QListWidget* widget);

    QString name;
    Gate::Type type;
    QStringList keys;
    bool triggered;

    Ui::EditGateDialog *ui;
};

#endif // EDITGATEDIALOG_H
