#ifndef NEWQUESTDIALOG_H
#define NEWQUESTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QCloseEvent>
#include <QMessageBox>
#include <QValidator>

namespace Ui {
class NewQuestDialog;
}

class NewQuestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewQuestDialog(QWidget *parent = 0);
    ~NewQuestDialog();

    /*!
     * \brief Retrieves the path the user has chosen to create the new quest within.
     * \return The folder path to create the quest at.
     */
    QString getFolderPath() const;
    QString getQuestName() const;


private slots:
    void on_toolButton_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked();

    void on_qnameEdit_textChanged(const QString &arg1);

    void on_qlocationEdit_textChanged(const QString &arg1);

    void on_qlocationEdit_editingFinished();

private:
    void updateDestination() const;

    Ui::NewQuestDialog *ui;

    QString questName;
    QString folderPath;
};

#endif // NEWQUESTDIALOG_H
