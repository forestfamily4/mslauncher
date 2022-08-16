#include "server.h"
#include <string>
using namespace std;

Server::Server(QString ServerName, QString Directory,QString JarFile,::ServerType ServerType)
{
  this->ServerName = ServerName;
  this->Directory = Directory;
  this->GUIOptions.DiscordBotToken="";
  this->GUIOptions.DiscordChannelId="";
  this->GUIOptions.isCommandGuess=true;
  this->ServerJARFile=JarFile;
  this->ServerType=ServerType;
}
void Server::LoadProperties(){
    this->ServerProperty.Load(this->Directory);
}
void Server::SetServerType(QString type){
    type=type.toLower();
    if(type=="official"){
        this->ServerType=ServerType::official;
    }
    else if(type=="mohist"){
        this->ServerType=ServerType::mohist;
    }
    else if(type=="spigot"){
        this->ServerType=ServerType::spigot;
    }
    else if(type=="papermc"){
        this->ServerType=ServerType::papermc;
    }
    else if(type=="fabric"){
        this->ServerType=ServerType::fabric;
    }
    else if(type=="forge"){
        this->ServerType=ServerType::forge;
    }

}
