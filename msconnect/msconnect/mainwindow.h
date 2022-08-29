#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commandline.h"

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
    void on_pushButton_clicked();

private:
    int timerId;
    Ui::MainWindow *ui;
    void ErrorWindow(QString info);
    void timerEvent(QTimerEvent *event);
    void closeEvent (QCloseEvent *event);
    CommandLineController* cloudflare;
    QString cloudflarestdouttemp="";
    bool iscloudflareconnecting=false;
};
#endif // MAINWINDOW_H
