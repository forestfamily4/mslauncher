#include "server.h"
#include <string>
using namespace std;

Server::Server(string ServerName, string Directory)
{
  this->ServerName = ServerName;
  this->Directory = Directory;
  GUIOption g;
  g.DiscordBotToken = "";
  g.DiscordChannelId = "";
  g.isCommandGuess = true;
  this->GUIOptions = g;
}
