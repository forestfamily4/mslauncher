#include "download.h"
#include "qdir.h"

download::download(QObject *parent) :
    QObject(parent)
{

    CommandLineController* c=new CommandLineController();
    this->os=c->Getos();
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(finished(QNetworkReply*)));

}

void download::downloadcloudflared(){
    if(os==0){
        FileDownload("","https://github.com/cloudflare/cloudflared/releases/latest/download/cloudflared-windows-amd64.exe","cloudflared.exe");
    }
    else if(os==1){
        //CommandLineController* c=new CommandLineController();
        //c.commandline(うんたらかんたら
    }
    else if(os==2){
        //linuxめんどくさ
    }
}

void download::FileDownload(QString dir,QString url,QString filename){
    if(dir==""){
        dir=QDir::currentPath();
    }
    this->dir=dir;
    this->url=QUrl::fromEncoded(url.toLocal8Bit());;
    this->filename=filename;


    QNetworkRequest request(url);
    manager.get(request);
    connect(manager.get(request), SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    qstatusbar.showMessage(filename+"をダウンロードしています...");

    qstatusbar.addWidget(this->qprogressbar);
}

void download::downloadProgress(qint64 recieved, qint64 total){
    qDebug()<<total;
    qDebug()<<recieved;
    qDebug()<<recieved/total;
    qprogressbar->setValue(recieved/total);
}

void download::finished(QNetworkReply* pReply) {
    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();
    QFile file(this->dir+filename);
    file.open(QIODevice::WriteOnly);
    file.write(m_DownloadedData);
    file.close();
    this->qstatusbar.close();
}
