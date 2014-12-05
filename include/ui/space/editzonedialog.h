#ifndef EDITZONEDIALOG_H
#define EDITZONEDIALOG_H

#include <QMessageBox>
#include <QDialog>

#include "zone.h"

namespace Ui {
class EditZoneDialog;
}

class EditZoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditZoneDialog(QList<Tileset*> tilesets, QWidget *parent = 0);
    explicit EditZoneDialog(QList<Tileset*> tilesets, Zone zone, QWidget *parent = 0);
    ~EditZoneDialog();

    inline QString getTileset() { return tileset; }
    inline QString getName() { return name; }
    inline int getAreaCount() { return areaCount; }

private slots:
    void on_OKButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::EditZoneDialog *ui;
    QString tileset;
    QString name;
    int areaCount;
};

#endif // EDITZONEDIALOG_H
