#ifndef NEWTILESETDIALOG_H
#define NEWTILESETDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsScene>

#include "tileset.h"

namespace Ui {
class NewTilesetDialog;
}

class NewTilesetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewTilesetDialog(QWidget *parent = 0);
    ~NewTilesetDialog();

    inline QString getName() { return name; }
    inline QString getFilePath() { return filePath; }
    inline int getTileSize() { return tileSize; }

private slots:
    void on_buttonOK_clicked();

    void on_buttonCancel_clicked();

    void on_buttonBrowse_clicked();

    void on_tileSetView_rubberBandChanged(const QRect &viewportRect, const QPointF &fromScenePoint, const QPointF &toScenePoint);

private:
    Ui::NewTilesetDialog *ui;

    QString name, filePath;
    int tileSize;

    QPixmap tileset; /*!< The image representing the tileset. */
    QGraphicsScene* scene; /*!< The graphics scene representing the tileset. */
};

#endif // NEWTILESETDIALOG_H
