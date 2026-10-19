#include "tcpclient.h"
#include "ui_tcpclient.h"
#include<QMessageBox>
#include<QDebug>
#include<QByteArray>
#include "openwidget.h"


TcpClient::TcpClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    pdu=nullptr;
    this->setFixedSize(500,300);
    this->setWindowTitle("登录");

    this->LoadConfig(); //加载配置文件
    //连接服务器
    this->m_TcpSock.connectToHost(QHostAddress(this->m_IP),this->m_PORT);
    connect(&this->m_TcpSock,&QTcpSocket::connected,[=](){
        qDebug()<<"连接成功";
    });

    //拆包
    connect(&this->m_TcpSock,&QTcpSocket::readyRead,this,&TcpClient::onreadyDetach);

    connect(ui->btn_logIn,&QPushButton::clicked,this,&TcpClient::on_btn_sign_up_clicked);

    connect(ui->edi_pwd,&QLineEdit::returnPressed,this,&TcpClient::on_btn_sign_up_clicked);

    connect(ui->edi_username,&QLineEdit::returnPressed,this,&TcpClient::on_btn_sign_up_clicked);
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::LoadConfig()    //得到服务端ip和端口
{
    QFile file(":/Client.config");
    if(file.open(QIODeviceBase::ReadOnly))
    {
        QByteArray byData=file.readAll();
        QString strData=byData.toStdString().c_str();
        QStringList list = strData.split("\r\n");
        //qDebug()<<list;
        this->m_IP=list[0].toStdString().c_str();
        this->m_PORT=list[1].toShort();
        //qDebug()<<this->m_IP<<this->m_PORT;
    }
    else
    {
        QMessageBox::critical(this,"File open","File open failed");
    }
}

