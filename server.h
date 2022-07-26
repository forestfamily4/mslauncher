#ifndef SERVER_H
#define SERVER_H

#include <string>
using namespace std;

#include "serverproperty.h"
typedef struct GUIOption
{
  bool isCommandGuess;
  string DiscordBotToken;
  string DiscordChannelId;
};

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
