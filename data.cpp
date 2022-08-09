#include "data.h"
#include <string>
#include <fstream>
#include <QtCore/QDebug>
using namespace std;
Data::Data()
{

}
void Data::Read(){

}
void Data::Write(){
    string result="-";
    for(int i=0;i<this->Servers.size();i++){
        Server& s=Servers[i];
        string name=s.ServerName;
        string dir=s.Directory;
        GUIOption g= s.GUIOptions;

        vector<string> gdata={g.DiscordBotToken,g.DiscordChannelId,BoolToString(g.isCommandGuess)};
        result+=(name+"\n"+dir);
        for(int ii=0;i<gdata.size();ii++){
            result+=gdata[ii]+"\n";
        }
        result+="\n-";
    }
    fstream f;
    qDebug()<<QString::fromStdString(result);
    f.open("serverdata",std::ios::out);
    f<<result<<endl;
    f.close();
}

string Data:: BoolToString(bool a){
    if(a){
        return "0";
    }
    else{
        return "1";
    }
}
bool Data::StringToBool(string s){
    if(s=="0"){
        return true;
    }
    else if(s=="1"){
        return false;
    }
    else{
        return false;
    }
}
