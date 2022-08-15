#ifndef COMBOBOXWINDOW_H
#define COMBOBOXWINDOW_H

#include <QObject>
#include <QComboBox>
#include <QDialog>

class ComboBoxWindow:public QDialog
{
    Q_OBJECT
public:
    ComboBoxWindow(QWidget *parent = nullptr,QStringList str={""});
    QString result="";
public slots:
    void ok();
signals:
    void finish();

private:
    QComboBox* c;
};

#endif // COMBOBOXWINDOW_H
