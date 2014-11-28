#ifndef EDITKEYEVENT_H
#define EDITKEYEVENT_H

#include <QDialog>

namespace Ui {
class EditKeyEvent;
}

class EditKeyEvent : public QDialog
{
    Q_OBJECT

public:
    explicit EditKeyEvent(QWidget *parent = 0);
    explicit EditKeyEvent(QString name, QWidget *parent = 0);
    ~EditKeyEvent();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::EditKeyEvent *ui;

    QString name;
};

#endif // EDITKEYEVENT_H
