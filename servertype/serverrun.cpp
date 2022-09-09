#include "serverrun.h"
#include "servertype/forge.h"

ServerRun::ServerRun()
{
}
QStringList ServerRun::GetArgs(ServerType& s,QString dir,QString ServerJARFile){
    if(s==ServerType::forge){
        QFile f(dir+"/run.bat");
        f.readAll();
        return {""};
    }
    else{
        return {"-jar","-Xmx1024M", "-Xms1024M",  ServerJARFile,"nogui"};
    }
}
