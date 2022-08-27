#ifndef JAVA_H
#define JAVA_H
#include <QString>
#include <QObject>

class java:public QObject
{
    Q_OBJECT
public:
    java();
    static bool hasjava();
    static QString version();
    static int versionint();
    static void downloadjdk();
public slots:
    void downloadedjava();
};

#endif // JAVA_H
