/********************************************************************************
** Form generated from reading UI file 'online.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONLINE_H
#define UI_ONLINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_online
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QListWidget *List_Online;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QPushButton *btn_addFriend;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *online)
    {
        if (online->objectName().isEmpty())
            online->setObjectName("online");
        online->resize(368, 306);
        horizontalLayout_2 = new QHBoxLayout(online);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        List_Online = new QListWidget(online);
        List_Online->setObjectName("List_Online");

        horizontalLayout->addWidget(List_Online);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        btn_addFriend = new QPushButton(online);
        btn_addFriend->setObjectName("btn_addFriend");
        QFont font;
        font.setPointSize(18);
        btn_addFriend->setFont(font);

        verticalLayout->addWidget(btn_addFriend);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(online);

        QMetaObject::connectSlotsByName(online);
    } // setupUi

    void retranslateUi(QWidget *online)
    {
        online->setWindowTitle(QCoreApplication::translate("online", "Form", nullptr));
        btn_addFriend->setText(QCoreApplication::translate("online", "\345\212\240\345\245\275\345\217\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class online: public Ui_online {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONLINE_H
