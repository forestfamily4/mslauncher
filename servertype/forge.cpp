#include "forge.h"
#include "back/downloadmanager.h"

Forge::Forge()
{
}
void Forge::Get(QString ver)
{
    DownloadManager *d = new DownloadManager();
    QDateTime q;
    tempfile = "forge" + QString::number(rand()) + q.currentDateTime().toString("hhmmss");
    QString html = "";
    if (ver == "latest")
    {
        html = "https://files.minecraftforge.net/net/minecraftforge/forge/index.html";
    }
    else
    {
        html = "https://files.minecraftforge.net/net/minecraftforge/forge/index_" + ver + ".html";
    }
    d->FileDownload(QDir::currentPath() + "/temp", html, tempfile, false);

    connect(d, SIGNAL(done()), this, SLOT(Downloadedhtml()));
}
void Forge::Downloadedhtml()
{
    QFile f(QDir::currentPath() + "/temp/" + tempfile);
    f.open(QIODevice::ReadOnly);
    QByteArray result = f.readAll();
    int a = result.indexOf("promo-recommended");
    result = result.mid(a, 2000);
    QRegExp r("https://maven.minecraftforge.net/net/minecraftforge/forge/[0-9]*\.[0-9]*-[0-9]*\.[0-9]*\.[0-9]*/forge-[0-9]*\.[0-9]*-[0-9]*\.[0-9]*\.[0-9]*-installer.jar");
    r.indexIn(result);
    QString s = r.cap(0);
    link = s;
    emit done();
    qDebug() << link;
}
