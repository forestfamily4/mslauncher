#ifndef RCON_H
#define RCON_H

#include <QObject>
#include <string>
#include <QTcpSocket>
using namespace std;
class rcon : public QObject
{
    Q_OBJECT
public:
    explicit rcon(QObject *parent = nullptr);

public:
    void auth(QString password, int port);
    void cmd(QString cmd);
    bool isconnected = false;
    QString data = "";
private slots:
    void receiveData();
    void error();
    void closeConnection();

private:
    QTcpSocket *tcpSocket;
    QByteArray packetbuild(qint32 packettype, QString s);
    qint32 pid;
signals:
};

#endif // RCON_H
