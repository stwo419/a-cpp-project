#include "friendwidget.h"
#include "ui_friendwidget.h"
#include"tcpclient.h"
#include<QInputDialog>
#include <QMessageBox>
#include <QToolTip>

friendWidget::friendWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::friendWidget)
{
    ui->setupUi(this);
    this->m_searchname=0;
    this->m_onlineUser=new online();
    ui->pmain->addWidget(this->m_onlineUser);
    this->m_onlineUser->hide();
    ui->listfriend->setFixedWidth(100);
    this->ui->text_->setEnabled(false);

    connect(ui->btn_shwoOnlineUser,&QPushButton::clicked,[=](){
        if(this->m_onlineUser->isHidden())
        {
            this->m_onlineUser->setHidden(false);
            ui->btn_shwoOnlineUser->setText("关闭在线人数");
        }
        else
        {
            this->m_onlineUser->setHidden(true);
            ui->btn_shwoOnlineUser->setText("显示在线人数");
        }
    });

    connect(ui->btn_shwoOnlineUser,&QPushButton::clicked,this,&friendWidget::showonlinesend);

    connect(ui->btn_serchUser,&QPushButton::clicked,this,&friendWidget::searchUser);

    connect(ui->btn_freashFriend,&QPushButton::clicked,this,&friendWidget::reFreashFriend);

    connect(ui->btn_delFriend,&QPushButton::clicked,this,&friendWidget::deleteFriend);

    connect(ui->btn_privateChat,&QPushButton::clicked,this,&friendWidget::privateChat);

    connect(ui->btn_sendMsg,&QPushButton::clicked,this,&friendWidget::GroupChat);

    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&friendWidget::GroupChat);

    connect(&TcpClient::getTcpclient(),&TcpClient::showFriend,this,&friendWidget::reFreashFriend);
}

friendWidget::~friendWidget()
{
    delete ui;
}

online *friendWidget::getonline()
{
    return this->m_onlineUser;
}

QList<QListWidgetItem *> friendWidget::getListFriendItemList()
{
    QList<QListWidgetItem *> ret;
    QListWidgetItem item;
    int count=ui->listfriend->count();
    for(int i=0;i<count;++i)
    {
        item=*(ui->listfriend->item(i));
        ret.push_back(new QListWidgetItem(item));
    }
    return ret;
}

void friendWidget::showonlinesend()
{
    if(!(ui->btn_shwoOnlineUser->text()=="关闭在线人数"))
        return;
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_ALLONLINE_REQUEST;

    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void friendWidget::handleShowRespond(PDU* pdu)
{
    int n=pdu->uiMsgLen/32;
    QStringList l;
    for(int i=0;i<n;++i)
    {
        QString temp=QString::fromLatin1((char *)pdu->caMsg+i*32,32);
        // qDebug()<<temp;
        int pos=temp.indexOf('\0');
        temp=temp.left(pos-1);
        // qDebug()<<temp;
        l.push_back(temp);
    }
    this->m_onlineUser->showuser(l);
}

void friendWidget::searchUser()
{
    this->m_searchname=QInputDialog::getText(this,"search","username:");
    if(this->m_searchname.toStdWString().empty())
    {
        //QMessageBox::critical(this,"error","用户名不能为空");
        return;
    }
    // qDebug()<<this->m_searchname;
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_SEARCH_REQUEST;
    std::strncpy(pdu->caData,this->m_searchname.toStdString().c_str(),this->m_searchname.length());
    //qDebug()<<pdu->caData;
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);

    free(pdu);
    pdu=nullptr;
}

void friendWidget::handleSearchRespond(PDU* pdu)
{
    QString data=pdu->caData;
    //qDebug()<<data;
    if(data==SEARCH_NO_SUCH_USER)
        QMessageBox::information(this,"result",SEARCH_NO_SUCH_USER);
    else if(data==SEARCH_USER_OFFLINE)
        QMessageBox::information(this,"result",SEARCH_USER_OFFLINE);
    else if(data==SEARCH_USER_ONLINE)
        QMessageBox::information(this,"result",SEARCH_USER_ONLINE);
}

