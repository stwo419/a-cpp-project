#ifndef OPENWIDGET_H
#define OPENWIDGET_H

#include <QWidget>
#include"friendwidget.h"
#include"directoryope.h"

namespace Ui {
class openwidget;
}

class openwidget : public QWidget
{
    Q_OBJECT

public:
    explicit openwidget(QWidget *parent = nullptr);
    ~openwidget();
    friendWidget* getfriendWidget();

    static openwidget &getopewidget();

    directoryOPE* getDIROPE();
private:
    Ui::openwidget *ui;
    friendWidget* m_FriendW;
    directoryOPE* m_dirOpe;
};

#endif // OPENWIDGET_H
