/********************************************************************************
** Form generated from reading UI file 'openwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENWIDGET_H
#define UI_OPENWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_openwidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QListWidget *List_;
    QStackedWidget *stack;
    QWidget *page;
    QWidget *page_2;

    void setupUi(QWidget *openwidget)
    {
        if (openwidget->objectName().isEmpty())
            openwidget->setObjectName("openwidget");
        openwidget->resize(535, 306);
        horizontalLayout_2 = new QHBoxLayout(openwidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        List_ = new QListWidget(openwidget);
        List_->setObjectName("List_");

        horizontalLayout->addWidget(List_);

        stack = new QStackedWidget(openwidget);
        stack->setObjectName("stack");
        page = new QWidget();
        page->setObjectName("page");
        stack->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stack->addWidget(page_2);

        horizontalLayout->addWidget(stack);


        horizontalLayout_2->addLayout(horizontalLayout);


        retranslateUi(openwidget);

        stack->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(openwidget);
    } // setupUi

    void retranslateUi(QWidget *openwidget)
    {
        openwidget->setWindowTitle(QCoreApplication::translate("openwidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class openwidget: public Ui_openwidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENWIDGET_H
