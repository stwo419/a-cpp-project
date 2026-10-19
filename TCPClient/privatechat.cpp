#include "privatechat.h"
#include "ui_privatechat.h"
#include <QMessageBox>
#include"protocol.h"
#include"tcpclient.h"

PrivateChat::PrivateChat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrivateChat)
{
    ui->setupUi(this);
    ui->tt_view->setEnabled(false);

    connect(ui->tt_inoutBox,&QLineEdit::returnPressed,this,&PrivateChat::sendMsg);

    connect(ui->btn_sendMsg,&QPushButton::clicked,this,&PrivateChat::sendMsg);
}

PrivateChat::~PrivateChat()
{
    delete ui;
}

void PrivateChat::setPeerName(QString temp)
{
    this->m_peername=temp;
    this->setWindowTitle(this->m_peername);
}

void PrivateChat::sendMsg()
{
    QString msg=ui->tt_inoutBox->text();
    if(msg.isEmpty())
    {
        QMessageBox::warning(this, "私聊", "发送的消息不能为空");
        return;
    }
    QByteArray temp=msg.toUtf8();
    PDU* pdu=mkPDU(temp.size()+1);
    pdu->uiMsgType=ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST;
    strncpy(pdu->caData,this->m_peername.toStdString().c_str(),this->m_peername.size());
    strncpy((char *)pdu->caMsg,temp.constData(),temp.size());
    TcpClient::getTcpclient().getTcpsocket().write((char* )pdu,pdu->uiPDULen);

    ui->tt_view->append(TcpClient::getTcpclient().getusername()+":"+msg);
    ui->tt_inoutBox->clear();
    free(pdu);
    pdu=nullptr;
}

void PrivateChat::handlePrivateChat(PDU *pdu)
{
    QString peername(pdu->caData);
    QString msg=QString::fromUtf8(reinterpret_cast<const char *>(pdu->caMsg));

    if(this->isHidden())
    {
        this->setHidden(false);
    }
    else if(this->m_peername!=peername)
    {
        ui->tt_view->clear();
        this->setHidden(true);
        this->setHidden(false);
        QMessageBox::information(this,"info","聊天已经切换，目前聊天人:"+peername);
    }
    this->setPeerName(peername);
    ui->tt_view->append(peername+":"+msg);
}

PrivateChat &PrivateChat::getPrivateChat()
{
    static PrivateChat instance;
    return instance;
}
