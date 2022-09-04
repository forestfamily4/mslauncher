#include "serverproperty.h"
#include <QDir>

ServerProperty::ServerProperty()
{

}



void ServerProperty::Load(QString Directory){
    QFile file(Directory+"/server.properties");
    file.open(QIODevice::ReadOnly);
    QString s=file.readAll();
    Properties.clear();
    QStringList a=s.split("\n");

    for(int i=0;i<a.size();i++){
        if(a[i].contains("=")){
            QStringList b=a[i].split("=");
            vector<QString> c;
            c.push_back(b[0]);
            c.push_back(b[1]);
            Properties.push_back(c);
        }
    }

    file.close();
}

void ServerProperty::Write(QString Directory){
    QFile file(Directory+"/server.properties");
    file.open(QIODevice::WriteOnly);
    for(int i=0;i<Properties.size();i++){
        file.write((Properties[i][0]+"="+Properties[i][1]+"\n").toUtf8());
    }
    file.close();
}

QString ServerProperty::Get(QString key){
    for(int i=0;i<this->Properties.size();i++){
        if(key==this->Properties[i][0]){
            return this->Properties[i][1];
        }
    }
    return "none";
}

int ServerProperty::GetIndex(QString key){
    for(int i=0;i<this->Properties.size();i++){
        if(key==this->Properties[i][0]){
            return i;
        }
    }
    return 0;
}