void TcpClient::on_btn_register_2_clicked()
{
    QString username=ui->edi_username->text();
    QString password=ui->edi_pwd->text();
    if(username.isEmpty()||password.isEmpty())  //如果用户名或者密码为空就不继续执行程序
    {
        QMessageBox::information(this,"注册失败","用户名和密码不能为空");
        return;
    }
    // qDebug()<<username;
    // qDebug()<<password;
    PDU* pdu=mkPDU(0);
    strncpy(pdu->caData,username.toStdString().c_str(),32);
    strncpy(pdu->caData+32,password.toStdString().c_str(),32);

    // for(int i=0;i<64;++i)
    //     qDebug()<<pdu->caData[i];

    pdu->uiMsgType=ENUM_MSG_TYPE_REGIST_REQUEST;
    this->m_TcpSock.write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void TcpClient::registerRespd(PDU* pdu)
{
    QString ret=pdu->caData;
    if(ret==REGISTER_SUCCESS)
        QMessageBox::information(this,"register info",REGISTER_SUCCESS);
    else
        QMessageBox::critical(this,"register error",REGISTER_FAILED);
}

 void TcpClient::LogInRespd(PDU* pdu)
{
    QString data(pdu->caData);
    if(data==LOGIN_SUCCESS)
    {
        ui->edi_username->setReadOnly(true);
        ui->edi_pwd->setReadOnly(true);
        qDebug()<<"登录成功";
        openwidget::getopewidget().show();
        //connect(&TcpClient::getTcpclient(),&TcpClient::showFriend,openwidget::getopewidget().getfriendWidget(),&friendWidget::reFreashFriend);
        this->hide();
        openwidget::getopewidget().getfriendWidget()->reFreashFriend();
        // openwidget::getopewidget().getDIROPE()->refreashDir();
    }
    else if(data==LOGIN_FAILED)
    {
        QMessageBox::critical(this,"登录错误","登录失败，账号或密码错误或账号已登录");
    }
}

TcpClient &TcpClient::getTcpclient()
{
    static TcpClient instance;
    return instance;
}

QTcpSocket &TcpClient::getTcpsocket()
{
    return this->m_TcpSock;
}

QString TcpClient::getusername()
{
    return this->m_name;
}


void TcpClient::handleResp(PDU* pdu)
{
    switch(pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_RESPOND:
        //qDebug()<<pdu->caData;
        this->registerRespd(pdu);
        break;
    case ENUM_MSG_TYPE_LOGIN_RESPOND:
        this->LogInRespd(pdu);
    default:
        break;
    }
}

void TcpClient::on_btn_sign_up_clicked()
{
    //qDebug()<<"login";
    QString username=ui->edi_username->text();
    QString password=ui->edi_pwd->text();
    this->m_name=username;
    if(username.isEmpty()||password.isEmpty())  //如果用户名或者密码为空就不继续执行程序
    {
        QMessageBox::information(this," 登录失败","用户名和密码不能为空");
        return;
    }
    // qDebug()<<username;
    // qDebug()<<password;
    PDU* pdu=mkPDU(0);
    strncpy(pdu->caData,username.toStdString().c_str(),32);
    strncpy(pdu->caData+32,password.toStdString().c_str(),32);

    // for(int i=0;i<64;++i)
    //     qDebug()<<pdu->caData[i];

    pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_REQUEST;
    this->m_TcpSock.write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;

}

PDU *TcpClient::getTcpPDU()
{
    return this->pdu;
}

void TcpClient::setTcpPDU()
{
    uint PDULen=0;
    char *data=(char *)&PDULen;
    int hasrecv=0;
    int fullrecv=sizeof(uint);
    int n=0;
    int remain=fullrecv-hasrecv;
    while(hasrecv<fullrecv)
    {
        remain=fullrecv-hasrecv;
        n=this->m_TcpSock.read(data+hasrecv,remain);
        if(n>0)
            hasrecv+=n;
        if(hasrecv>=fullrecv)
            break;
    }
    PDULen=*(int*)data;

    this->pdu=mkPDU(PDULen-sizeof(PDU));
    fullrecv=PDULen;
    remain=fullrecv-hasrecv;
    while(hasrecv<fullrecv)
    {
        remain=fullrecv-hasrecv;
        n=this->m_TcpSock.read((char *)pdu+hasrecv,remain);
        if(n>0)
            hasrecv+=n;
        if(hasrecv>=fullrecv)
            break;
    }

    pdu->uiPDULen=PDULen;
}

void TcpClient::onreadyDetach()
{
    if(this->m_TcpSock.bytesAvailable()==0)
    {
        qDebug()<<"m_TcpSock.bytesAvailable()==0";
        return;
    }
    if(!openwidget::getopewidget().getDIROPE()->getIsDownLoad())
    {
        this->setTcpPDU();
        PDU* pdu=this->getTcpPDU();
        qDebug()<<"msgtype"<<pdu->uiMsgType;
        switch(pdu->uiMsgType)
        {
        case ENUM_MSG_TYPE_LOGIN_RESPOND:
        case ENUM_MSG_TYPE_REGIST_RESPOND:
            this->handleResp(pdu);
            break;
        case ENUM_MSG_TYPE_ALLONLINE_RESPOND:
            openwidget::getopewidget().getfriendWidget()->handleShowRespond(pdu);
            break;
        case ENUM_MSG_TYPE_SEARCH_RESPOND:
            openwidget::getopewidget().getfriendWidget()->handleSearchRespond(pdu);
            break;
        case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
            openwidget::getopewidget().getfriendWidget()->getonline()->addFriendRequest(pdu);
            break;
        case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:
            openwidget::getopewidget().getfriendWidget()->getonline()->addFriendRespond(pdu);
            break;
        case ENUM_MSG_TYPE_REFREASHFRIEND_RESPOND:
            openwidget::getopewidget().getfriendWidget()->handleRefreashFriend(pdu);
            break;
        case ENUM_MSG_TYPE_DELETEFRIEND_RESPOND:
            openwidget::getopewidget().getfriendWidget()->handleDelFriendRespond(pdu);
            break;
        case ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND:
            PrivateChat::getPrivateChat().handlePrivateChat(pdu);
            break;
        case ENUM_MSG_TYPE_GROUP_CHAT_RESPOND:
            openwidget::getopewidget().getfriendWidget()->handleGroupChat(pdu);
            break;
        case ENUM_DIR_TYPE_CREATE_DIR_RESPOND:
            openwidget::getopewidget().getDIROPE()->handleCreateDir(pdu);
            break;
        case ENUM_DIR_TYPE_REFREASH_DIR_RESPOND:
            openwidget::getopewidget().getDIROPE()->handleRefreashDir(pdu);
            break;
        case ENUM_DIR_TYPE_DELETE_DIR_RESPOND:
            openwidget::getopewidget().getDIROPE()->handleDelDir(pdu);
            break;
        case ENUM_DIR_TYPE_DELETE_FILE_RESPOND:
            openwidget::getopewidget().getDIROPE()->handleDelFile(pdu);
            break;
        case ENUM_DIR_TYPE_RENAME_RESPOND:
            openwidget::getopewidget().getDIROPE()->handleReName(pdu);
            break;
        case ENUM_DIR_TYPE_UPLOAD_RESPOND:
            openwidget::getopewidget().getDIROPE()->handleUpload(pdu);
            break;
        case ENUM_DIR_TYPE_DOWNLOAD_RESPOND:
            openwidget::getopewidget().getDIROPE()->handleDownLoad(pdu);
            break;
        case ENUM_DIR_TYPE_IF_SHAREFILE_REQUEST:
            openwidget::getopewidget().getDIROPE()->getShare().askIfAcceptShare(pdu);
            break;
        case ENUM_DIR_TYPE_SHAREFILE_RESPOND:
            openwidget::getopewidget().getDIROPE()->getShare().handelSharFile(pdu);
            break;
        default:
            qDebug()<<"没有相关槽函数";
        }
        free(pdu);
        pdu=nullptr;
    }
    else
    {
        openwidget::getopewidget().getDIROPE()->DownLoadFileData();
    }
}

