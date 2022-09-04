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
    this->os=os::Getos();
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
    if(!Data.Servers.empty()){
        ui->label_ServerDirectory->setText(Data.Servers[0].Directory);
        ui->comboBox_ServerType->setCurrentIndex((int)CurrentServer().ServerType);
    }
    ui->comboBox_ServerType->addItems({"official","mohist","spigot","paper","fabric","forge"});
    ui->comboBox_ServerType->setEnabled(false);
    java(Data.isJavainmslauncher);


    
    
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
    Server* s=new Server(ServerName,Directory,"server.jar",ServerType::official);
    Data.Servers.push_back(*s);
    vector<Server>& Servers=Data.Servers;
    ui->comboBox_Servers->clear();
    for(int i=0;i<Servers.size();i++){
        ui->comboBox_Servers->addItem(Servers[i].ServerName);
    }
    delete s;

    this->ServerTypeComobobox=new ComboBoxWindow(this,{"official","mohist","spigot","papermc","fabric","forge"});
    //connect(ServerTypeComobobox,SIGNAL(finished()),this,SLOT(ServerTypeComoboboxfinished()));
    connect(ServerTypeComobobox,&ComboBoxWindow::finished,[=](){
        int a=this->Data.Servers.size()-1;
        Server& s=this->Data.Servers[a];
        s.SetServerType(this->ServerTypeComobobox->result);
    });
    ServerTypeComobobox->setFixedSize(250,70);
    ServerTypeComobobox->setWindowFlag(Qt::WindowCloseButtonHint,false);
    ServerTypeComobobox->setWindowTitle(tr("サーバーの種類を決めてください。"));
    ServerTypeComobobox->show();
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
                    process.start("notepad.exe",{CurrentServer().Directory+"/eula.txt"});
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
                    Server s=CurrentServer();
                    s.LoadProperties();
                    QString port=s.ServerProperty.Get("query.port");
                    this->CommandLine_cloudflared->Command(&s,1,port);
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
            QRegExp r("https://.*trycloudflare.com");
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

    qDebug()<<java::versionint();
    if(!IsServerRunning){
        if(Data.Servers.empty()){
            QErrorMessage qmes;
            qmes.showMessage(tr("サーバーがありません。新規サーバーで新しくサーバーを作ってください。"));
            qmes.exec();
            return;
        }
        bool test=java::hasjava();
        if(!test)
        {
            if(os::Getos()==0){
                QMessageBox e;
                int Answer= QMessageBox::question(this,tr("確認"),tr("Javaがインストールされていない、もしくは環境変数の設定がうまくいってないようです。(環境変数を変えたばかりだと反映されない可能性があります。) mslauncherのフォルダにそのままJREをインストールすることもできますが、どうしますか？"));
                qDebug()<<Answer<<"Anser";
                if(Answer==QMessageBox::Yes){
                    java::downloadjdk();
                    this->Data.SetisJavainmslauncher(true);
                    return;
                }
                else{
                    return;
                }
            }else{
                return ErrorWindow(tr("Javaの存在が確かめられませんでした。"));
            }

        }
        if(java::versionint()<17){
            ErrorWindow(tr("javaのバージョンが低いためサーバーが起動できない可能性があります。JRE17以上のバージョンをおすすめします。"));
        }

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
                return ErrorWindow(tr("cloudflaredがインストールされていません。"));
            }
        }

        Server s=CurrentServer();
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

Server MainWindow::CurrentServer(){
    int a=ui->comboBox_Servers->currentIndex();
    if(a<0){
        a=0;
    }
    return Data.Servers[a];
}
int MainWindow::CurrentServerIndex(){
    return ui->comboBox_Servers->currentIndex();
}
void MainWindow::on_pushButton_ClipBoard_clicked()
{
    QClipboard* c= QApplication::clipboard();
    c->setText(this->cloudflaredlink);
    //java::downloadjdk();
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
            Server s=this->CurrentServer();
            s.LoadProperties();
        ServerProperty sp= s.ServerProperty;
            for(int i=0;i<sp.Properties.size();i++){
            QString a=sp.Properties[i][0];
                ui->comboBox_ServerProperties->addItem(a);
            }
        }
    }
    ui->checkBox_isjavainmslauncher->setChecked(Data.isJavainmslauncher);
}


