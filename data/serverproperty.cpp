#include "serverproperty.h"
#include <string>
using namespace std;
#include <fstream>
#include <iostream>
#include <sstream>

ServerProperty::ServerProperty()
{

}



void ServerProperty::Load(string Directory){
    std::ifstream stdoutfile;
    stdoutfile.open(Directory+"/server.properties");
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

void ServerProperty::Write(string Directory){
    ofstream file;
    file.open(Directory+"/server.properties", std::ios::out);

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
