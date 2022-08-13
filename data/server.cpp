#include "server.h"
#include <string>
using namespace std;

Server::Server(QString ServerName, QString Directory)
{
  this->ServerName = ServerName;
  this->Directory = Directory;
  this->GUIOptions.DiscordBotToken="";
  this->GUIOptions.DiscordChannelId="";
  this->GUIOptions.isCommandGuess=true;
}
