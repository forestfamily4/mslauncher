#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include <string>

#include "server.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <QPromise>
#include <QFuture>
#include <QProcess>

using namespace std;

class CommandLineController
{
public:
    CommandLineController(Server* Server,int id);
    void EnterCommand(string Command);
    qint64 PID;
    bool HasJava();
    QProcess* ThisProcess;
    void GenerateCMDFile(int id,Server Server);
    void SyncCommandLine(Server* Server,int id);
};

#endif // COMMANDLINE_H
