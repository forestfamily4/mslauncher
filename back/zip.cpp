#include "zip.h"

zip::zip()
{

}
void zip::extract(QString path,QString parentpath){
    QProcess* p=new QProcess();
    p->setProgram("7z/7z");
    p->setArguments({"x","-o"+parentpath,path});
    p->start();
    p->waitForFinished();
    qDebug()<<p->readAllStandardOutput();
}
