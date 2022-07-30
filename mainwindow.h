#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "data.h"
#include "commandline.h"
#include <QApplication>

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

private:
    Ui::MainWindow *ui;
    Data Data;
    CommandLineController* CommandLine_Server;
    int stdoutFileSize=0;
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


