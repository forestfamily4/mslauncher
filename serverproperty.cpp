#include "serverproperty.h"

#include <string>
using namespace std;
#include <fstream>

ServerProperty::ServerProperty()
{

}
void ServerProperty::Load(string Directory){
    std::ifstream stdoutfile;
    stdoutfile.open(Directory+"/server.properties");
    string linebuffer="";
    int i=0;
    while(getline(stdoutfile,linebuffer,' ')){
        std::cout<<linebuffer<<endl;
         Properties[i/2].push_back(linebuffer);
         i+=1;
    }
}
