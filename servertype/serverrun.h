#ifndef SERVERRUN_H
#define SERVERRUN_H
#include <QStringList>
#include "data/server.h"
class ServerRun
{
public:
    ServerRun();
    QStringList GetArgs(ServerType& s,QString dir,QString ServerJARFile);
};

#endif // SERVERRUN_H
