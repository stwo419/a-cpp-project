/********************************************************************************
** Form generated from reading UI file 'directoryope.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIRECTORYOPE_H
#define UI_DIRECTORYOPE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_directoryOPE
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QListWidget *list_dir;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_back;
    QPushButton *btn_createDir;
    QPushButton *btn_delDir;
    QPushButton *btn_renameFile;
    QPushButton *btn_refreashDir;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btn_upFile;
    QPushButton *btn_delFile;
    QPushButton *btn_downloadFile;
    QPushButton *btn_shareFile;

    void setupUi(QWidget *directoryOPE)
    {
        if (directoryOPE->objectName().isEmpty())
            directoryOPE->setObjectName("directoryOPE");
        directoryOPE->resize(419, 303);
        verticalLayout_3 = new QVBoxLayout(directoryOPE);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        list_dir = new QListWidget(directoryOPE);
        list_dir->setObjectName("list_dir");

        horizontalLayout->addWidget(list_dir);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        btn_back = new QPushButton(directoryOPE);
        btn_back->setObjectName("btn_back");

        verticalLayout->addWidget(btn_back);

        btn_createDir = new QPushButton(directoryOPE);
        btn_createDir->setObjectName("btn_createDir");

        verticalLayout->addWidget(btn_createDir);

        btn_delDir = new QPushButton(directoryOPE);
        btn_delDir->setObjectName("btn_delDir");

        verticalLayout->addWidget(btn_delDir);

        btn_renameFile = new QPushButton(directoryOPE);
        btn_renameFile->setObjectName("btn_renameFile");

        verticalLayout->addWidget(btn_renameFile);

        btn_refreashDir = new QPushButton(directoryOPE);
        btn_refreashDir->setObjectName("btn_refreashDir");

        verticalLayout->addWidget(btn_refreashDir);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        btn_upFile = new QPushButton(directoryOPE);
        btn_upFile->setObjectName("btn_upFile");

        verticalLayout_2->addWidget(btn_upFile);

        btn_delFile = new QPushButton(directoryOPE);
        btn_delFile->setObjectName("btn_delFile");

        verticalLayout_2->addWidget(btn_delFile);

        btn_downloadFile = new QPushButton(directoryOPE);
        btn_downloadFile->setObjectName("btn_downloadFile");

        verticalLayout_2->addWidget(btn_downloadFile);

        btn_shareFile = new QPushButton(directoryOPE);
        btn_shareFile->setObjectName("btn_shareFile");

        verticalLayout_2->addWidget(btn_shareFile);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout);


        retranslateUi(directoryOPE);

        QMetaObject::connectSlotsByName(directoryOPE);
    } // setupUi

    void retranslateUi(QWidget *directoryOPE)
    {
        directoryOPE->setWindowTitle(QCoreApplication::translate("directoryOPE", "Form", nullptr));
        btn_back->setText(QCoreApplication::translate("directoryOPE", "\350\277\224\345\233\236", nullptr));
        btn_createDir->setText(QCoreApplication::translate("directoryOPE", "\345\210\233\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        btn_delDir->setText(QCoreApplication::translate("directoryOPE", "\345\210\240\351\231\244\346\226\207\344\273\266\345\244\271", nullptr));
        btn_renameFile->setText(QCoreApplication::translate("directoryOPE", "\351\207\215\345\221\275\345\220\215\346\226\207\344\273\266", nullptr));
        btn_refreashDir->setText(QCoreApplication::translate("directoryOPE", "\345\210\267\346\226\260\346\226\207\344\273\266\345\244\271", nullptr));
        btn_upFile->setText(QCoreApplication::translate("directoryOPE", "\344\270\212\344\274\240\346\226\207\344\273\266", nullptr));
        btn_delFile->setText(QCoreApplication::translate("directoryOPE", "\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
        btn_downloadFile->setText(QCoreApplication::translate("directoryOPE", "\344\270\213\350\275\275\346\226\207\344\273\266", nullptr));
        btn_shareFile->setText(QCoreApplication::translate("directoryOPE", "\345\210\206\344\272\253\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class directoryOPE: public Ui_directoryOPE {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIRECTORYOPE_H
