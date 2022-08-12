#ifndef QTDownloadManager_H
#define QTDownloadManager_H

#include <QObject>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>



class QtDownloadManager : public QObject {
    Q_OBJECT
public:
    explicit QtDownloadManager();
    ~QtDownloadManager();

    void setTarget(const QString& t);

private:
    QNetworkAccessManager manager;
    QString target;

signals:
    void done();

public slots:
    void DownloadManager();
    void DownloadManagerFinished(QNetworkReply* data);
    void DownloadManagerProgress(qint64 recieved, qint64 total);
};

#endif
