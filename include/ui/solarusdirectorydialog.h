#ifndef SOLARUSDIRECTORYDIALOG_H
#define SOLARUSDIRECTORYDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class SolarusDirectoryDialog;
}

class SolarusDirectoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolarusDirectoryDialog(QString currentPath, QWidget *parent = 0);
    ~SolarusDirectoryDialog();

    bool isDefault() { return def; }
    QString getPath() { return path; }

private slots:
    void on_cancelButton_clicked();
    void on_OKButton_clicked();

    void on_environmentPATHButton_toggled(bool checked);

    void on_customDirectoryButton_toggled(bool checked);

    void on_browseButton_clicked();

private:
    Ui::SolarusDirectoryDialog *ui;

    void setPathEnabled(bool enabled);

    bool def;
    QString path;
};

#endif // SOLARUSDIRECTORYDIALOG_H
