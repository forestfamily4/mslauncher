#include "java.h"
#include "commandline.h"
#include <QRegExp>
#include <QDir>
#include "downloadmanager.h"
#include "zip.h"

java::java()
{

}
bool java::hasjava(){
    CommandLineController* c=new CommandLineController();
    QString a=c->Command({"java","-version"});
    qDebug()<<a;

    if(a.contains("64-Bit Server VM")&& a.toLower().contains("jdk")){
        return true;
    }
    else{
        return false;
    }
}
QString java::version(){
    CommandLineController* c=new CommandLineController();
    QString a=c->Command({"java","-version"});

    QRegExp b("(java version \"|openjdk version \"|openjdk |java )[0-9]*\.[0-9]*\.[0-9]*");
    b.indexIn(a);
    QString d=b.cap(0);

    QRegExp Second("[0-9]+\.[0-9]+\.[0-9]+");
    Second.indexIn(d);
    QString result=Second.cap(0);

    return result;
}
void java::downloadjdk(){
    DownloadManager* d=new DownloadManager();
    connect(d,&DownloadManager::done,[=](){
        zip::extract(QDir::currentPath()+"/java.zip",QDir::currentPath());
    });
    d->FileDownload(QDir::currentPath(),"https://download.java.net/java/GA/jdk19/877d6127e982470ba2a7faa31cc93d04/35/GPL/openjdk-19_windows-x64_bin.zip","java.zip");
}
void java::downloadedjava(){
    //
}
