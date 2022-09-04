#ifndef SERVERPROPERTY_H
#define SERVERPROPERTY_H
#include <string>
#include <vector>
#include <QString>
#include <QDebug>
using namespace std;

class ServerProperty
{
public:
    ServerProperty();
    std::vector<vector<QString>> Properties;
    void Load(QString Directory);
    void Write(QString Directory);
    QString Get(QString key);
    int GetIndex(QString key);
};

#endif // SERVERPROPERTY_H
