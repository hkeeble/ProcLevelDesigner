#include "editorwindow.h"
#include "common.h"
#include <QApplication>

QString appDir;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    appDir = a.applicationDirPath();
    EditorWindow w;
    w.showMaximized();

    return a.exec();
}
