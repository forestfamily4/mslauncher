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
  QString DiscordBotToken;
  QString DiscordChannelId;
};
typedef guiOption GUIOption;
class Server
{
public:
  Server(QString ServerName,QString Directory);
  QString ServerName="";
  QString Directory="";
  QString ServerJARFile="";
  ServerProperty ServerProperty;
   GUIOption GUIOptions;
  void LoadProperties();
};

#endif // SERVER_H
