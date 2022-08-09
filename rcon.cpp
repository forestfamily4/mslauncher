#include "rcon.h"

rcon::rcon(QObject *parent)
    : QObject{parent}
{
    this->tcpSocket=new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(closeConnection()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    pid=rand();

}
void rcon::auth(string password,int port){
    tcpSocket->connectToHost("localhost",port);
    tcpSocket->waitForConnected();
    isconnected=true;
    tcpSocket->write(packetbuild(3,password));
}
void rcon::cmd(string cmd){
    tcpSocket->write(packetbuild(2,cmd));
}
QByteArray rcon::packetbuild(qint32 packettype,string s){
    QByteArray id;
    id.append((const char*)&pid, sizeof(pid));
    QByteArray ptype;
    ptype.append((const char*)&packettype, sizeof(packettype));
     QByteArray body=  QByteArray::fromStdString(s+"\0");

    int length=4+4+body.length()-4;
     QByteArray l;
    l.append((const char*)&length,sizeof(length));

     QByteArray result=id+ptype+body+l;
    qDebug()<<"-------------";
     qDebug()<<id;
     qDebug()<<ptype;
     qDebug()<<body;
     qDebug()<<"-------------";
     qDebug()<<result;
     qDebug()<<"-------------";
     return result;
}

void rcon::receiveData()
{
    qDebug("rcv: start");

    //受信
    QByteArray rcv_bytes = tcpSocket->readAll();

    QString rcv_data;
    //データなし？
    if (rcv_bytes.length() == 0) rcv_data = "[no data]";
    //データあり → UTF8でQString化
    else rcv_data = QString::fromUtf8(rcv_bytes);

    //表示
    qDebug("rcv: '%s'", rcv_data.toUtf8().constData());

    //クローズ
    tcpSocket->close();
}

void rcon::closeConnection()
{
    qDebug("close: close conn");
    tcpSocket->close();
    isconnected=false;
}

//エラー (tcpSocket の error() シグナル)
void rcon::error()
{
    QString mess = tcpSocket->errorString();
    qDebug("error: %s", mess.toUtf8().constData());

    tcpSocket->close();
    isconnected=false;
}
