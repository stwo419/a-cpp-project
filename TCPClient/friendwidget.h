#ifndef FRIENDWIDGET_H
#define FRIENDWIDGET_H

#include <QWidget>
#include"online.h"
#include"protocol.h"
#include"privatechat.h"
#include<QListWidget>

namespace Ui {
class friendWidget;
}

class friendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit friendWidget(QWidget *parent = nullptr);
    ~friendWidget();

    online *getonline();

    QList<QListWidgetItem*> getListFriendItemList();

    void showonlinesend();
    void handleShowRespond(PDU* pdu);

    void searchUser();
    QString m_searchname;
    void handleSearchRespond(PDU* pdu);

    void reFreashFriend();
    void handleRefreashFriend(PDU* pdu);

    void deleteFriend();
    void handleDelFriendRespond(PDU* pdu);

    void privateChat();

    void GroupChat();
    void handleGroupChat(PDU* pdu);

private:
    Ui::friendWidget *ui;

    online* m_onlineUser;
};

#endif // FRIENDWIDGET_H
