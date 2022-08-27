#include "commandline.h"
#include <string>
#include <iostream>
#include "data/server.h"
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
void CommandLineController::GenerateCMDFile(int id,Server Server,string filename,string& filenameresult){
    std::ofstream file;
    string content="";
    if(id==0){//BuildServer
        content+="cd "+Server.Directory.toStdString();
        content+="\n java -Xmx1024M -Xms1024M -jar server.jar nogui";
    }

    if(filename==""){
        return;
    }
    if(os::Getos()==0){
        filename+=".bat";
    }
    else{
        filename+=".sh";
    }
    filenameresult=filename;
    file.open(filename);
    file<< content<< endl;
    file.close();
}

/**
 *id
 *0 java server 起動
 *1 cloudflare
*/

void CommandLineController::Command(Server* Server,int id,QString port){
    string CommandFile;
    isrunning=true;
    QDateTime q;
    this->stdoutfilepath=QDir::currentPath()+"/temp/stdout"+QString::number(rand())+q.currentDateTime().toString("hhmmss") +".txt";
    if(id==0){
        isrunning=true;
        ThisProcess=new QProcess();
        ThisProcess->setWorkingDirectory(Server->Directory);
        ThisProcess->setStandardOutputFile(stdoutfilepath);
        ThisProcess->start("java",{"-jar","-Xmx1024M","-Xms1024M",Server->ServerJARFile,"nogui"});

    }
    else if(id==1){
        //macで動くかは知らぬ
        isrunning=true;
        ThisProcess=new QProcess();
        ThisProcess->setWorkingDirectory(QDir::currentPath());
        ThisProcess->setStandardErrorFile(stdoutfilepath);
        ThisProcess->start("cloudflared",{"tunnel","--url", "tcp://localhost:"+port});
    }
}

QString CommandLineController::Command(QStringList command){
    ThisProcess=new QProcess();
    ThisProcess->setWorkingDirectory(QDir::currentPath());
    if(os::Getos()==0){
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
        return "";
    }
}


void CommandLineController::kill(){
    if(isrunning){
        ThisProcess->kill();
    }

}
