#include "online.h"
#include "ui_online.h"
#include<QMessageBox>
#include"protocol.h"
#include"tcpclient.h"
#include <QToolTip>

online::online(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::online)
{
    ui->setupUi(this);
    ui->List_Online->setFixedHeight(75);
    connect(ui->btn_addFriend,&QPushButton::clicked,this,&online::addFriend);

}

online::~online()
{
    delete ui;
}

void online::showuser(QStringList &list)
{
    ui->List_Online->clear();
    for(int i=0;i<list.size();++i)
    {
        QListWidgetItem* item=new QListWidgetItem;
        if(list.at(i)!=TcpClient::getTcpclient().getusername())
        {
            item->setForeground(QBrush(Qt::green));
        }
        item->setText(list.at(i));
        ui->List_Online->addItem(item);
    }
}

void online::addFriend()
{
    if(ui->List_Online->currentItem()==nullptr)
    {
        QMessageBox::information(this,"attention","请选择用户");
        return;
    }
    QString name=ui->List_Online->currentItem()->text();
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    strncpy(pdu->caData,name.toStdString().c_str(),name.length());
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    QToolTip::showText(QCursor::pos(),"增加好友消息已发出，等待对方确认",this);
    free(pdu);
    pdu=nullptr;
}

void online::addFriendRequest(PDU* pdu)
{
    char mname[32]={'\0'},pname[32]={'\0'};
    QString myname,peername;
    strncpy(pname,pdu->caData,32);
    strncpy(mname,(char *)pdu->caData+32,32);
    myname=mname;
    peername=pname;
    qDebug()<<myname<<peername;
    PDU* pduback=mkPDU(0);
    if(QMessageBox::Yes==QMessageBox::information(this,"friend apply",peername+"请求加你为好友",QMessageBox::Yes,QMessageBox::No))
        pduback->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_AGREE;
    else
        pduback->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_DISAGREE;
    strncpy((char *)pduback->caData,myname.toStdString().c_str(),myname.length());
    strncpy((char *)pduback->caData+32,peername.toStdString().c_str(),peername.length());

    TcpClient::getTcpclient().getTcpsocket().write((char *)pduback,pduback->uiPDULen);
    free(pduback);
    pduback=nullptr;
}

void online::addFriendRespond(PDU* pdu)
{
    QString data=pdu->caData;
    //qDebug()<<data;
    if(data==UNKNOWN_ERROR)
        QMessageBox::critical(this,"error","出现未知错误，无法添加好友");
    else if(data==ADDFRI_USER_NOT_EXIST)
        QMessageBox::information(this,"attention","该用户不存在或者已经注销账号，无法添加好友");
    else if(data==ADDFRI_USER_OFFLINE)
        QMessageBox::information(this,"attention","该用户已经下线，无法添加好友");
    else if(data==ADDFRI_FRIEND_EXIST)
        QMessageBox::information(this,"attention","该用户是你的好友，无需添加好友");
    else if(data==ADDFRI_AGGRE)
        QMessageBox::information(this,"attention","对方同意添加好友，添加成功");
    else if(data==ADDFRI_DISAGGRE)
        QMessageBox::information(this,"attention","对方不同意添加好友，添加失败");
    TcpClient::getTcpclient().showFriend();

}
