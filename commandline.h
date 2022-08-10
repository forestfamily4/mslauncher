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
    qint64 PID;
    bool HasJava();
    int Getos();//0 win 1 mac 2 linux
private:
    void GenerateCMDFile(int id,Server Server,string filename,string& filenameresult);
    void ASyncCommandLine(Server* Server,int id);
    QProcess* ThisProcess;

};

#endif // COMMANDLINE_H
