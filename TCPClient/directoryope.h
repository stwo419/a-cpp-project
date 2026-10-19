#ifndef DIRECTORYOPE_H
#define DIRECTORYOPE_H

#include <QWidget>
#include"protocol.h"
#include"share.h"

const int FILE_TYPE_ROLE=Qt::UserRole+1;

namespace Ui {
class directoryOPE;
}

class directoryOPE : public QWidget
{
    Q_OBJECT

public:
    explicit directoryOPE(QWidget *parent = nullptr);
    ~directoryOPE();

    void createDir();
    void handleCreateDir(PDU* pdu);

    void refreashDir();
    void handleRefreashDir(PDU* pdu);

    void delDir();
    void handleDelDir(PDU* pdu);

    void delFile();
    void handleDelFile(PDU* pdu);

    void reName();
    void handleReName(PDU *pdu);

    void enterDir(const QModelIndex &index);

    void back();

    void upload();
    void handleUpload(PDU *pdu);

    void downLoad();
    void handleDownLoad(PDU* Pdu);
    bool getIsDownLoad(){return this->m_isDownload;}
    QString getDownLoadPath(){return this->m_downLoadPath;}
    void DownLoadFileData();

    void initShare();
    void chareedFile();

    share& getShare(){return *this->m_shareToFriend;}
private:
    Ui::directoryOPE *ui;
    QString m_currentDir;

    bool m_isUpload;
    QString m_upFilePath;
    QString m_upFileName;

    QString m_downLoadPath;
    QString m_downLoadName;
    long long m_downFileSize;
    long long m_hasDownFileSize;

    bool m_isDownload;

    share* m_shareToFriend;
};

#endif // DIRECTORYOPE_H
