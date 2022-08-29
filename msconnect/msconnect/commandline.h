#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include <string>

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
    void cloudflared(QString hosturl,QString userurl );
    QString Command(QStringList command);
private:
    QProcess* ThisProcess;
};

#endif // COMMANDLINE_H
