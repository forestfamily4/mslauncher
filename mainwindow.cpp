#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    this->Data.Read();
    remove("stdout.txt");
    ui->setupUi(this);
    this->CommandLine_Server=nullptr;
    timerId = startTimer(200);
    IsServerRunning=false;
    this->os=CommandLine_Server->Getos();
    ui->comboBox_lang->addItem("Language:日本語");
    ui->comboBox_lang->addItem("Language:English");
    DataRender(true);
    vector<Server> ss=this->Data.Servers;
    for(int i=0;i<ss.size();i++){
        qDebug()<<i<<"thinking";
        ui->comboBox_Servers->addItem(ss[i].ServerName);
    }
    this->isfirst=false;
}

MainWindow::~MainWindow()
{
    killTimer(timerId);
    delete ui;

}

void MainWindow::on_NewServerButton_clicked()
{
    QString ServerName=  QInputDialog::getText(this, tr("サーバーの名前"), tr("新しく作るサーバーの名前を入力してください。"));
    if(ServerName==""){return;}
    QString Directory= QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                      Data.DirHistory,
                                      QFileDialog::ShowDirsOnly
                                          | QFileDialog::DontResolveSymlinks);
    if(Directory==""){return;}
    Data.DirHistory=Directory;
    Server* s=new Server(ServerName,Directory);
    Data.Servers.push_back(*s);
    vector<Server>& Servers=Data.Servers;
    ui->comboBox_Servers->clear();
    for(int i=0;i<Servers.size();i++){
        ui->comboBox_Servers->addItem(Servers[i].ServerName);
    }
    delete s;


}

void MainWindow::timerEvent(QTimerEvent *event){
    if(rcon.data.length()!=this->rcondatasize){
        ui->plainTextEdit_CommandResult->setPlainText(QString::fromStdString(rcon.data));
    }
   this->rcondatasize= rcon.data.length();



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
                qmes.showMessage(tr("EULAに同意する必要があります。\nEULA=trueに変更して、もう一度サーバーを起動して下さい。"));
                qmes.exec();
                //windowsのnotepad
                if(this->os==0){
                    QProcess process;
                    process.start("notepad.exe",{Data.Servers[CurrentServer()].Directory+"/eula.txt"});
                    process.waitForFinished();
                }


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
            qmes.showMessage(tr("サーバーがありません。新規サーバーで新しくサーバーを作ってください。"));
            qmes.exec();
            return;
        }
        Server& s=Data.Servers[CurrentServer()];
        this->CommandLine_Server= new CommandLineController(&s,0);
        ui->LaunchServerButton->setText(tr("サーバーストップ"));
    }
    else{
        ui->LaunchServerButton->setText(tr("サーバー起動"));
        this->CommandLine_Server->kill();
        remove("stdout.txt");
    }
    IsServerRunning=!IsServerRunning;
}

int MainWindow::CurrentServer(){
    int a=ui->comboBox_Servers->currentIndex();
    if(a<0){
        a=0;
    }
    return a;
}

void MainWindow::on_pushButton_ClipBoard_clicked()
{
    DownloadManager* d=new DownloadManager();
    d->downloadcloudflared();
    //QtDownloadManager* qd=new QtDownloadManager();
    //qd->setTarget("https://github.com/cloudflare/cloudflared/releases/latest/DownloadManager/cloudflared-windows-amd64.exe");
    //qd->DownloadManager();
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
    SaveGUIOption();
    Data.langindex=ui->comboBox_lang->currentIndex();
    this->Data.Write();
    this->CommandLine_Server->kill();
}




void MainWindow::ChangeServerToRender(){

    if(IsServerRunning){
        QMessageBox e;
        int Answer= QMessageBox::question(this,tr("確認"),tr("すでにサーバーが起動しています。終了しますか？"));
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
    if(index==-1){
        return;
    }
    SaveGUIOption();
    ChangeServerToRender();
    DataRender(false);
    ServerIndex=index;
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
        return ErrorWindow(tr("RCONがまだ有効ではありません。起動するまで待ってください。"));
    }
    QString command=ui->lineEdit_Command->text();

    if(rcon.isconnected){
        rcon.cmd(command.toStdString());
        ui->lineEdit_Command->clear();
    }
    else{
        this->Data.Servers[CurrentServer()].ServerProperty.Load(Data.Servers[CurrentServer()].Directory);
        string e= this->Data.Servers[CurrentServer()].ServerProperty.Properties[8][1];
        QString enablercon=QString::fromStdString(e);
        QString rconpass= QString::fromStdString(this->Data.Servers[CurrentServer()].ServerProperty.Properties[37][1]);
        QString rconport=QString::fromStdString(this->Data.Servers[CurrentServer()].ServerProperty.Properties[38][1]);

        if(enablercon!="true"){
            return ErrorWindow(tr("RCONが有効になっていません。serverのpropertiesの「enable-rcon」をtrueにしてください。"));
        }
        if(rconpass==""){
            return ErrorWindow(tr("RCONのパスワードが空白です。serverのpropertiesの「rcon-password」に何らかのパスワードを設定してください。"));
        }
        bool is_ok=false;
        int port = rconport.toInt(&is_ok, 10);

        if(!is_ok){
            return ErrorWindow(tr("サーバーのポートがある整数型にあてはまりません。serverのpropertiesの「rcon-port」に何らかの整数を設定してください。\n初期値の25565をおすすめします。"));
        }
        qDebug()<<"port";
        qDebug()<<port;
        rcon.auth(rconpass.toStdString(),port);
    }
}

void MainWindow::ErrorWindow(QString info){
    QErrorMessage e;
    e.showMessage(info);
    e.exec();
}

void MainWindow::on_lineEdit_Command_returnPressed()
{
    Command();
}

void MainWindow::SaveGUIOption(){
   GUIOption &g= Data.Servers[ServerIndex].GUIOptions;
    g.DiscordBotToken=ui->lineEdit_DiscordBotToken->text();
   g.DiscordChannelId=ui->lineEdit_DiscordChannelId->text();
    g.isCommandGuess=ui->checkBox_GuessCommand->isChecked();
}


void MainWindow::DataRender(bool isfirst){
    vector<Server> ss=this->Data.Servers;
    Server s=Server("","");
    if(!isfirst){
    s=ss[CurrentServer()];
    }
    else{
        s=ss[0];
        ui->comboBox_lang->setCurrentIndex(Data.langindex);
    }
    GUIOption g=s.GUIOptions;


    ui->lineEdit_DiscordBotToken->setText(g.DiscordBotToken);
    ui->lineEdit_DiscordChannelId->setText(g.DiscordChannelId);
    ui->checkBox_GuessCommand->setChecked(g.isCommandGuess);
}

void MainWindow::on_comboBox_lang_currentIndexChanged(int index)
{
    if(isfirst){return;}
    ErrorWindow("言語を変えるにはこのアプリを再起動する必要があります。\nYou need to restart this app to change the language.");
}

