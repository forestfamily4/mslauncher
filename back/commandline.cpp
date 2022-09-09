#include "commandline.h"
#include <string>
#include <iostream>
#include "data/server.h"
#include "os.h"
#include <stdlib.h>
#include <QProcess>
#include <QDir>
#include <fstream>
#include "servertype/serverrun.h"
#include "java.h"
using namespace std;

CommandLineController::CommandLineController()
{
}
bool CommandLineController::HasJava()
{
    return true;
}

/**
 *id
 *0 java server 起動
 *1 cloudflare
 */

void CommandLineController::RunServer(Server *Server)
{
    //args= {"-jar", "-Xmx1024M", "-Xms1024M", "nogui"}
    //  if(Server->ServerType)
    QDateTime q;
    this->stdoutfilepath = QDir::currentPath() + "/temp/stdout" + QString::number(rand()) + q.currentDateTime().toString("hhmmss") + ".txt";
    this->isrunning = true;
    ThisProcess = new QProcess();
    ThisProcess->setStandardOutputFile(stdoutfilepath);
    ThisProcess->setWorkingDirectory(Server->Directory);
    ServerRun *s=new ServerRun();
    QStringList args= s->GetArgs(Server->ServerType,Server->Directory,Server->ServerJARFile);
    qDebug()<<args;
    ThisProcess->start(java::javapath(),args);
}

QString CommandLineController::Command(QStringList command)
{
    ThisProcess = new QProcess();

    if (os::Getos() == 0)
    {
        /*
        if(QDir(QDir::currentPath()+"/java/bin").exists()){
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            env.insert("PATH", env.value("Path") +";"+QDir::toNativeSeparators(QDir::currentPath())+"\\java\\bin");
            ThisProcess->setProcessEnvironment(env);
        }
        */
        QString a = command[0];

        command.remove(0);
        ThisProcess->start(a, command);
        ThisProcess->waitForFinished();

        return QString::fromUtf8(ThisProcess->readAllStandardError() + ThisProcess->readAllStandardOutput());
    }
    else
    {
        QString a = command[0];
        command.remove(0);
        ThisProcess->start(a, command);
        ThisProcess->waitForFinished();
        return QString::fromUtf8(ThisProcess->readAllStandardError() + ThisProcess->readAllStandardOutput());
    }
}

void CommandLineController::kill()
{
    if (isrunning)
    {
        ThisProcess->kill();
    }
}

void CommandLineController::RunCloudflared(QString port)
{
    QDateTime q;
    this->stdoutfilepath = QDir::currentPath() + "/temp/stdout" + QString::number(rand()) + q.currentDateTime().toString("hhmmss") + ".txt";
    ThisProcess = new QProcess();
    ThisProcess->setStandardErrorFile(stdoutfilepath);
    ThisProcess->setWorkingDirectory(QDir::currentPath());
    ThisProcess->start("cloudflared", {"tunnel", "--url", "tcp://localhost:" + port});
    this->isrunning = true;
}
