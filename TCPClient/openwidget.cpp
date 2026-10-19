#include "openwidget.h"
#include "ui_openwidget.h"
#include <qstackedwidget.h>
#include<QHBoxLayout>
#include<QPushButton>
#include"tcpclient.h"

openwidget::openwidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::openwidget)
{
    ui->setupUi(this);
    ui->List_->addItem("好友");
    ui->List_->addItem("文件");
    ui->List_->setFixedWidth(75);

    this->m_FriendW=new friendWidget;
    this->m_dirOpe=new directoryOPE;

    while(ui->stack->count()>0)
    {
        QWidget* w=ui->stack->widget(0);
        ui->stack->removeWidget(w);
    }
    ui->stack->addWidget(this->m_FriendW);
    // qDebug()<<ui->stack->indexOf(this->m_FriendW);
    ui->stack->addWidget(this->m_dirOpe);

    ui->stack->setCurrentWidget(this->m_FriendW);
    this->setWindowTitle(TcpClient::getTcpclient().getusername());


    // QStackedWidget* stach=new QStackedWidget;
    // stach->addWidget(this->m_FriendW);

    // QHBoxLayout* hbl=new QHBoxLayout;
    // hbl->addWidget(ui->List_);
    // hbl->addWidget(stach);

    // setLayout(hbl);

    connect(ui->List_,&QListWidget::currentRowChanged,ui->stack,&QStackedWidget::setCurrentIndex);

    connect(ui->List_,&QListWidget::currentRowChanged,[=](){
        this->getDIROPE()->refreashDir();
    });
}

openwidget::~openwidget()
{
    delete ui;
}

friendWidget *openwidget::getfriendWidget()
{
    return this->m_FriendW;
}

openwidget &openwidget::getopewidget()
{
    static openwidget instance;
    return instance;
}

directoryOPE *openwidget::getDIROPE()
{
    return this->m_dirOpe;
}
