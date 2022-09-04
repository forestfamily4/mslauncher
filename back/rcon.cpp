#include "rcon.h"
#include <iostream>

rcon::rcon(QObject *parent)
    : QObject{parent}
{
}
void rcon::auth(string password, int port)
{
    this->tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(closeConnection()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    // connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    pid = rand();
    qDebug() << pid;
    tcpSocket->connectToHost("localhost", port);
    tcpSocket->waitForConnected();
    isconnected = false;
    tcpSocket->write(packetbuild(3, password));
}
void rcon::cmd(string cmd)
{
    tcpSocket->write(packetbuild(2, cmd));
}
QByteArray rcon::packetbuild(qint32 packettype, string s)
{
    QByteArray id;
    id.append((const char *)&pid, sizeof(pid));
    QByteArray ptype;
    ptype.append((const char *)&packettype, sizeof(packettype));
    QByteArray strbody = QString::fromStdString(s).toUtf8();
    char empty = '\x00';
    qint32 length = 4 + 4 + strbody.length() + 2;
    QByteArray l;
    l.append((const char *)&length, sizeof(length));
    QByteArray result = l + id + ptype + strbody;
    result.append(empty);
    result.append(empty);
    qDebug() << result;
    return result;
}

void rcon::receiveData()
{
    qDebug("データを受け取っています");
    QByteArray rcv_bytes = tcpSocket->readAll();

    if (this->isconnected)
    {
        rcv_bytes.chop(1);
        rcv_bytes.remove(0, 12);
    }
    else
    {
        rcv_bytes.chop(6);
        rcv_bytes.remove(0, 4);
    }
    QString rcv_data;
    if (rcv_bytes.length() == 0)
    {
        rcv_data = "none";
    }
    else
    {
        rcv_data = QString::fromUtf8(rcv_bytes);
    }

    QByteArray pidd;
    pidd.append((const char *)&pid, sizeof(pid));
    if (pidd == rcv_bytes)
    {
        isconnected = true;
        qDebug("Authできました。");
        rcv_data = tr("RCON接続できました。");
    }
    cout << "test" << endl;
    qDebug() << rcv_bytes;
    qDebug() << rcv_data;
    if (isconnected)
    {
        data += rcv_data.toStdString();
        data += "\n";
    }
}

void rcon::closeConnection()
{
    qDebug("close: close conn");
    isconnected = false;
    data = "";
}

void rcon::error()
{
    QString mess = tcpSocket->errorString();
    qDebug("error: %s", mess.toUtf8().constData());
    tcpSocket->close();
}
