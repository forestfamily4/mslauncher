#ifndef FORGE_H
#define FORGE_H

#include <QFile>
#include <QRegExp>
#include <QDir>
class Forge: public QObject
{
    Q_OBJECT
public:
    Forge();
    void Get(QString ver);
    QString link="";
private:
    QString tempfile="";
signals:
    void done();
private slots:
    void Downloadedhtml();
};

#endif // FORGE_H
