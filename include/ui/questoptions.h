#ifndef QUESTOPTIONS_H
#define QUESTOPTIONS_H

#include <QDialog>
#include <QMessageBox>

#include "quest.h"

namespace Ui {
class QuestOptions;
}

class QuestOptions : public QDialog
{
    Q_OBJECT

public:
    explicit QuestOptions(QWidget *parent = 0);
    ~QuestOptions();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::QuestOptions *ui;
    QString name;
};

#endif // QUESTOPTIONS_H
