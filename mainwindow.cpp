#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->CommandLine_Server=new CommandLineController();
    this->CommandLine_cloudflared=new CommandLineController();
    this->Data.Read();
    remove("stdout.txt");
    ui->setupUi(this);
    timerId = startTimer(200);
    IsServerRunning=false;
    this->os=CommandLine_Server->Getos();
    ui->comboBox_lang->addItem("Language:日本語");
    ui->comboBox_lang->addItem("Language:English");
    DataRender(true);
    vector<Server> ss=this->Data.Servers;
    for(int i=0;i<ss.size();i++){
        ui->comboBox_Servers->addItem(ss[i].ServerName);
    }
    this->isfirst=false;

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

    ComboBoxWindow* c=new ComboBoxWindow(this,{"official","mohist","papermc","fabric","forge"});
    c->setFixedSize(250,70);
    c->setWindowFlag(Qt::WindowCloseButtonHint,false);
    c->setWindowTitle(tr("サーバーの種類を決めてください。"));
    c->show();
}

void MainWindow::timerEvent(QTimerEvent *event){
    if(rcon.data.length()!=this->rcondatasize){
        ui->plainTextEdit_CommandResult->setPlainText(QString::fromStdString(rcon.data));
    }
   this->rcondatasize= rcon.data.length();


   if(CommandLine_Server->isrunning){
   if(QFile::exists(CommandLine_Server->stdoutfilepath)){
       QFile f=QFile(CommandLine_Server->stdoutfilepath);
       f.open(QIODevice::ReadOnly);
       QString line= f.readAll();
        ui->label_CommandLine->setText(line);

        if(stdoutFileSize!=line.length()){
            if(line.contains("You need to agree to the EULA")){
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
                if(line.contains("RCON")){
                    this->IsrconStarted=true;
                }
                else{
                    this->IsrconStarted=false;
                }

                if(line.contains("Done")){
                    if(this->CommandLine_cloudflared->isrunning==true){
                        return;
                    }
                    this->CommandLine_cloudflared->kill();
                    this->CommandLine_cloudflared=new CommandLineController();
                    Server& s=Data.Servers[CurrentServer()];
                    s.LoadProperties();
                    string port=s.ServerProperty.Get("query.port");
                    this->CommandLine_cloudflared->Command(&s,1,QString::fromStdString(port));
                }
            }

        }



        stdoutFileSize=line.length();
        f.close();
    }

   if(QFile(this->CommandLine_cloudflared->stdoutfilepath).exists()&&!this->iscloudflaredlinkexists){
        QFile f=QFile(CommandLine_cloudflared->stdoutfilepath);
        f.open(QIODevice::ReadOnly);
        QString line= f.readAll();
        if(line.contains("Your quick Tunnel has been created!")){
            int index=line.indexOf("Your quick Tunnel has been created!");
            QString a=line.mid(index,300);
            qDebug()<<a<<"cloudflare";
            QRegExp r("https://.*\.trycloudflare.com");
            r.indexIn(a);
            QString result= r.cap(0).mid(8);
            this->cloudflaredlink=result;
            ui->label_cloudflared->setText(result);
            this->iscloudflaredlinkexists=true;
        }
    }
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
        this->CommandLine_Server= new CommandLineController();
        this->CommandLine_Server->Command(&s,0);
        ui->LaunchServerButton->setText(tr("サーバーストップ"));
    }
    else{
        ui->LaunchServerButton->setText(tr("サーバー起動"));
        this->CommandLine_Server->kill();
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
    QClipboard* c= QApplication::clipboard();
    c->setText(this->cloudflaredlink);
    Forge* f=new Forge();
    f->Get("latest");
}

void MainWindow::ClosedDelay(){

}
void MainWindow::closeEvent (QCloseEvent *event)
{
    QDir d("temp");
    if(d.exists()){
        d.removeRecursively();
    }
    QDir a;
    a.mkdir("temp");

    SaveGUIOption();
    Data.langindex=ui->comboBox_lang->currentIndex();
    this->Data.Write();
    this->CommandLine_cloudflared->kill();
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
        if(ui->comboBox_ServerProperties->count()==0){
            this->Data.Servers[CurrentServer()].LoadProperties();
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
    /*if(this->Data.Servers[CurrentServer()].ServerProperty.Get("enable-rcon")!="true"){
        return ErrorWindow(tr("RCONが有効でありません。サーバーのプロパティの「enable-rcon」をtrueにしてください。"));
    }
    if(!this->IsrconStarted){
        return ErrorWindow(tr("RCONがまだ有効ではありません。起動するまで待ってください。"));
    }*/
    QString command=ui->lineEdit_Command->text();

    if(rcon.isconnected){
        rcon.cmd(command.toStdString());
        ui->lineEdit_Command->clear();
    }
    else{
        this->Data.Servers[CurrentServer()].LoadProperties();
        string e= this->Data.Servers[CurrentServer()].ServerProperty.Properties[8][1];
        QString enablercon=QString::fromStdString(e);
        QString rconpass= QString::fromStdString(this->Data.Servers[CurrentServer()].ServerProperty.Get("rcon.password"));
        QString rconport=QString::fromStdString(this->Data.Servers[CurrentServer()].ServerProperty.Get("rcon.port"));

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

void MainWindow::on_pushButton_DeleteServer_clicked()
{
    vector<Server>& v=this->Data.Servers;
    if(v.empty()){
        return;
    }
    v.erase(cbegin(v) + CurrentServer());
    ui->comboBox_Servers->clear();
    for(int i=0;i<v.size();i++){
        ui->comboBox_Servers->addItem(v[i].ServerName);
    }
}

