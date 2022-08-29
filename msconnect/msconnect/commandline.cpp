#include "commandline.h"
#include <string>
#include <iostream>
#include "os.h"
#include <stdlib.h>
#include <QPromise>
#include <QFuture>
#include <QProcess>
#include <QDir>
#include <fstream>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
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

void CommandLineController::cloudflared(QString hosturl,QString userurl){
    string CommandFile;
    isrunning=true;
    QDateTime q;
    this->stdoutfilepath=QDir::currentPath()+"/temp/stdout"+QString::number(rand())+q.currentDateTime().toString("hhmmss") +".txt";

        isrunning=true;
        ThisProcess=new QProcess();
        ThisProcess->setStandardErrorFile(stdoutfilepath);
        ThisProcess->start("cloudflared",{"access","tcp", "--hostname",hosturl,"-url","tcp://"+userurl});
}

void CommandLineController::kill(){
    if(isrunning){
        ThisProcess->kill();
    }

}
QString CommandLineController::Command(QStringList command){
    ThisProcess=new QProcess();

    if(os::Getos()==0){
        /*
        if(QDir(QDir::currentPath()+"/java/bin").exists()){
            qDebug()<<"add env";
            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
            env.insert("PATH", env.value("Path") +";"+QDir::toNativeSeparators(QDir::currentPath())+"\\java\\bin");
            qDebug()<<env.value("PATH");
            ThisProcess->setProcessEnvironment(env);
        }
        */
        QString a=command[0];

        command.remove(0);
        ThisProcess->start(a,command);
        ThisProcess->waitForFinished();

        return QString::fromUtf8(ThisProcess->readAllStandardError()+ThisProcess->readAllStandardOutput());
    }
    else{
        QString a=command[0];
        command.remove(0);
        ThisProcess->start(a,command);
        return QString::fromUtf8(ThisProcess->readAllStandardError()+ThisProcess->readAllStandardOutput());
    }
}
