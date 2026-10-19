#ifndef ONLINE_H
#define ONLINE_H

#include <QWidget>
#include "protocol.h"


namespace Ui {
class online;
}

class online : public QWidget
{
    Q_OBJECT

public:
    explicit online(QWidget *parent = nullptr);
    ~online();
    void showuser(QStringList &list);
public slots:
    void addFriend();
    void addFriendRequest(PDU* pdu);
    void addFriendRespond(PDU* pdu);
private:
    Ui::online *ui;
};

#endif // ONLINE_H
