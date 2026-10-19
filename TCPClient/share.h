#ifndef SHARE_H
#define SHARE_H

#include <QWidget>
#include<QString>
#include<QListWidgetItem>
#include "protocol.h"

namespace Ui {
class share;
}

class share : public QWidget
{
    Q_OBJECT

public:
    explicit share(QWidget *parent = nullptr);
    ~share();

    void init();
    void setSharedFilePath(QString &path){this->m_shareFilePath=path;}
    void setSharedObj(QList<QListWidgetItem*> Obj){this->m_LsShareObj=Obj;}
    void share_confirm();
    void askIfAcceptShare(PDU* Pdu);
    void handelSharFile(PDU *Pdu);


private:
    Ui::share *ui;

    QString m_shareFilePath;

    QList<QListWidgetItem*> m_LsShareObj;
};

#endif // SHARE_H
