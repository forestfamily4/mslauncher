#include "commandline.h"
#include <string>
#include <iostream>
#include "server.h"
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
    SyncCommandLine(Server,id);
}
void CommandLineController::EnterCommand(string Command)
{

}
bool CommandLineController::HasJava()
{
    return true;
}
void CommandLineController::GenerateCMDFile(int id,Server Server){
    std::ofstream file;
    string content="";
    string filename="";
    if(id==0){//BuildServer
        filename="buildserver.bat";
        content+="cd "+Server.Directory;
        content+="\n java -Xmx1024M -Xms1024M -jar server.jar nogui";
        std::cout << content <<endl;
    }

    if(filename==""){
        return;
    }
    file.open(filename);
    file<< content<< endl;
    file.close();
}

void CommandLineController::SyncCommandLine(Server* Server,int id){
    string CommandFile;
    if(id==0){
        CommandFile="buildserver.bat";
        GenerateCMDFile(0,*Server);
    }

    if(CommandFile==""){
        return;
    }
    ThisProcess=new QProcess();
    std::cout << CommandFile << std::endl;
   // process->setProgram(QString::fromStdString(CommandFile));
    ThisProcess->setWorkingDirectory(QString::fromStdString(Server->Directory));
    ThisProcess->setStandardOutputFile("stdout.txt");
    ThisProcess->setStandardErrorFile("sterr.txt");
    ThisProcess->start("java.exe",{"-jar","-Xmx1024M","-Xms1024M",QString::fromStdString(Server->Directory+"/server.jar"),"nogui"});;
    PID=ThisProcess->processId();

}
