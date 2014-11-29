#include "applicationdispatcher.h"

QProcess* ApplicationDispatcher::Run(QString executablePath)
{
    QProcess* proc = new QProcess();

    proc->start(executablePath);

    return proc;
}

QProcess* ApplicationDispatcher::RunThroughTerminal(QObject* parent, QString executable, QStringList args, bool hold)
{
    QProcess* proc = new QProcess(parent);

    args.insert(args.begin(), executable);
    args.insert(args.begin(), "-e");
    if(hold)
        args.insert(args.begin(), "-hold");

    if(CURRENT_OS == OS::Linux)
        proc->start("xterm", args);

    return proc;
}
