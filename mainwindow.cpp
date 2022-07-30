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
    this->Data.Write();
    this->CommandLine_Server->ThisProcess->kill();
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
    if(index==1){
        this->Data.Servers[CurrentServer()].ServerProperty.Load(Data.Servers[CurrentServer()].Directory);
        ServerProperty sp= this->Data.Servers[CurrentServer()].ServerProperty;
        QWidget* s= ui->tab_2;
        cout<<"thinking"<<endl;
        for(int i=0;sp.Properties.size();i++){


            QFormLayout* layout=ui->formLayout_serverproperty;
            QLabel* q;
            layout->addWidget(q);
            string aa=sp.Properties[i][0];
            cout<<aa<<endl;
            q->setText(QString::fromStdString(aa));
            q->setGeometry(i*30,i*30,100,30);
            QLineEdit* l;
            layout->addWidget(l);
            string aaa=sp.Properties[i][1];
            l->setText(QString::fromStdString(aaa));
            l->setGeometry(i*30+100,i*30,100,30);
        }
    }
}

