#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include <string>

#include "../data/server.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <QProcess>

using namespace std;

class CommandLineController
{
public:
    CommandLineController();
    QString stdoutfilepath = "";
    bool isrunning = false;

    bool HasJava();
    void kill();
    void RunCloudflared(QString port);
    void RunServer(Server *Server);
    QString Command(QStringList command);

private:
    QProcess *ThisProcess;
    void GenerateCMDFile(int id, Server Server, string filename, string &filenameresult);
};

#endif // COMMANDLINE_H
