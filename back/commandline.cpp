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


CommandLineController::CommandLineController(Server* Server,int id)
{
    ASyncCommandLine(Server,id);
}
CommandLineController::CommandLineController(){

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

void CommandLineController::ASyncCommandLine(Server* Server,int id){
    string CommandFile;
    if(id==0){
        //GenerateCMDFile(0,*Server,"buildserver",CommandFile);
        isrunning=true;
        ThisProcess=new QProcess();
        ThisProcess->setWorkingDirectory(Server->Directory);
        ThisProcess->setStandardOutputFile("stdout.txt");
        ThisProcess->setStandardErrorFile("sterr.txt");
        ThisProcess->start("java",{"-jar","-Dfile.encoding=UTF-8","-Xmx1024M","-Xms1024M",Server->Directory+"/server.jar","nogui"});;
        PID=ThisProcess->processId();
    }
    else if(id==1){
        /*
        CommandFile="tekitou";
        GenerateCMDFile(0,*Server,CommandFile);
        */
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
