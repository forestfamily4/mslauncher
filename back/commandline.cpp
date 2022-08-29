#include "commandline.h"
#include <string>
#include <iostream>
#include "data/server.h"
#include "os.h"
#include <stdlib.h>
#include <QProcess>
#include <QDir>
#include <fstream>
#include "java.h"
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
        ThisProcess->setStandardOutputFile(stdoutfilepath);
        ThisProcess->setWorkingDirectory(Server->Directory);
        ThisProcess->start(java::javapath(),{"-jar","-Xmx1024M","-Xms1024M",Server->ServerJARFile,"nogui"});
    }
    else if(id==1){
        //macで動くかは知らぬ
        isrunning=true;
        ThisProcess=new QProcess();
        ThisProcess->setStandardErrorFile(stdoutfilepath);
        ThisProcess->start("cloudflared",{"tunnel","--url", "tcp://localhost:"+port});
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


void CommandLineController::kill(){
    if(isrunning){
        ThisProcess->kill();
    }

}
