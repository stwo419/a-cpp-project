/********************************************************************************
** Form generated from reading UI file 'tcpclient.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENT_H
#define UI_TCPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpClient
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *edi_username;
    QLabel *label_2;
    QLineEdit *edi_pwd;
    QPushButton *btn_logIn;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QPushButton *btn_register_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_logout;

    void setupUi(QMainWindow *TcpClient)
    {
        if (TcpClient->objectName().isEmpty())
            TcpClient->setObjectName("TcpClient");
        TcpClient->resize(526, 316);
        centralwidget = new QWidget(TcpClient);
        centralwidget->setObjectName("centralwidget");
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName("gridLayout_3");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setAlignment(Qt::AlignmentFlag::AlignJustify|Qt::AlignmentFlag::AlignVCenter);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(24);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        edi_username = new QLineEdit(groupBox);
        edi_username->setObjectName("edi_username");
        QFont font1;
        font1.setPointSize(22);
        edi_username->setFont(font1);

        gridLayout->addWidget(edi_username, 0, 1, 2, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 1, 0, 2, 1);

        edi_pwd = new QLineEdit(groupBox);
        edi_pwd->setObjectName("edi_pwd");
        edi_pwd->setFont(font1);
        edi_pwd->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout->addWidget(edi_pwd, 2, 1, 1, 1);

        btn_logIn = new QPushButton(groupBox);
        btn_logIn->setObjectName("btn_logIn");
        btn_logIn->setFont(font);

        gridLayout->addWidget(btn_logIn, 3, 0, 1, 2);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setAlignment(Qt::AlignmentFlag::AlignJustify|Qt::AlignmentFlag::AlignVCenter);
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName("gridLayout_2");
        btn_register_2 = new QPushButton(groupBox_2);
        btn_register_2->setObjectName("btn_register_2");
        QFont font2;
        font2.setPointSize(14);
        btn_register_2->setFont(font2);

        gridLayout_2->addWidget(btn_register_2, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 1, 1, 1);

        btn_logout = new QPushButton(groupBox_2);
        btn_logout->setObjectName("btn_logout");
        btn_logout->setFont(font2);

        gridLayout_2->addWidget(btn_logout, 0, 2, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 1, 0, 1, 1);

        TcpClient->setCentralWidget(centralwidget);

        retranslateUi(TcpClient);

        QMetaObject::connectSlotsByName(TcpClient);
    } // setupUi

    void retranslateUi(QMainWindow *TcpClient)
    {
        TcpClient->setWindowTitle(QCoreApplication::translate("TcpClient", "TcpClient", nullptr));
        groupBox->setTitle(QString());
        label->setText(QCoreApplication::translate("TcpClient", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("TcpClient", "\345\257\206    \347\240\201\357\274\232", nullptr));
        btn_logIn->setText(QCoreApplication::translate("TcpClient", "\347\231\273\345\275\225", nullptr));
        groupBox_2->setTitle(QString());
        btn_register_2->setText(QCoreApplication::translate("TcpClient", "\346\263\250\345\206\214", nullptr));
        btn_logout->setText(QCoreApplication::translate("TcpClient", "\346\263\250\351\224\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpClient: public Ui_TcpClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H
