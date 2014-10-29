#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <qstring.h>

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
    void on_build_run_clicked();

private:
    Ui::EditorWindow *ui;
    QString appDir;         /*!< The location of the application. */
};

#endif // EDITORWINDOW_H
