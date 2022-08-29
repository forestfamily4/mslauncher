#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("icon.ico"));
    MainWindow w;
    w.setWindowFlag(Qt::WindowMaximizeButtonHint,false);
    w.show();
    return a.exec();
}
