#include "data.h"
#include <QTextCodec>
#include "back/java.h"

Data::Data()
{

}
void Data::Read(){

    QSettings* settings=new QSettings("mslauncher.ini", QSettings::IniFormat);

    settings->beginGroup("General");
    this->langindex=settings->value("lang",0).toInt();
    this->DirHistory=settings->value("DirHistory","").toString();
    this->ServerNum=settings->value("ServerNum",0).toInt();
    this->isJavainmslauncher=settings->value("isjavainmslauncher",false).toBool();
    settings->endGroup();
    if(this->ServerNum==0){
        return;
    }
    for(int i=0;i<this->ServerNum;i++){
        settings->beginGroup("Server"+QString::number(i));

        QString name= settings->value("ServerName","").toString();
        QString dir= settings->value("Directory","").toString();
        QString jar=settings->value("ServerJARFile","").toString();
        ServerType type=(ServerType)settings->value("ServerType","official").toInt();
        qDebug()<<name;

        Server* s=new Server(name,dir,jar,type);

        GUIOption* g=new GUIOption();

        QString token= settings->value("DiscordBotToken","").toString();
        QString channel=settings->value("DiscordChannelId","").toString();
        bool guess=settings->value("CommandGuess",true).toBool();

        g->isCommandGuess=guess;
        settings->endGroup();
        s->GUIOptions=*g;
        this->Servers.push_back(*s);
    }






}
void Data::Write(){
    QSettings* settings=new QSettings("mslauncher.ini", QSettings::IniFormat);
    settings->clear();
    this->ServerNum=this->Servers.size();

    settings->beginGroup("General");
    settings->setValue("lang",langindex);
    settings->setValue("DirHistory",DirHistory);
    settings->setValue("ServerNum",this->ServerNum);
    settings->setValue("isjavainmslauncher",this->isJavainmslauncher);
    settings->endGroup();

    for(int i=0;i<Servers.size();i++){
        Server* s=&Servers[i];
        settings->beginGroup("Server"+QString::number(i));
        settings->setValue("ServerName",s->ServerName);
        settings->setValue("Directory",s->Directory);
        settings->setValue("ServerJARFile",s->ServerJARFile);
        settings->setValue("ServerType",s->ServerType);
        //guioption
        GUIOption &g=s->GUIOptions;
        settings->setValue("CommandGuess",g.isCommandGuess);

        settings->endGroup();
    }
    settings->sync();
}
void Data::SetisJavainmslauncher(bool a){
    this->isJavainmslauncher=a;
    java::isjavainmslauncher=a;
}
