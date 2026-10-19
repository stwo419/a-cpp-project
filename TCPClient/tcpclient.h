#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMainWindow>
#include <QFile>
#include<QTcpSocket>
#include <QByteArray>
#include"protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class TcpClient;
}
QT_END_NAMESPACE

class TcpClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = nullptr);
    ~TcpClient() override;

    void LoadConfig();  //得到服务端ip和端口

    void registerRespd(PDU* pdu);
    void LogInRespd(PDU* pdu);

    PDU* getTcpPDU();

    static TcpClient &getTcpclient();
    QTcpSocket& getTcpsocket();

    QString getusername();

    void onreadyDetach();

signals:
    void showFriend();

    void showDir();

private slots:
    void on_btn_register_2_clicked();

    void handleResp(PDU* pdu);
    void on_btn_sign_up_clicked();


private:
    void setTcpPDU();

    Ui::TcpClient *ui;
    QString m_IP;
    quint16 m_PORT;
    QTcpSocket m_TcpSock;
    QByteArray m_buffer;
    PDU* pdu;

    QString m_name;
};
#endif // TCPCLIENT_H
