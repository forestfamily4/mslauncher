#ifndef SERVERPROPERTY_H
#define SERVERPROPERTY_H
#include <string>
#include <vector>
using namespace std;

class ServerProperty
{
public:
    ServerProperty();
    std::vector<vector<string>> Properties;
    void Load(string Directory);
    void Write(string Directory);
private:
    vector<string> split(const string &s, char delim);
};

#endif // SERVERPROPERTY_H
