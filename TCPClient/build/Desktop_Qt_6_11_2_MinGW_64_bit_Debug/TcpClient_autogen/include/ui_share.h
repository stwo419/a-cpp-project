/********************************************************************************
** Form generated from reading UI file 'share.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHARE_H
#define UI_SHARE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_share
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *m_btn_chooseAll;
    QPushButton *m_btn_delChoose;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QListWidget *m_lw_sharedObject;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *m_btn_ok;
    QPushButton *m_btn_cancel;

    void setupUi(QWidget *share)
    {
        if (share->objectName().isEmpty())
            share->setObjectName("share");
        share->resize(499, 375);
        horizontalLayout_3 = new QHBoxLayout(share);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        m_btn_chooseAll = new QPushButton(share);
        m_btn_chooseAll->setObjectName("m_btn_chooseAll");
        QFont font;
        font.setPointSize(12);
        m_btn_chooseAll->setFont(font);

        horizontalLayout->addWidget(m_btn_chooseAll);

        m_btn_delChoose = new QPushButton(share);
        m_btn_delChoose->setObjectName("m_btn_delChoose");
        m_btn_delChoose->setFont(font);

        horizontalLayout->addWidget(m_btn_delChoose);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        label = new QLabel(share);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        m_lw_sharedObject = new QListWidget(share);
        m_lw_sharedObject->setObjectName("m_lw_sharedObject");

        verticalLayout->addWidget(m_lw_sharedObject);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        m_btn_ok = new QPushButton(share);
        m_btn_ok->setObjectName("m_btn_ok");
        m_btn_ok->setFont(font);

        horizontalLayout_2->addWidget(m_btn_ok);

        m_btn_cancel = new QPushButton(share);
        m_btn_cancel->setObjectName("m_btn_cancel");
        m_btn_cancel->setFont(font);

        horizontalLayout_2->addWidget(m_btn_cancel);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);


        retranslateUi(share);

        QMetaObject::connectSlotsByName(share);
    } // setupUi

    void retranslateUi(QWidget *share)
    {
        share->setWindowTitle(QCoreApplication::translate("share", "Form", nullptr));
        m_btn_chooseAll->setText(QCoreApplication::translate("share", "\345\205\250\351\200\211", nullptr));
        m_btn_delChoose->setText(QCoreApplication::translate("share", "\345\217\226\346\266\210\351\200\211\346\213\251", nullptr));
        label->setText(QCoreApplication::translate("share", "\347\273\277\350\211\262\344\270\272\345\234\250\347\272\277\357\274\214\347\272\242\350\211\262\344\270\272\347\246\273\347\272\277", nullptr));
        m_btn_ok->setText(QCoreApplication::translate("share", "\347\241\256\345\256\232", nullptr));
        m_btn_cancel->setText(QCoreApplication::translate("share", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class share: public Ui_share {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHARE_H
