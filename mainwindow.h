#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data.h"
#include "commandline.h"
#include <QApplication>
#include "rcon.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
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

private:
    Ui::MainWindow *ui;
    Data Data;
    CommandLineController* CommandLine_Server;
    int stdoutFileSize=0;
    rcon rcon;
    void Command();
    void ErrorWindow(string info);
public:
    int CurrentServer();
    int timerId;
protected:
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *bar);
    void ClosedDelay();
    QTimer* closedtimer;
    void ChangeServerToRender();
    bool IsServerRunning;
};
#endif // MAINWINDOW_H


