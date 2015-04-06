#ifndef EDITKEYEVENT_H
#define EDITKEYEVENT_H

#include <QDialog>
#include <QMessageBox>

#include "key.h"

namespace Ui {
class EditKeyEvent;
}

class EditKeyEvent : public QDialog
{
    Q_OBJECT

public:
    explicit EditKeyEvent(QList<Key*> existingKeys, QWidget *parent = 0);
    explicit EditKeyEvent(Key* key, QList<Key*> existingKeys, QWidget *parent = 0);
    ~EditKeyEvent();

    inline Key::Type getType()  { return type; }
    inline QString getName()    { return name; }
    inline QString getMessage() { return message; }

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Key* givenKey; /* The key the window was given if applicable. */
    QList<Key*> existingKeys; /* The existing keys. */

    Ui::EditKeyEvent *ui;

    Key::Type type;
    QString name;
    QString message;
};

#endif // EDITKEYEVENT_H
