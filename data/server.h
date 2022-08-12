#ifndef SERVER_H
#define SERVER_H

#include <string>
using namespace std;

#include "serverproperty.h"
 struct guiOption
{
  bool isCommandGuess;
  string DiscordBotToken;
  string DiscordChannelId;
};
typedef guiOption GUIOption;
class Server
{
public:
  Server(string ServerName,string Directory);
  string ServerName;
  string Directory;
  ServerProperty ServerProperty;
   GUIOption GUIOptions;
};

#endif // SERVER_H
