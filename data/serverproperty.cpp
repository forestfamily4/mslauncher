#include "serverproperty.h"
#include <string>
using namespace std;
#include <fstream>
#include <iostream>
#include <sstream>

ServerProperty::ServerProperty()
{

}



void ServerProperty::Load(QString Directory){
    std::ifstream stdoutfile;
    stdoutfile.open((Directory+"/server.properties").toStdString());
    string linebuffer="";
    int i=0;
    Properties.clear();
    while(getline(stdoutfile,linebuffer,'\n')){
        if(linebuffer.find('=')!=std::string::npos){
            vector<string> v=split(linebuffer,'=');
            if(v.size()==1){v.push_back("");}
            Properties.push_back(v);}
         i+=1;
    }
    stdoutfile.close();
}

void ServerProperty::Write(QString Directory){
    ofstream file;
    file.open((Directory+"/server.properties").toStdString(), std::ios::out);

    string result="";

    for(int i=0;i<this->Properties.size();i++){
        if(i==this->Properties.size()-1){
            result+=Properties[i][0]+"="+Properties[i][1];
        }
        else{
         result+=Properties[i][0]+"="+Properties[i][1]+"\n";
        }
    }
    file<<result<<endl;
    file.close();
}

vector<string> ServerProperty:: split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

string ServerProperty::Get(string key){
    for(int i=0;i<this->Properties.size();i++){
        if(key==this->Properties[i][0]){
            return this->Properties[i][1];
        }
    }
    return "none";
}
