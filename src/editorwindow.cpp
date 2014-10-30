#include "editorwindow.h"
#include "ui_editorwindow.h"


EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::build()
{
    // Build maps
}

void EditorWindow::run()
{
    // Run Solarus
}

void EditorWindow::on_actionOpen_Quest_triggered()
{
    OpenQuestDialog* dialog = new OpenQuestDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        quest = Quest(dialog->getFolderPath());

        if(quest.Init())
        {
            populateTreeView(quest.getFSModel(), dialog->getFolderPath());
        }
        else
        {
             QMessageBox::warning(this, "Error", "No valid quest was found in this folder.\nQuests must contain the following files at a minimum:\n\n\
quest.dat\nproject_db.dat\nmain.lua", QMessageBox::Ok);
        }
    }
}

void EditorWindow::on_actionNew_Quest_triggered()
{
    NewQuestDialog* dialog = new NewQuestDialog(this);
    if(dialog->exec() == QDialog::Accepted)
    {
        quest = Quest(dialog->getFolderPath());
    }
}

void EditorWindow::on_actionExit_triggered()
{
    this->close();
}

 void EditorWindow::populateTreeView(QFileSystemModel* model, QString rootDir)
 {
     ui->treeView->setModel(model);
     ui->treeView->setRootIndex(quest.getFSModel()->index(rootDir));
 }

void EditorWindow::on_testParse_clicked()
{
    Table table("game_data/maps/first_map.dat");

    qDebug(table.getElementValue("tile", "pattern").toStdString().c_str());
}
