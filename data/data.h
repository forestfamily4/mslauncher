#ifndef DATA_H
#define DATA_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include <string>
#include <vector>
#include <fstream>
#include <QtCore/QDebug>
#include <QSettings>
using namespace std;

#include "server.h"

class Data
{
public:
    Data();
    vector<Server> Servers;
    void Read();
    void Write();
    int langindex = 0;
    QString DirHistory = "";
    bool isJavainmslauncher = false;
    void SetisJavainmslauncher(bool a);

private:
    int ServerNum = 0;
};

#endif // DATA_H
