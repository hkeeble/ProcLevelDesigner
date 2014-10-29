#ifndef OPENQUESTDIALOG_H
#define OPENQUESTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

namespace Ui {
class OpenQuestDialog;
}

class OpenQuestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenQuestDialog(QWidget *parent = 0);
    ~OpenQuestDialog();

    QString getFolderPath() const;

private slots:
    void on_browseButton_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::OpenQuestDialog *ui;
    QString folderPath;
};

#endif // OPENQUESTDIALOG_H
