#include "editorwindow.h"
#include "ui_editorwindow.h"
#include "common.h"
#include <QDir>

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

/*!
 * \brief Test button to build and then run a game.
 */
void EditorWindow::on_build_run_clicked()
{
    // Construct the folder hierarchy required
    QDir dir(appDir);

    if(!QDir(appDir + "/build").exists())
        dir.mkdir(appDir + "/build");

    // Output data files

    // Run Solarus with the generated quest
}
