#ifndef APPLICATIONDISPATCHER_H
#define APPLICATIONDISPATCHER_H

#include <QProcess>

#include <QDebug>

enum class OS {
    Win32,
    Linux
};

#ifdef Q_OS_WIN32
    const OS CURRENT_OS = OS::Win32;
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

    /**
     * @brief Run Runs the given executable.
     * @param executablePath Path of the executable.
     * @return Returns the running QProcess, calling code takes ownership of the resource.
     */
    static QProcess* Run(QString executablePath);

    /**
     * @brief RunThroughTerminal Runs the given executable path through a terminal or console. Calling code takes
     *                           ownership of the returned process
     * @param parent             The parent window.
     * @param executable         The executable path.
     * @param args               The arguments to pass to the program.
     * @param hold               Whether or not to 'hold' the terminal (does not function in Windows).
     * @return                   Returns the running process, if the process fails to start returns a null pointer.
     */
    static QProcess* RunThroughTerminal(QObject* parent, QString executable, QStringList args, bool hold = false);

private:
    ApplicationDispatcher() { };
};

#endif // APPLICATIONDISPATCHER_H
