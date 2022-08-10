#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "commandline.h"
#include "server.h"
#include <QInputDialog>
#include <QFileDialog>
#include <fstream>
#include <QTimer>
#include <QErrorMessage>
#include <QProcess>
#include <QMessageBox>
#include "rcon.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    remove("stdout.txt");
    ui->setupUi(this);
    this->CommandLine_Server=nullptr;
    timerId = startTimer(200);
    IsServerRunning=false;
}

MainWindow::~MainWindow()
{
    killTimer(timerId);
    delete ui;

}

void MainWindow::on_NewServerButton_clicked()
{
    QString ServerName=  QInputDialog::getText(this, "サーバーの名前", "新しく作るサーバーの名前を入力してください。");
    if(ServerName==""){return;}
    QString Directory= QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                      "/home",
                                      QFileDialog::ShowDirsOnly
                                          | QFileDialog::DontResolveSymlinks);
    if(Directory==""){return;}
    Server* s=new Server(ServerName.toStdString(),Directory.toStdString());
    Data.Servers.push_back(*s);
    vector<Server>& Servers=Data.Servers;
    ui->comboBox_Servers->clear();
    for(int i=0;i<Servers.size();i++){
        ui->comboBox_Servers->addItem(QString::fromStdString(Servers[i].ServerName));
    }
    delete s;


}

void MainWindow::timerEvent(QTimerEvent *event){
    ui->plainTextEdit_CommandResult->setPlainText(QString::fromStdString(rcon.data));




    if(filesystem::exists("stdout.txt")){
        std::ifstream stdoutfile;
        stdoutfile.open("stdout.txt");
        string line="";
        string linebuffer="";
        while(getline(stdoutfile,linebuffer)){
            line+="\n"+linebuffer;
        }
        ui->label_CommandLine->setText(QString::fromStdString(line));

        if(stdoutFileSize!=line.length()){
            if(line.find("You need to agree to the EULA")!=std::string::npos){
                QErrorMessage qmes;
                qmes.showMessage("EULAに同意する必要があります。\nEULA=trueに変更して、もう一度サーバーを起動して下さい。");
                qmes.exec();
                //windowsのnotepad
                QProcess process;
                cout<<Data.Servers[CurrentServer()].Directory+"/eula.txt"<<endl;
                process.start("notepad.exe",{QString::fromStdString(Data.Servers[CurrentServer()].Directory+"/eula.txt")});
                process.waitForFinished();
            }
            else {
                if(line.find("RCON")!=std::string::npos){
                    this->IsrconStarted=true;
                }
                else{
                    this->IsrconStarted=false;
                }
            }
        }



        stdoutFileSize=line.length();
    }


}



void MainWindow::on_LaunchServerButton_clicked()
{
    if(!IsServerRunning){
        if(Data.Servers.empty()){
            QErrorMessage qmes;
            qmes.showMessage("サーバーがありません。新規サーバーで新しくサーバーを作ってください。");
            qmes.exec();
            return;
        }
        Server& s=Data.Servers[CurrentServer()];
        this->CommandLine_Server= new CommandLineController(&s,0);
        ui->LaunchServerButton->setText("サーバーストップ");
    }
    else{
        ui->LaunchServerButton->setText("サーバー起動");
        this->CommandLine_Server->ThisProcess->kill();
        remove("stdout.txt");
    }
    IsServerRunning=!IsServerRunning;
}

int MainWindow::CurrentServer(){
   return ui->comboBox_Servers->currentIndex();
}

void MainWindow::on_pushButton_ClipBoard_clicked()
{

}

void MainWindow::ClosedDelay(){

}
#include <QCloseEvent>
void MainWindow::closeEvent (QCloseEvent *event)
{
    /*サーバーごとに設定しなおす---------------------------------------------
    GUIOption g;
    g.DiscordBotToken=ui->lineEdit_DiscordBotToken->text().toStdString();
    g.DiscordChannelId=ui->lineEdit_DiscordChannelId->text().toStdString();
    g.isCommandGuess=ui->checkBox_GuessCommand->isChecked();
    */
    this->CommandLine_Server->ThisProcess->kill();
    this->Data.Write();

}




