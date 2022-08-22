#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include <string>

#include "../data/server.h"
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
    CommandLineController();
    QString stdoutfilepath="";
    bool isrunning=false;


    bool HasJava();
    void kill();
    void Command(Server* Server,int id,QString port="");
    QString Command(QStringList command);
private:
    QProcess* ThisProcess;
    void GenerateCMDFile(int id,Server Server,string filename,string& filenameresult);
};

#endif // COMMANDLINE_H
