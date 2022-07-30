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
private:
    void Write();
};

#endif // SERVERPROPERTY_H
