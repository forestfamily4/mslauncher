#include "zip.h"
#include <QProgressDialog>

zip::zip()
{
}
void zip::extract(QString path, QString parentpath)
{
    QProcess *p = new QProcess();
    p->setProgram("7z/7z");
    p->setStandardOutputFile(QDir::currentPath() + "/temp/zip" + QString::number(rand()) + ".txt");
    p->setArguments({"x", "-y", "-o" + parentpath, path});
    p->start();
    p->waitForFinished();
}
