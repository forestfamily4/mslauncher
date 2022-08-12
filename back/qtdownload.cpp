#include "downloadmanager.h"
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkRequest>
#include <QFile>
#include <QDebug>

QtDownloadManager::QtDownloadManager() : QObject(0) {
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(DownloadManagerFinished(QNetworkReply*)));
}

QtDownloadManager::~QtDownloadManager() {

}


void QtDownloadManager::setTarget(const QString &t) {
    this->target = t;
}

void QtDownloadManager::DownloadManagerFinished(QNetworkReply *data) {
    QFile localFile("DownloadManageredfile");
    if (!localFile.open(QIODevice::WriteOnly))
        return;
    const QByteArray sdata = data->readAll();
    localFile.write(sdata);
    localFile.close();

    emit done();
}

void QtDownloadManager::DownloadManager() {
    QUrl url = QUrl::fromEncoded(this->target.toLocal8Bit());
    QNetworkRequest request(url);
    QObject::connect(manager.get(request), SIGNAL(DownloadManagerProgress(qint64,qint64)), this, SLOT(DownloadManagerProgress(qint64,qint64)));

}

void QtDownloadManager::DownloadManagerProgress(qint64 recieved, qint64 total) {
    qDebug() << recieved << total;
    qDebug()<< (recieved/total);
}
