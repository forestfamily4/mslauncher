#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>

#include "data/data.h"
#include "back/commandline.h"
#include "back/rcon.h"
#include "back/downloadmanager.h"
#include "servertype/forge.h"
#include "ui/comboboxwindow.h"
#include "back/os.h"
#include "back/java.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QInputDialog>
#include <QFileDialog>
#include <fstream>
#include <QTimer>
#include <QErrorMessage>
#include <QProcess>
#include <QMessageBox>
#include <QRegExp>
#include <QClipboard>
#include <QCloseEvent>
#include <QScrollbar>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_NewServerButton_clicked();

    void on_LaunchServerButton_clicked();

    void on_pushButton_ClipBoard_clicked();

    void on_comboBox_Servers_currentIndexChanged(int index);

    void on_tabWidget_tabBarClicked(int index);

    void on_comboBox_ServerProperties_currentIndexChanged(int index);

    void on_lineEdit_ServerProperties_textChanged(const QString &arg1);

    void on_pushButton_Command_clicked();

    void on_lineEdit_Command_returnPressed();

    void on_comboBox_lang_currentIndexChanged(int index);

    void on_pushButton_DeleteServer_clicked();
    
    void on_pushButton_OpenDirectory_clicked();

    void on_checkBox_isjavainmslauncher_stateChanged(int arg1);

    void on_comboBox_ServerProperties_currentTextChanged(const QString &arg1);

    void on_pushButton_SaveProperty_clicked();

private:
    Ui::MainWindow *ui;
    Data Data;
    CommandLineController *CommandLine_Server;
    CommandLineController *CommandLine_cloudflared;
    int os = 0;
    rcon rcon;
    QString cloudflaredlink = "";

    void Command();
    void ErrorWindow(QString info);
    void SaveGUIOption();
    void DataRender(bool isfirst);

    // temp value
    int ServerIndex = 0;
    int stdoutFileSize = 0;
    bool IsrconStarted = false;
    int rcondatasize = 0;
    bool isfirst = true;
    bool iscloudflaredlinkexists = false;
    ComboBoxWindow *ServerTypeComobobox;
    bool scrolllate;//scrollするときの一時的
    bool scrolllate2;//scrollするときの一時的

    QString test = "";

public:
    Server *CurrentServer();
    int CurrentServerIndex();
    int timerId;

protected:
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *bar);
    void ClosedDelay();
    QTimer *closedtimer;
    void ChangeServerToRender();
    bool IsServerRunning;
};
#endif // MAINWINDOW_H
