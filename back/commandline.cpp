#include "commandline.h"
#include <string>
#include <iostream>
#include "data/server.h"
#include "stdio.h"
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
    if(Getos()==0){
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
        //GenerateCMDFile(0,*Server,"buildserver",CommandFile);
        isrunning=true;
        ThisProcess=new QProcess();
        ThisProcess->setWorkingDirectory(Server->Directory);
        ThisProcess->setStandardOutputFile(stdoutfilepath);
        ThisProcess->start("java",{"-jar","-Dfile.encoding=UTF-8","-Xmx1024M","-Xms1024M",Server->Directory+"/server.jar","nogui"});
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

void CommandLineController::kill(){
    if(isrunning){
        ThisProcess->kill();
    }

}

#ifdef Q_OS_WIN
int CommandLineController::Getos(){//0 win 1 mac 2 linux 3 None
    return 0;
}
#elif Q_OS_MACX
int CommandLineController::Getos(){//0 win 1 mac 2 linux 3 None
    return 1;
}
#elif Q_OS_LINUX
int CommandLineController::Getos(){//0 win 1 mac 2 linux 3 None
    return 2;
}
#else
int CommandLineController::Getos(){//0 win 1 mac 2 linux 3 None
    return 3;
}
#endif
