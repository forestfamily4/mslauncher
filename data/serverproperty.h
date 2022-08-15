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
    std::vector<vector<string>> Properties;
    void Load(QString Directory);
    void Write(QString Directory);
    string Get(string key);
private:
    vector<string> split(const string &s, char delim);
};

#endif // SERVERPROPERTY_H
