#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QInputDialog>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_NewServerButton_clicked()
{
    QString ServerName=  QInputDialog::getText(this, "Server Name", "Enter Server Name");
   QString Directory= QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                      "/home",
                                      QFileDialog::ShowDirsOnly
                                          | QFileDialog::DontResolveSymlinks);
    qDebug() << ServerName <<2000;
   qDebug() << Directory <<2000;
    Server* s=new Server(ServerName.toStdString(),Directory.toStdString());
    Data.Servers.push_back(*s);
    Render();
    delete s;
}

void MainWindow::Render(){
    vector<Server>& Servers=Data.Servers;
    ui->comboBox_Servers->clear();
    for(int i=0;i<Servers.size();i++){
        ui->comboBox_Servers->addItem(QString::fromStdString(Servers[i].ServerName));
    }
}
