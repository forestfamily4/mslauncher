#include "downloadmanager.h"
#include "commandline.h"
#include "qdir.h"
#include "os.h"

DownloadManager::DownloadManager() : QObject(0)
{

    CommandLineController *c = new CommandLineController();
    this->os = os::Getos();
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(DownloadFinished(QNetworkReply *)));

    this->progresswindow = new QWidget();
    this->progresswindow->setFixedSize(600, 50);
    this->progresswindow->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    this->progresswindow->setWindowFlag(Qt::WindowCloseButtonHint, false);
    this->progresswindow->setWindowFlag(Qt::WindowMinimizeButtonHint, false);
    this->progressbar = new QProgressBar(this->progresswindow);
    QHBoxLayout *l = new QHBoxLayout();
    l->addWidget(this->progressbar);
    this->progresswindow->setLayout(l);
    this->progressbar->setTextVisible(true);
}
DownloadManager::~DownloadManager()
{
}

void DownloadManager::downloadcloudflared()
{
    if (os == 0)
    {
        FileDownload("", "https://github.com/cloudflare/cloudflared/releases/latest/download/cloudflared-windows-amd64.exe", "cloudflared.exe");
    }
    else if (os == 1)
    {
        // CommandLineController* c=new CommandLineController();
        // c.commandline(
        // brew install cloudflare/cloudflare/cloudflared
    }
    else if (os == 2)
    {
        // linux
    }
}

void DownloadManager::FileDownload(QString dir, QString url, QString filename, bool downloadingdialog)
{
    if (dir.toStdString() == "")
    {
        dir = QDir::currentPath();
    }
    this->dir = dir;
    this->url = QUrl::fromEncoded(url.toLocal8Bit());
    ;
    this->filename = filename;

    QNetworkRequest request(this->url);
    QObject::connect(manager.get(request), SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(DownloadProgress(qint64, qint64)));

    this->progresswindow->setWindowTitle("Downloading " + this->filename);
    if (!downloadingdialog)
    {
        return;
    }
    this->progresswindow->show();
    isdownloading = true;
}
void DownloadManager::DownloadFinished(QNetworkReply *data)
{

    this->progressbar->setFormat(tr("ファイルを書き込んでいます...."));
    QFile localFile(this->dir + "/" + this->filename);
    if (!localFile.open(QIODevice::WriteOnly))
        return;
    const QByteArray sdata = data->readAll();
    localFile.write(sdata);
    localFile.close();
    this->progresswindow->hide();
    isdownloading = false;
    emit done();
}

void DownloadManager::DownloadProgress(qint64 recieved, qint64 total)
{
    double a = recieved;
    double b = total;
    double division = (a / b) * 100;

    QString percent = QString::number(division) + "%";

    this->progressbar->setValue(round(division));
    this->progressbar->setFormat(tr("\nダウンロードしています...") + percent);
}
