#ifndef JAVA_H
#define JAVA_H
#include <QString>
#include <QObject>

class java:public QObject
{
    Q_OBJECT
public:
    java(bool isjavainms);
    static bool hasjava();
    static QString version();
    static int versionint();
    static void downloadjdk();
    static bool isjavainmslauncher;
public slots:
    void downloadedjava();
    static QString javapath();
};
#endif // JAVA_H
