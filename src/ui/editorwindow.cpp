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

        if(quest.Init()) // Attempt to initialize quest from the given path
        {
            // Populate the tree and set window title
            populateTreeView(quest.getFSModel(), dialog->getFolderPath());
            setWindowTitle(quest.getName());

            // Enable menu items
            ui->actionClose->setEnabled(true);
            ui->actionSave_Quest->setEnabled(true);
        }
        else
            QMessageBox::warning(this, "Error", "No valid quest was found in this folder.", QMessageBox::Ok);
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
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
}

void EditorWindow::on_actionSave_Quest_triggered()
{

}

void EditorWindow::on_actionClose_triggered()
{
    quest.getFSModel()->dumpObjectTree();
}
