#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include <string>

#include "data/server.h"
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
    CommandLineController();
    qint64 PID;
    bool HasJava();
    int Getos();//0 win 1 mac 2 linux
    QProcess* ThisProcess;
private:
    void GenerateCMDFile(int id,Server Server,string filename,string& filenameresult);
    void ASyncCommandLine(Server* Server,int id);

};

#endif // COMMANDLINE_H
