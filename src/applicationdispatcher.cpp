#include "applicationdispatcher.h"

QProcess* ApplicationDispatcher::Run(QString executablePath)
{
    QProcess* process = new QProcess();

    process->start(executablePath);

    return process;
}

QProcess* ApplicationDispatcher::RunThroughTerminal(QObject* parent, QString executable, QStringList args, bool hold)
{
    QProcess* process = new QProcess(parent);

    switch(CURRENT_OS)
    {
    case OS::Linux:
        args.insert(args.begin(), executable);
        args.insert(args.begin(), "-e");
        if(hold)
            args.insert(args.begin(), "-hold");
        process->start("xterm", args);

        if(!process->waitForStarted(20000))
        {
            delete process;
            process = nullptr;
        }
        break;

    case OS::Win32:
        process->start("cmd");

        // Once started,
        if(process->waitForStarted(20000))
        {
            process->write(QString(executable + " " + args.join(" ") + "\r\n").toUtf8());
        }
        else
        {
            delete process;
            process = nullptr;
        }
        break;
    }

    return process;
}
