#ifndef EDITZONEDIALOG_H
#define EDITZONEDIALOG_H

#include <QMessageBox>
#include <QDialog>
#include <QColorDialog>
#include <QDebug>

#include "zone.h"

namespace Ui {
class EditZoneDialog;
}

class EditZoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditZoneDialog(QList<Tileset*> tilesets, QWidget *parent = 0);
    explicit EditZoneDialog(QList<Tileset*> tilesets, Zone* zone, QWidget *parent = 0);
    ~EditZoneDialog();

    inline QString getTileset() { return tileset; }
    inline QString getName() { return name; }
    inline QColor getColor() { return color; }

private slots:
    void on_OKButton_clicked();
    void on_cancelButton_clicked();
    void on_pickColorButton_clicked();

private:
    void setSelectedColor(QColor newColor);

    Ui::EditZoneDialog *ui;

    QColor color;
    QString tileset;
    QString name;
};

#endif // EDITZONEDIALOG_H
