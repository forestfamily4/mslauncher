#ifndef ZIP_H
#define ZIP_H
#include <QString>
#include <QProcess>
#include <QDir>

class zip
{
public:
    zip();
    static void extract(QString path,QString parentpath);
};

#endif // ZIP_H