void friendWidget::reFreashFriend()
{
    QString name=TcpClient::getTcpclient().getusername();
    if(name.isEmpty())
    {
        QMessageBox::critical(this,"error","unknown error");
        return;
    }
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_REFREASHFRIEND_REQUEST;
    strncpy(pdu->caData,name.toStdString().c_str(),name.length());
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void friendWidget::handleRefreashFriend(PDU* pdu)
{
    QList<QPair<QString,QString>> list;
    int n=pdu->uiMsgLen/33;
    for(int i=0;i<n;++i)
    {
        QString name((char *)pdu->caMsg+i*33);
        QString stat=QString::fromLatin1((char *)pdu->caMsg+i*33+32,1);
        list.push_back(QPair<QString,QString>(name,stat));
    }
    std::sort(list.begin(),list.end(),[](QPair<QString, QString> p1, QPair<QString, QString> p2){
        if(p1.second=="0"&&p2.second=="1")
        {
            return false;
        }
        else if(p1.second=="1"&&p2.second=="0")
            return true;
        return false;
    });
    ui->listfriend->clear();
    for(int i=0;i<list.size();++i)
    {
        QListWidgetItem* item=new QListWidgetItem;
        item->setText(list.at(i).first);
        if(list.at(i).second=="0")
        {
            item->setForeground(QBrush(Qt::red));
        }
        else
        {
            item->setForeground(QBrush(Qt::green));
        }
        ui->listfriend->addItem(item);
    }
}

void friendWidget::deleteFriend()
{
    QString peername;
    if(ui->listfriend->currentItem()==nullptr)
    {
        QMessageBox::critical(this,"error","请从好友列表选择要删除的好友");
        return;
    }
    peername=ui->listfriend->currentItem()->text();
    if(QMessageBox::No==QMessageBox::question(this,"提示","是否删除好友:"+peername,QMessageBox::Yes|QMessageBox::No,QMessageBox::No))
    {
        return;
    }
    //确定删除好友;
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_DELETEFRIEND_REQUEST;
    strncpy((char *)pdu->caData,peername.toStdString().c_str(),peername.length());
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void friendWidget::handleDelFriendRespond(PDU *pdu)
{
    QString result(pdu->caData);
    if(result==DELETE_FRIEND_SUCCESS)
        QToolTip::showText(QCursor::pos(),"删除好友成功",this);
    else
        QToolTip::showText(QCursor::pos(),"删除好友失败",this);
    TcpClient::getTcpclient().showFriend();
}

void friendWidget::privateChat()
{
    if(ui->listfriend->currentItem()==nullptr)
    {
        QMessageBox::warning(this,"waining","请选择需要私聊的对象");
        return;
    }
    QString peername=ui->listfriend->currentItem()->text();
    PrivateChat::getPrivateChat().setPeerName(peername);
    // qDebug()<<peername;
    if(PrivateChat::getPrivateChat().isHidden())
        PrivateChat::getPrivateChat().show();
    else
        QMessageBox::warning(this,"warning","请先关闭已经打开的私聊框");
}

void friendWidget::GroupChat()
{
    QString msg=ui->lineEdit->text();
    QString myname=TcpClient::getTcpclient().getusername();
    if(msg.isEmpty())
    {
        QMessageBox::warning(this, "群聊", "发送的消息不能为空");
        return;
    }
    QByteArray temp=msg.toUtf8();
    PDU* pdu=mkPDU(temp.size()+1);
    pdu->uiMsgType=ENUM_MSG_TYPE_GROUP_CHAT_REQUEST;
    strncpy(pdu->caData,myname.toStdString().c_str(),myname.size());
    strncpy((char *)pdu->caMsg,temp.constData(),temp.size());
    qDebug()<<(char *)pdu->caMsg;
    TcpClient::getTcpclient().getTcpsocket().write((char* )pdu,pdu->uiPDULen);

    ui->text_->append(TcpClient::getTcpclient().getusername()+":"+msg);
    ui->lineEdit->clear();
    free(pdu);
    pdu=nullptr;
}

void friendWidget::handleGroupChat(PDU *pdu)
{
    QString peername(pdu->caData);
    QString msg=QString::fromUtf8(reinterpret_cast<const char *>(pdu->caMsg));

    ui->text_->append(peername+":"+msg);
}
