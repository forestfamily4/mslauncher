#ifndef DOWNLOAD_H
#define DOWNLOAD_H
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStatusBar>
#include <QProgressBar>
#include <string>
#include "commandline.h"
using namespace std;
class download:QObject
{
public:
    explicit download(QObject *parent = nullptr);
public:
    QByteArray downloadedData() const;
    void downloadcloudflared();
    int os;
private:
    QStatusBar qstatusbar;
    QProgressBar* qprogressbar;

    QString dir;
    QUrl url;
    QString filename;

    void FileDownload(QString dir,QString url,QString filename);
    QNetworkAccessManager manager;
    QByteArray m_DownloadedData;
public slots:
    void downloadProgress(qint64 recieved, qint64 total);
    void finished(QNetworkReply* pReply);
};

#endif // DOWNLOAD_H
