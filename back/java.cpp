#include "java.h"
#include "commandline.h"
#include <QRegExp>
#include <QDir>
#include "downloadmanager.h"
#include "zip.h"
#include <QProgressDialog>
#include <QRegularExpression>

bool java::isjavainmslauncher;

java::java(bool isjavainms)
{
    isjavainmslauncher = isjavainms;
}
bool java::hasjava()
{
    CommandLineController *c = new CommandLineController();
    QString a = c->Command({javapath(), "-version"});
    qDebug() << a << "javalog" << a.toLower().contains("jdk");
    if (a.toLower().contains("java") || a.toLower().contains("jdk"))
    {
        return true;
    }
    else
    {
        return false;
    }
}
QString java::version()
{
    CommandLineController *c = new CommandLineController();
    QString a = c->Command({javapath(), "-version"});
    qDebug() << "thinking";
    if (a.contains("open"))
    {
        QRegExp b("[0-9]+");
        b.indexIn(a);
        QString r = b.cap(0);
        return r;
    }
    else
    {

        QRegExp b("version \"[0-9]*\.[0-9]*\.[0-9]*");
        b.indexIn(a);
        QString d = b.cap(0);
        qDebug()<<d;
        QRegExp Second("[0-9]+\.[0-9]+\.[0-9]+");
        Second.indexIn(d);
        QString result = Second.cap(0);
        qDebug()<<result<<"ha";
        if (result.mid(1, 1) == "\.")
        {
            return result.mid(2, 1);
        }
        else
        {
            return result.mid(0, 2);
        }
    }
}
int java::versionint()
{
    QString a = version();
    qDebug() << a;
    return a.toInt();
}
void java::downloadjdk()
{
    qDebug() << "thinking";
    DownloadManager *d = new DownloadManager();
    connect(d, &DownloadManager::done, [=]()
            {
        zip::extract(QDir::currentPath()+"/java.zip",QDir::currentPath());
        QFile a("java.zip");
        a.remove();
        QDir dir(QDir::currentPath());
        QStringList l=dir.entryList();
        l=l.filter(QRegularExpression("(java|jdk).*"));
        if(l.empty()){
            return;
        }
        QString r=l.first();
        dir.rename(QDir::currentPath()+"/"+r,QDir::currentPath()+"/"+"java"); });

    d->FileDownload(QDir::currentPath(), "https://download.java.net/java/GA/jdk19/877d6127e982470ba2a7faa31cc93d04/35/GPL/openjdk-19_windows-x64_bin.zip", "java.zip");
}
void java::downloadedjava()
{
    //
}
QString java::javapath()
{
    if (isjavainmslauncher)
    {
        return QDir::currentPath() + "/java/bin/java";
    }
    else
    {
        return "java";
    }
}
