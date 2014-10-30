#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDir>
#include <QFileSystemModel>

#include "common.h"
#include "newquestdialog.h"
#include "openquestdialog.h"
#include "quest.h"
#include "filetools.h"

namespace Ui {
class EditorWindow;
}

/*! \brief Main Editor Window Class.
 *
 *  Contains all main program functionality.
  */
class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

private slots:
    void on_actionOpen_Quest_triggered();
    void on_actionExit_triggered();
    void on_actionNew_Quest_triggered();

    void on_testParse_clicked();

private:
    void build(); /*!< Builds and outputs data files. */
    void run(); /*!< Runs the currently output quest. */

    Ui::EditorWindow *ui;   /*!< Pointer to the window UI elements. */
    QString appDir;         /*!< The location of the application. */

    Quest quest; /*!< The currently loaded quest. */

    void populateTreeView(QFileSystemModel* model, QString rootDir);

};

#endif // EDITORWINDOW_H