void MainWindow::on_comboBox_ServerProperties_currentIndexChanged(int index)
{
    Server s=this->CurrentServer();
    s.LoadProperties();
    ui->lineEdit_ServerProperties->setText(s.ServerProperty.Properties[index][1]);
}

void MainWindow::on_comboBox_ServerProperties_currentTextChanged(const QString &arg1)
{

}

void MainWindow::on_lineEdit_ServerProperties_textChanged(const QString &arg1)
{

}

void MainWindow::on_pushButton_SaveProperty_clicked()
{
    if(this->Data.Servers.empty()){return;}
    Server& s=Data.Servers[CurrentServerIndex()];
    s.LoadProperties();
    s.ServerProperty.Properties[s.ServerProperty.GetIndex(ui->comboBox_ServerProperties->currentText())][1]=ui->lineEdit_ServerProperties->text();
    s.ServerProperty.Write(CurrentServer().Directory);
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
    if(this->Data.Servers.empty()){
        return;
    }
    QString command=ui->lineEdit_Command->text();

    if(rcon.isconnected){
        rcon.cmd(command.toStdString());
        ui->lineEdit_Command->clear();
    }
    else{
        Server s=CurrentServer();
        s.LoadProperties();
        QString enablercon=(s.ServerProperty.Get("enable-rcon"));
        QString rconpass= (s.ServerProperty.Get("rcon.password"));
        QString rconport=(s.ServerProperty.Get("rcon.port"));
        if(!this->IsServerRunning){
            return ErrorWindow(tr("サーバーが起動していません。"));
        }
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
        if(!this->IsrconStarted){
            return ErrorWindow(tr("RCONがまだ有効になっていません。もう少し待ってください。"));
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
    g.isCommandGuess=ui->checkBox_GuessCommand->isChecked();
}


void MainWindow::DataRender(bool isfirst){
    if(this->Data.Servers.empty()){
        return;
    }
    vector<Server> ss=this->Data.Servers;
    Server s=Server("","","",ServerType::official);
    if(!isfirst){
    s=CurrentServer();
    }
    else{
        s=ss[0];
        ui->comboBox_lang->setCurrentIndex(Data.langindex);
    }
    GUIOption g=s.GUIOptions;


    ui->checkBox_GuessCommand->setChecked(g.isCommandGuess);
}

void MainWindow::on_comboBox_lang_currentIndexChanged(int index)
{
    if(isfirst){return;}
    ErrorWindow("言語を変えるにはこのアプリを再起動する必要があります。\nYou need to restart this app to change the language.");
}

void MainWindow::on_pushButton_DeleteServer_clicked()
{
    if(this->Data.Servers.empty()){
        return;
    }
    QMessageBox e;
    int Answer= QMessageBox::question(this,tr("確認"),tr("サーバーを本当に削除してもいいですか？この操作は取り消せません。"));
    if(Answer==QMessageBox::No){
        return;
    }
    vector<Server>& v=this->Data.Servers;
    if(v.empty()){
        return;
    }
    v.erase(cbegin(v) + ui->comboBox_Servers->currentIndex());
    ui->comboBox_Servers->clear();
    for(int i=0;i<v.size();i++){
        ui->comboBox_Servers->addItem(v[i].ServerName);
    }
}


void MainWindow::on_pushButton_OpenDirectory_clicked()
{
    if(Data.Servers.empty()){
        return;
    }
    Server s=CurrentServer();
    const QFileInfo fileInfo(s.Directory);
    // Mac, Windows support folder or file.
    if (os::Getos()==0) {

        QStringList param;
        if (!fileInfo.isDir())
            param += QLatin1String("/select,");
        param += QDir::toNativeSeparators(fileInfo.canonicalFilePath());
        QProcess::startDetached("explorer", param);
    } else if (os::Getos()==1) {
        QStringList scriptArgs;
        scriptArgs << QLatin1String("-e")
                   << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
                          .arg(fileInfo.canonicalFilePath());
        QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
        scriptArgs.clear();
        scriptArgs << QLatin1String("-e")
                   << QLatin1String("tell application \"Finder\" to activate");
        QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    } else {
        ErrorWindow(tr("対応していません。"));
    }
}


void MainWindow::on_checkBox_isjavainmslauncher_stateChanged(int arg1)
{
    Data.SetisJavainmslauncher(ui->checkBox_isjavainmslauncher->isChecked());
}





