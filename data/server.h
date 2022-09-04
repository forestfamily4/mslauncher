#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <QString>
using namespace std;

#include "serverproperty.h"
struct guiOption
{
public:
    bool isCommandGuess;
};
typedef guiOption GUIOption;

typedef enum
{
    official,
    mohist,
    spigot,
    papermc,
    fabric,
    forge
} ServerType;

class Server
{
public:
    Server(QString ServerName, QString Directory, QString JarFile, ::ServerType ServerType);
    QString ServerName = "";
    QString Directory = "";
    QString ServerJARFile = "";
    ServerProperty ServerProperty;
    ServerType ServerType;
    GUIOption GUIOptions;
    void LoadProperties();
    void SetServerType(QString type);
};

#endif // SERVER_H
