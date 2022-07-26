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
};

#endif // DATA_H
