#include "share.h"
#include "ui_share.h"
#include<QAbstractItemView>
#include<QList>
#include<QMessageBox>
#include<QDebug>
#include"tcpclient.h"

share::share(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::share)
{
    ui->setupUi(this);
    ui->m_lw_sharedObject->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->label->setEnabled(false);

    connect(ui->m_btn_cancel,&QPushButton::clicked,this,[=](){this->setHidden(true);});
    connect(ui->m_btn_ok,&QPushButton::clicked,this,&share::share_confirm);
    connect(ui->m_btn_chooseAll,&QPushButton::clicked,this,[=](){ui->m_lw_sharedObject->selectAll();});
    connect(ui->m_btn_delChoose,&QPushButton::clicked,this,[=](){ui->m_lw_sharedObject->clearSelection();});

}

share::~share()
{
    delete ui;
}

void share::init()
{
    ui->m_lw_sharedObject->clear();
    //拿到好友列表
    int itemSize=this->m_LsShareObj.size();
    qDebug()<<"item count:"<<itemSize;
    QListWidgetItem* item=nullptr;
    for(int i=0;i<itemSize;++i)
    {
        //qDebug()<<"item name"<<this->m_LsShareObj.at(i)->text();
        item=this->m_LsShareObj.at(i);
        ui->m_lw_sharedObject->addItem(item);
    }
}


void share::share_confirm()
{
    QList<QListWidgetItem*> lsChooseObj;
    lsChooseObj=ui->m_lw_sharedObject->selectedItems();
    if(lsChooseObj.size()==0)
    {
        QMessageBox::warning(this,"文件分享","请选择你要分享的文件对象");
        return;
    }
//qDebug()<<"share file path(server):"<<this->m_shareFilePath<<lsChooseObj;
    int shareCount=lsChooseObj.size();
    PDU* pdu=mkPDU(64+shareCount*32+1);     //前64个存分享的文件路径
    pdu->uiMsgType=ENUM_DIR_TYPE_SHAREFILE_REQUEST;
    QString username=TcpClient::getTcpclient().getusername();
    std::string strCount=std::to_string(shareCount);
    strncpy(pdu->caData,username.toStdString().c_str(),username.size());
    strncpy(pdu->caData+48,strCount.c_str(),strCount.size());
    memcpy((char *)pdu->caMsg,this->m_shareFilePath.toStdString().c_str(),this->m_shareFilePath.size());
    int i=0;
    for(QListWidgetItem* item:lsChooseObj)
    {
        QString target=item->text();
        memcpy((char *)pdu->caMsg+64+(i++)*32,target.toStdString().c_str(),target.size());
    }
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
    this->setHidden(true);
}

void share::askIfAcceptShare(PDU *Pdu)
{
    QString data(Pdu->caData);
    QString sendPeople=data.right(data.size()-2);
    sendPeople=sendPeople.left(data.indexOf("/"));
    QString fileName=data.right(data.size()-data.lastIndexOf("/")-1);
//qDebug()<<sendPeople<<fileName;
    QString question="用户 "+sendPeople+" 向你分享文件 "+fileName+" 请问是否确认接收";
    if(QMessageBox::No==QMessageBox::question(this,"文件分享",question))
    {
        return;
    }
    PDU* pduSend=mkPDU(0);
    pduSend->uiMsgType=ENUM_DIR_TYPE_IF_SHAREFILE_RESPOND;
    strncpy(pduSend->caData,data.toStdString().c_str(),data.size());
    TcpClient::getTcpclient().getTcpsocket().write((char *)pduSend,pduSend->uiPDULen);
    free(pduSend);
    pduSend=nullptr;
}

void share::handelSharFile(PDU *Pdu)
{
    QString resp(Pdu->caData);
    if(resp==SHAREFILE_SUCCESS)
        QMessageBox::information(this,"共享文件","文件接收成功");
    else
        QMessageBox::critical(this,"共享文件","文件接收失败");
}
