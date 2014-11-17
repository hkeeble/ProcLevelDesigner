#ifndef QUESTDATABASE_H
#define QUESTDATABASE_H

#include <QDialog>
#include <QStandardItemModel>
#include <QGraphicsScene>
#include <QCloseEvent>

#include "tilesetview.h"
#include "newtilesetdialog.h"
#include "quest.h"

namespace Ui {
class QuestDatabase;
}

class QuestDatabase : public QDialog
{
    Q_OBJECT

public:
    explicit QuestDatabase(Quest* quest, QWidget *parent = 0);
    ~QuestDatabase();

private slots:
    void on_tilesetsList_doubleClicked(const QModelIndex &index);
    void on_addTilesetButton_clicked();
    void on_removeTilesetButton_clicked();
    void on_OKButton_clicked();
    void on_questNameEdit_editingFinished();

private:
    void closeEvent(QCloseEvent* event) override final;

    bool validate();

    void initInfoTab();
    void initTilesetTab();

    // Global
    Ui::QuestDatabase *ui;
    Quest* quest;

    // Tileset Tab
    /*!
     * \brief addTileset Adds the tileset to the list view.
     * \param tileSet The set to add.
     */
    void addTileset(Tileset tileSet);

    QStandardItemModel* tilesetModel;   /*!< Model used to represent current tilesets. */
    QList<QStandardItem*> tileSetList;  /*!< List used to contain all tile set items for use in the model. */
    QList<Tileset*> openTileSets;       /*!< The list of tile sets currently opened. */
    Tileset* selectedTileset;           /*!< The currently selected tileset in the tileset view. */
    TilesetView* tilesetScene;          /*!< Scene used to display the tileset currently selected. */
};

#endif // QUESTDATABASE_H
