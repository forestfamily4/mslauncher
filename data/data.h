#ifndef DATA_H
#define DATA_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include <string>
#include <vector>
using namespace std;

#include "server.h"

class Data
{
public:
    Data();
    vector<Server> Servers;
    void Read();
    void Write();
protected:
    bool StringToBool(string s);
    string BoolToString(bool a);
};

#endif // DATA_H
