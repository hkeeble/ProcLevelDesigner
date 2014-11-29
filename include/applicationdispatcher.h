#ifndef APPLICATIONDISPATCHER_H
#define APPLICATIONDISPATCHER_H

#include <QProcess>

#include <QDebug>

enum class OS {
    Win32,
    Linux
};

#ifdef Q_OS_WIN32
    const OS CURRENT_OS = OS::Win32
#endif // Q_OS_WIN32
#ifdef Q_OS_LINUX
    const OS CURRENT_OS = OS::Linux;
#endif // Q_OS_LINUX

/*!
 * \brief Used to execute external applications through the program.
 */
class ApplicationDispatcher
{
public:
    static QProcess* Run(QString executablePath);
    static QProcess* RunThroughTerminal(QObject* parent, QString executable, QStringList args = QStringList(), bool hold = false);
private:
    ApplicationDispatcher() { };
};

#endif // APPLICATIONDISPATCHER_H