void MainWindow::ChangeServerToRender(){
    if(IsServerRunning){
        QMessageBox e;
       int Answer= QMessageBox::question(this,"確認","すでにサーバーが起動しています。終了しますか？");
        if(Answer==QMessageBox::Ok){
           on_LaunchServerButton_clicked();
       }
        else{
            return;
        }
    }
    else{

    }
}


void MainWindow::on_comboBox_Servers_currentIndexChanged(int index)
{
    ChangeServerToRender();
}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{

    if(this->Data.Servers.empty()){return;}
    if(index==1){
        qDebug()<<ui->comboBox_ServerProperties->count();
        if(ui->comboBox_ServerProperties->count()==0){
        this->Data.Servers[CurrentServer()].ServerProperty.Load(Data.Servers[CurrentServer()].Directory);
        ServerProperty sp= this->Data.Servers[CurrentServer()].ServerProperty;
            for(int i=0;i<sp.Properties.size();i++){
                ui->comboBox_ServerProperties->addItem(QString::fromStdString(sp.Properties[i][0]));
            }
        }
    }

}


void MainWindow::on_comboBox_ServerProperties_currentIndexChanged(int index)
{
    ui->lineEdit_ServerProperties->setText(QString::fromStdString(this->Data.Servers[CurrentServer()].ServerProperty.Properties[index][1]));
}


void MainWindow::on_lineEdit_ServerProperties_textChanged(const QString &arg1)
{
    if(this->Data.Servers.empty()){return;}
    this->Data.Servers[CurrentServer()].ServerProperty.Properties[ui->comboBox_ServerProperties->currentIndex()][1]=arg1.toStdString();
    this->Data.Servers[CurrentServer()].ServerProperty.Write(Data.Servers[CurrentServer()].Directory);
}

/*
void MainWindow::finished(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    QString str(bts);
    QMessageBox::information(this,"sal",str,"ok");
}*/

void MainWindow::on_pushButton_Command_clicked()
{
    Command();
}

void MainWindow::Command(){
    if(!this->IsrconStarted){
        return ErrorWindow("RCONがまだ有効ではありません。起動するまで待ってください。");
    }
    QString command=ui->lineEdit_Command->text();

    if(rcon.isconnected){
        rcon.cmd(command.toStdString());
        ui->lineEdit_Command->clear();
    }
    else{
        this->Data.Servers[CurrentServer()].ServerProperty.Load(Data.Servers[CurrentServer()].Directory);
        qDebug()<<"thinking";
        string e= this->Data.Servers[CurrentServer()].ServerProperty.Properties[8][1];
        QString enablercon=QString::fromStdString(e);
        QString rconpass= QString::fromStdString(this->Data.Servers[CurrentServer()].ServerProperty.Properties[37][1]);
        QString rconport=QString::fromStdString(this->Data.Servers[CurrentServer()].ServerProperty.Properties[38][1]);

        if(enablercon!="true"){
            return ErrorWindow("rconが有効になっていません。serverのpropertiesの「enable-rcon」をtrueにしてください。");
        }
        if(rconpass==""){
            return ErrorWindow("rconのパスワードが空白です。serverのpropertiesの「rcon-password」に何らかのパスワードを設定してください。");
        }
        bool is_ok=false;
        int port = rconport.toInt(&is_ok, 10);

        if(!is_ok){
            return ErrorWindow("サーバーのポートがある整数型にあてはまりません。serverのpropertiesの「rcon-port」に何らかのパスワードを設定してください。\n初期値の25565をおすすめします。");
        }
        qDebug()<<"port";
        qDebug()<<port;
        rcon.auth(rconpass.toStdString(),port);
    }
}

void MainWindow::ErrorWindow(string info){
    QErrorMessage e;
    e.showMessage(QString::fromStdString(info));
    e.exec();
}

void MainWindow::on_lineEdit_Command_returnPressed()
{
    Command();
}

