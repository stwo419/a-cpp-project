#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>
#include"protocol.h"

namespace Ui {
class PrivateChat;
}

class PrivateChat : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChat(QWidget *parent = nullptr);
    ~PrivateChat();

    void setPeerName(QString temp);

    void sendMsg();
    void handlePrivateChat(PDU* pdu);

    static PrivateChat &getPrivateChat();

private:
    Ui::PrivateChat *ui;
    QString m_peername;
};

#endif // PRIVATECHAT_H
