#include "comboboxwindow.h"
#include <QHBoxLayout>
#include <QPushButton>

ComboBoxWindow::ComboBoxWindow(QWidget *parent,QStringList str)
    :QDialog(parent)
{
    QHBoxLayout* l=new QHBoxLayout();

    c=new QComboBox();
    c->addItems(str);
    QPushButton* p=new QPushButton("OK");

    connect(p,SIGNAL(clicked()),this,SLOT(ok()));

    l->addWidget(c);
    l->addWidget(p);

    this->setLayout(l);
}

void ComboBoxWindow::ok(){
    this->result=c->currentText();
    emit finish();
}
