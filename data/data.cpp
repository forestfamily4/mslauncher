#include "data.h"
#include <QTextCodec>

Data::Data()
{

}
void Data::Read(){

    QSettings* settings=new QSettings("mslauncher.ini", QSettings::IniFormat);

    settings->beginGroup("General");
    this->langindex=settings->value("lang",0).toInt();
    this->DirHistory=settings->value("DirHistory","").toString();
    this->ServerNum=settings->value("ServerNum",0).toInt();
    qDebug()<<ServerNum;
    settings->endGroup();

    for(int i=0;i<this->ServerNum;i++){
        settings->beginGroup("Server"+QString::number(i));

        QString name= settings->value("ServerName","").toString();
        QString dir= settings->value("Directory","").toString();
        QString jar=settings->value("ServerJARFile","").toString();
        qDebug()<<name;

        Server* s=new Server(name,dir);

        s->ServerJARFile=jar;

        GUIOption* g=new GUIOption();

        QString token= settings->value("DiscordBotToken","").toString();
        QString channel=settings->value("DiscordChannelId","").toString();
        bool guess=settings->value("CommandGuess",true).toBool();

        g->DiscordChannelId=channel;
        g->isCommandGuess=guess;
        g->DiscordBotToken=token;
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
    settings->endGroup();

    for(int i=0;i<Servers.size();i++){
        Server* s=&Servers[i];
        settings->beginGroup("Server"+QString::number(i));
        settings->setValue("ServerName",s->ServerName);
        settings->setValue("Directory",s->Directory);
        settings->setValue("ServerJARFile",s->ServerJARFile);
        //guioption
        GUIOption &g=s->GUIOptions;
        settings->setValue("DiscordBotToken",g.DiscordBotToken);
        settings->setValue("DiscordChannelId",g.DiscordChannelId);
        settings->setValue("CommandGuess",g.isCommandGuess);

        settings->endGroup();
    }
    settings->sync();
}
