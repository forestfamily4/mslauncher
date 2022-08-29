#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "os.h"
#include "downloadmanager.h"
#include <QFile>
#include <QErrorMessage>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timerId = startTimer(200);
    this->cloudflare=new CommandLineController();
    QDir d("temp");
    if(d.exists()){
        d.removeRecursively();
    }
    QDir a;
    a.mkdir("temp");
}

MainWindow::~MainWindow()
{
    killTimer(timerId);
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(this->iscloudflareconnecting){
        this->cloudflare->kill();
        this->iscloudflareconnecting=false;
        ui->pushButton->setText("接続！");
        return;
    }


    QString cloudflare=ui->lineEdit_cloudflared->text();
    QString localhost=ui->lineEdit_url->text();
    if(os::Getos()==0){
        if(!QFile("cloudflared.exe").exists()){
            DownloadManager* d=new DownloadManager();
            d->downloadcloudflared();
        }
    }
    else{
        CommandLineController* c =new CommandLineController();
        QString a=c->Command({"cloudflared"});
        if(a==""){
            return ErrorWindow("cloudflaredがインストールされていません。");
        }
    }

   this->cloudflare->cloudflared(cloudflare,localhost);
}

void MainWindow::ErrorWindow(QString info){
    QErrorMessage e;
    e.showMessage(info);
    e.exec();
}

void MainWindow::timerEvent(QTimerEvent *event){
    if(!this->cloudflare->isrunning){
        return;
    }
    QFile a(this->cloudflare->stdoutfilepath);
    a.open(QIODevice::ReadOnly);
    QString b=a.readAll();
    if(b==this->cloudflarestdouttemp){
        return;
    }
    this->cloudflarestdouttemp=b;
    a.close();
    qDebug()<<b;
    if(b.contains("Only one usage of each socket address (protocol/network address/port) is normally permitted")){
        return ErrorWindow("同じポートには一つしか接続できません。");
    }
    if(b!=""&&!iscloudflareconnecting){
        ErrorWindow("接続しました。");
        this->iscloudflareconnecting=true;
        ui->pushButton->setText("接続を切る");
    }
}
void MainWindow::closeEvent (QCloseEvent *event)
{
    this->cloudflare->kill();
}

