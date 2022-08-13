#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>
#include "data/data.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    Data* d=new Data();
    d->Read();
    qDebug()<<"dfaf";
    qDebug()<<d->langindex;
    if(d->langindex==0){
        translator.load("lang/ja");
    }
    else{
        translator.load("lang/en");
    }
    a.installTranslator(&translator);
    a.setWindowIcon(QIcon("icon.ico"));

    MainWindow w;

    w.show();
    delete d;
    return a.exec();
}
