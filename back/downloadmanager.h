#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStatusBar>
#include <QProgressBar>
#include <string>
#include <QLayout>
#include <QTimer>
#include <QEventLoop>

using namespace std;
class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager();
    ~DownloadManager();

public:
    bool isdownloading = false;
    void downloadcloudflared();

private:
    QProgressBar *progressbar;
    QWidget *progresswindow;

    QString dir;
    QUrl url;
    QString filename;
    int os;

private:
    QByteArray downloadedData() const;
    QNetworkAccessManager manager;
    QString target;

signals:
    void done();

public slots:
    void FileDownload(QString dir, QString url, QString filename, bool downloadingdialog = true);
    void DownloadFinished(QNetworkReply *data);
    void DownloadProgress(qint64 recieved, qint64 total);
};

#endif // DOWNLOADMANAGER_H
