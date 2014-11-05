#ifndef QUEST_OPTIONS_DIALOG_H
#define QUEST_OPTIONS_DIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "quest.h"

namespace Ui {
class QuestOptionsDialog;
}

class QuestOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuestOptionsDialog(QWidget *parent = 0);
    ~QuestOptionsDialog();

    QString getName() const;

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::QuestOptionsDialog *ui;
    QString name;
};

#endif // QUEST_OPTIONS_DIALOG_H
