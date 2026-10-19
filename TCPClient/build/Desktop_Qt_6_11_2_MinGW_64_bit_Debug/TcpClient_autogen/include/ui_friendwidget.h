/********************************************************************************
** Form generated from reading UI file 'friendwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDWIDGET_H
#define UI_FRIENDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_friendWidget
{
public:
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *pmain;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QTextEdit *text_;
    QListWidget *listfriend;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_delFriend;
    QPushButton *btn_freashFriend;
    QPushButton *btn_shwoOnlineUser;
    QPushButton *btn_serchUser;
    QPushButton *btn_privateChat;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit;
    QPushButton *btn_sendMsg;

    void setupUi(QWidget *friendWidget)
    {
        if (friendWidget->objectName().isEmpty())
            friendWidget->setObjectName("friendWidget");
        friendWidget->resize(705, 319);
        horizontalLayout_4 = new QHBoxLayout(friendWidget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        pmain = new QVBoxLayout();
        pmain->setObjectName("pmain");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        text_ = new QTextEdit(friendWidget);
        text_->setObjectName("text_");

        horizontalLayout->addWidget(text_);

        listfriend = new QListWidget(friendWidget);
        listfriend->setObjectName("listfriend");

        horizontalLayout->addWidget(listfriend);


        horizontalLayout_3->addLayout(horizontalLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        btn_delFriend = new QPushButton(friendWidget);
        btn_delFriend->setObjectName("btn_delFriend");

        verticalLayout->addWidget(btn_delFriend);

        btn_freashFriend = new QPushButton(friendWidget);
        btn_freashFriend->setObjectName("btn_freashFriend");

        verticalLayout->addWidget(btn_freashFriend);

        btn_shwoOnlineUser = new QPushButton(friendWidget);
        btn_shwoOnlineUser->setObjectName("btn_shwoOnlineUser");

        verticalLayout->addWidget(btn_shwoOnlineUser);

        btn_serchUser = new QPushButton(friendWidget);
        btn_serchUser->setObjectName("btn_serchUser");

        verticalLayout->addWidget(btn_serchUser);

        btn_privateChat = new QPushButton(friendWidget);
        btn_privateChat->setObjectName("btn_privateChat");

        verticalLayout->addWidget(btn_privateChat);


        horizontalLayout_3->addLayout(verticalLayout);


        pmain->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        lineEdit = new QLineEdit(friendWidget);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout_2->addWidget(lineEdit);

        btn_sendMsg = new QPushButton(friendWidget);
        btn_sendMsg->setObjectName("btn_sendMsg");

        horizontalLayout_2->addWidget(btn_sendMsg);


        pmain->addLayout(horizontalLayout_2);


        horizontalLayout_4->addLayout(pmain);


        retranslateUi(friendWidget);

        QMetaObject::connectSlotsByName(friendWidget);
    } // setupUi

    void retranslateUi(QWidget *friendWidget)
    {
        friendWidget->setWindowTitle(QCoreApplication::translate("friendWidget", "Form", nullptr));
        btn_delFriend->setText(QCoreApplication::translate("friendWidget", "\345\210\240\351\231\244\345\245\275\345\217\213", nullptr));
        btn_freashFriend->setText(QCoreApplication::translate("friendWidget", "\345\210\267\346\226\260\345\245\275\345\217\213", nullptr));
        btn_shwoOnlineUser->setText(QCoreApplication::translate("friendWidget", "\346\230\276\347\244\272\345\234\250\347\272\277\347\224\250\346\210\267", nullptr));
        btn_serchUser->setText(QCoreApplication::translate("friendWidget", "\346\237\245\346\211\276\347\224\250\346\210\267", nullptr));
        btn_privateChat->setText(QCoreApplication::translate("friendWidget", "\347\247\201\350\201\212", nullptr));
        btn_sendMsg->setText(QCoreApplication::translate("friendWidget", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class friendWidget: public Ui_friendWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDWIDGET_H
