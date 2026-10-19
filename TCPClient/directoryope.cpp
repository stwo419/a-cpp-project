#include "directoryope.h"
#include "ui_directoryope.h"
#include"tcpclient.h"
#include<QDebug>
#include<QInputDialog>
#include<QMessageBox>
#include<QTimer>
#include<QToolTip>
#include<QFileDialog>
#include"openwidget.h"

directoryOPE::directoryOPE(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::directoryOPE)
{
    ui->setupUi(this);
    this->m_currentDir="./"+TcpClient::getTcpclient().getusername();
    qDebug()<<this->m_currentDir;

    this->m_isUpload=false;
    this->m_isDownload=false;
    this->m_downFileSize=0;
    this->m_hasDownFileSize=0;
    this->m_shareToFriend=new share;

    connect(ui->btn_createDir,&QPushButton::clicked,this,&directoryOPE::createDir);

    connect(ui->btn_refreashDir,&QPushButton::clicked,this,&directoryOPE::refreashDir);

    connect(&TcpClient::getTcpclient(),&TcpClient::showDir,this,&directoryOPE::refreashDir);

    connect(ui->btn_delDir,&QPushButton::clicked,this,&directoryOPE::delDir);

    connect(ui->btn_delFile,&QPushButton::clicked,this,&directoryOPE::delFile);

    connect(ui->btn_renameFile,&QPushButton::clicked,this,&directoryOPE::reName);

    void (QListWidget::*dbclic)(const QModelIndex &)=&QListWidget::doubleClicked;
    void (directoryOPE::*m_enterDir)(const QModelIndex &)=&directoryOPE::enterDir;
    connect(ui->list_dir,dbclic,this,m_enterDir);

    connect(ui->btn_back,&QPushButton::clicked,this,&directoryOPE::back);

    connect(ui->btn_upFile,&QPushButton::clicked,this,&directoryOPE::upload);

    connect(ui->btn_downloadFile,&QPushButton::clicked,this,&directoryOPE::downLoad);

    connect(ui->btn_shareFile,&QPushButton::clicked,this,[=](){openwidget::getopewidget().getfriendWidget()->reFreashFriend();});
    connect(ui->btn_shareFile,&QPushButton::clicked,this,&directoryOPE::initShare);
    connect(ui->btn_shareFile,&QPushButton::clicked,this,&directoryOPE::chareedFile);
}

directoryOPE::~directoryOPE()
{
    delete ui;
}

void directoryOPE::createDir()
{
    bool ok=false;
    QString newDirName=QInputDialog::getText(this,"新建文件夹","文件夹名：",QLineEdit::Normal,"",&ok);
    if(!ok)
        return;
    if(newDirName.isEmpty())
    {
        QMessageBox::warning(this,"新建文件夹","新建的文件夹不能为空");
        return;
    }
    QString currentPath=this->m_currentDir;
    PDU* pdu=mkPDU(currentPath.size()+1);
    strncpy((char *)pdu->caMsg,currentPath.toStdString().c_str(),currentPath.size());
    strncpy(pdu->caData,newDirName.toStdString().c_str(),newDirName.size());
    pdu->uiMsgType=ENUM_DIR_TYPE_CREATE_DIR_REQUEST;
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void directoryOPE::handleCreateDir(PDU *pdu)
{
    QString data(pdu->caData);
    if(data==DIR_CREATE_DIR_SUCCESS)
    {
        QMessageBox::information(this,"新建文件夹","文件夹创建成功");
        //emit this->ui->btn_refreashDir->click();
        //emit TcpClient::getTcpclient().showDir();
        QTimer::singleShot(0, this, [=]{
            TcpClient::getTcpclient().showDir();
        });
    }
    else if(data==DIR_CREATE_DIR_FAILED)
    {
        QMessageBox::critical(this,"新建文件夹","文件夹创建失败");
    }
    else
    {
        QMessageBox::warning(this,"新建文件夹","文件夹已存在");
    }
}

void directoryOPE::refreashDir()
{
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_DIR_TYPE_REFREASH_DIR_REQUEST;
    strncpy(pdu->caData,this->m_currentDir.toStdString().c_str(),this->m_currentDir.size());
    TcpClient::getTcpclient().getTcpsocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void directoryOPE::handleRefreashDir(PDU *pdu)
{
    ui->list_dir->clear();
    QString name;
    QString isdir;
    QList<QListWidgetItem*> dirset;
    int n=pdu->uiMsgLen/32;
    for(int i=0;i<n;++i)
    {
        name=(char *)pdu->caMsg+i*32;
        if(name=="."||name=="..")
            continue;
        QListWidgetItem* item=new QListWidgetItem;
        isdir=QString::fromLatin1((char *)pdu->caMsg+i*32+31,1);
        item->setText(name);
        if(isdir=="1")
        {
            item->setData(FILE_TYPE_ROLE,ENUMFILE_TYPE_DIR);
            item->setForeground(Qt::yellow);
            item->setIcon(QIcon(":/resource/file.png"));
        }
        else
        {
            item->setData(FILE_TYPE_ROLE,ENUMFILE_TYPE_PRI_FILE);
            item->setForeground(Qt::white);
            item->setIcon(QIcon(":/resource/docum.png"));
        }
        dirset.push_back(item);
    }


    int nn=dirset.size();
    for(int i=0;i<nn;++i)
    {
        QListWidgetItem* item=dirset.at(i);
        if(item->data(FILE_TYPE_ROLE)==ENUMFILE_TYPE_DIR)   //是目录
            ui->list_dir->addItem(item);
    }
    for(int i=0;i<nn;++i)
    {
        QListWidgetItem* item=dirset.at(i);
        if(item->data(FILE_TYPE_ROLE)!=ENUMFILE_TYPE_DIR)   //是目录
            ui->list_dir->addItem(item);
    }
}

void directoryOPE::delDir()
{
    QListWidgetItem* curItem=ui->list_dir->currentItem();
    if(curItem==nullptr)
    {
        QMessageBox::warning(this,"删除文件夹","请选择要删除的文件夹");
        return;
    }
    if(curItem->data(FILE_TYPE_ROLE).toInt()==ENUMFILE_TYPE_PRI_FILE)
    {
         QMessageBox::warning(this,"删除文件夹","请选择文件夹");
        return;
    }
    if(QMessageBox::No==QMessageBox::question(this,"请确认","是否真的删除文件，一但删除无法恢复！！！",QMessageBox::Yes|QMessageBox::No,QMessageBox::No))
    {
        return;
    }
    QString name=curItem->text();
    PDU* pdu=mkPDU(this->m_currentDir.size()+1);
    pdu->uiMsgType=ENUM_DIR_TYPE_DELITE_DIR_REQUEST;
    strncpy((char *)pdu->caMsg,this->m_currentDir.toStdString().c_str(),this->m_currentDir.size());
    strncpy((char *)pdu->caData,name.toStdString().c_str(),name.size());
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void directoryOPE::handleDelDir(PDU *pdu)
{
    QString data(pdu->caData);
    if(data==DEL_DIR_FILE_SUCCESS)
    {
        QMessageBox::information(this,"删除文件夹","文件夹删除成功");
    }
    else
    {
        QMessageBox::critical(this,"删除文件夹","文件夹删除失败,部分文件无法删除，请稍后重试");
    }
    //this->refreashDir();
    //TcpClient::getTcpclient().showDir();
    QTimer::singleShot(0, this, [=]{
        TcpClient::getTcpclient().showDir();
    });
}

void directoryOPE::delFile()
{
    QListWidgetItem* curItem=ui->list_dir->currentItem();
    if(curItem==nullptr)
    {
        QMessageBox::warning(this,"删除文件","请选择要删除的文件");
        return;
    }
    if(curItem->data(FILE_TYPE_ROLE).toInt()==ENUMFILE_TYPE_DIR)
    {
        QMessageBox::warning(this,"删除文件","请选择文件");
        return;
    }
    if(QMessageBox::No==QMessageBox::question(this,"请确认","是否真的删除文件，一但删除无法恢复！！！",QMessageBox::Yes|QMessageBox::No,QMessageBox::No))
    {
        return;
    }
    QString name=curItem->text();
    PDU* pdu=mkPDU(this->m_currentDir.size()+1);
    pdu->uiMsgType=ENUM_DIR_TYPE_DELITE_FILE_REQUEST;
    strncpy((char *)pdu->caMsg,this->m_currentDir.toStdString().c_str(),this->m_currentDir.size());
    strncpy((char *)pdu->caData,name.toStdString().c_str(),name.size());
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void directoryOPE::handleDelFile(PDU *pdu)
{
    QString data(pdu->caData);
    if(data==DEL_DIR_FILE_SUCCESS)
    {
        QMessageBox::information(this,"删除文件","文件删除成功");
    }
    else
    {
        QMessageBox::critical(this,"删除文件","文件删除失败");
    }
    //this->refreashDir();
    QTimer::singleShot(0, this, [=]{
        TcpClient::getTcpclient().showDir();
    });
    //TcpClient::getTcpclient().showDir();
}

void directoryOPE::reName()
{
    QListWidgetItem* curItem=ui->list_dir->currentItem();
    if(curItem==nullptr)
    {
        QMessageBox::warning(this,"重命名","请选择需要重命名的文件或文件夹");
        return;
    }

    QString oldName(curItem->text());
    bool ok=false;
    QString newName=QInputDialog::getText(this,"重命名","请输入新名字：",QLineEdit::Normal,"",&ok);
    if(!ok)
        return;
    if(newName.isEmpty())
    {
        QMessageBox::warning(this,"重命名","新名字不能为空");
        return;
    }
    PDU* pdu=mkPDU(this->m_currentDir.size()+1);
    pdu->uiMsgType=ENUM_DIR_TYPE_RENAME_REQUEST;
    strncpy((char *)pdu->caData,oldName.toStdString().c_str(),oldName.size());
    strncpy((char *)pdu->caData+32,newName.toStdString().c_str(),newName.size());
    strncpy((char *)pdu->caMsg,this->m_currentDir.toStdString().c_str(),this->m_currentDir.size());
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void directoryOPE::handleReName(PDU *pdu)
{
    QString data(pdu->caData);
    if(data==COM_SUCCESS)
    {
        QMessageBox::information(this,"重命名","重命名成功");
    }
    else
    {
        QMessageBox::critical(this,"重命名","重命名失败");
    }
    //this->refreashDir();
    QTimer::singleShot(0, this, [=]{
        TcpClient::getTcpclient().showDir();
    });
}

void directoryOPE::enterDir(const QModelIndex &index)
{
    QString name=index.data().toString();
    if(index.data(FILE_TYPE_ROLE).toInt()==ENUMFILE_TYPE_PRI_FILE)
    {
        QToolTip::showText(QCursor::pos(),"进入失败，这不是一个文件夹无法进入");
        return;
    }
    this->m_currentDir+="/"+name;
    QTimer::singleShot(0, this, [=]{
        TcpClient::getTcpclient().showDir();
    });
}

void directoryOPE::back()
{
    if(this->m_currentDir.lastIndexOf("/")==1)
    {
        QToolTip::showText(QCursor::pos(),"返回失败，已是最深目录");
        return;
    }
    //qDebug()<<this->m_currentDir;
    int pos=this->m_currentDir.lastIndexOf("/");
    if(pos<0)
        return;
    this->m_currentDir=this->m_currentDir.left(pos);
    //qDebug()<<this->m_currentDir;

    QTimer::singleShot(0, this, [=]{
        TcpClient::getTcpclient().showDir();
    });
}

void directoryOPE::upload()
{
    QString curPath=this->m_currentDir;
    bool isok=false;
    QString upLoadFilePath=QFileDialog::getOpenFileName(this,"选择文件","D:\\ENVi\\");
    if(upLoadFilePath.isEmpty())
    {
        // QMessageBox::critical(this,"file upload","文件不能为空");
        return;
    }
    QString upFileName=upLoadFilePath.right(upLoadFilePath.size()-1-upLoadFilePath.lastIndexOf('/'));
    this->m_upFilePath=upLoadFilePath;
    this->m_upFileName=upFileName;
    QFile file(upLoadFilePath);
    qint64 fileSize=file.size();
    PDU* pdu=mkPDU(this->m_currentDir.size()+1);
    pdu->uiMsgType=ENUM_DIR_TYPE_UPLOAD_REQUEST;
    strncpy(pdu->caData,upFileName.toStdString().c_str(),upFileName.size());
    std::string strfileseze=std::to_string(fileSize);
    strncpy(pdu->caData+48,strfileseze.c_str(),strfileseze.size());
    strncpy((char *)pdu->caMsg,this->m_currentDir.toStdString().c_str(),this->m_currentDir.size());
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void directoryOPE::handleUpload(PDU *Pdu)
{
    QString rep(Pdu->caData);
    qDebug()<<rep;
    if(rep==UPLOADFILE_READY)
    {
        // qDebug()<<"ready";
        QFile file(this->m_upFilePath);
        qDebug()<<this->m_upFilePath;
        if(!file.open(QIODevice::ReadOnly))
        {
            qDebug()<<"file open failed";
            return;
        }
        char buf[4096];
/*按pdu方式发送
        PDU* pdu=mkPDU(4096);
        pdu->uiMsgType=ENUM_DIR_TYPE_UPLOAD_DATABLOCK;
        int n=0;
        int insended=0;
        while(n=file.read(buf,sizeof(buf)))
        {

            insended+=n;
            memcpy((char *)pdu->caMsg,"\0",pdu->uiMsgLen);
            memcpy((char *)pdu->caMsg,buf,n);
            std::string si=std::to_string(insended);
            std::string sin=std::to_string(n);
            strncpy(pdu->caData,si.c_str(),si.size());
            strncpy(pdu->caData+32,sin.c_str(),sin.size());
            //qDebug()<<pdu->caData;
            TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
            qDebug()<<(double)insended/file.size()*100<<"%"<<file.size();
        }
        free(pdu);
        pdu=nullptr;
*/

//直接按字节流方式发送
        qDebug()<<"start upload";
        int n=0;
        int sended=0;
        while((n=file.read(buf,sizeof(buf)))>0)
        {
            TcpClient::getTcpclient().getTcpsocket().write(buf,n);
            sended+=n;
            qDebug()<<sended<<"|"<<file.size();
        }
//结束发送
        file.close();
    }
    else if(rep==UPLOADFILE_SUCCESS)
    {
        QMessageBox::information(this,"upload file","文件上传成功");
        QTimer::singleShot(0, this, [=]{
            TcpClient::getTcpclient().showDir();
        });
    }
    else
    {
        QMessageBox::critical(this,"upload file","文件上传失败");
    }
}

void directoryOPE::downLoad()
{
    QListWidgetItem* curItem=ui->list_dir->currentItem();
    if(curItem==nullptr)
    {
        QMessageBox::warning(this,"下载文件","请选择要下载的文件");
        return;
    }
    if(curItem->data(FILE_TYPE_ROLE).toInt()==ENUMFILE_TYPE_DIR)
    {
        QMessageBox::warning(this,"下载文件","请选择文件");
        return;
    }
    QString name=curItem->text();
    this->m_downLoadName=name;

    QString dirPath=QFileDialog::getExistingDirectory(this,"请选择保存位置","D:\\ENVi\\");
    this->m_downLoadPath=dirPath+"/"+this->m_downLoadName;
    //qDebug()<<this->m_downLoadPath;

    PDU* pdu=mkPDU(this->m_currentDir.size()+1);
    pdu->uiMsgType=ENUM_DIR_TYPE_DOWNLOAD_REQUEST;
    strncpy((char *)pdu->caMsg,this->m_currentDir.toStdString().c_str(),this->m_currentDir.size());
    strncpy((char *)pdu->caData,name.toStdString().c_str(),name.size());
    TcpClient::getTcpclient().getTcpsocket().write((char *)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=nullptr;
}

void directoryOPE::handleDownLoad(PDU *Pdu)
{
    QString data=Pdu->caData;
    QString strsize=Pdu->caData+48;
    if(data==DOWNLOADFILE_FAILED)
    {
        QMessageBox::information(this,"下载文件","文件下载失败");
        this->m_isDownload=false;
        this->m_hasDownFileSize=0;
        this->m_downFileSize=0;
        return;
    }
    else if(data==DOWNLOADFILE_SUCCESS)
    {
        QMessageBox::information(this,"下载文件","文件下载成功\n文件路径为："+this->m_downLoadPath);
        this->m_isDownload=false;
        this->m_hasDownFileSize=0;
        this->m_downFileSize=0;
        return;
    }
    else
    {
        long long llsize=std::stoll(strsize.toStdString());
        this->m_downFileSize=llsize;
        this->m_hasDownFileSize=0;
        this->m_isDownload=true;
        QFile file(this->m_downLoadPath);
        if(!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this,"文件下载","文件打开失败，无法下载");
            this->m_isDownload=false;
            return;
        }
        file.close();
    }
}

void directoryOPE::DownLoadFileData()
{
    char buff[4096];
    int n=0;
    QFile file(this->m_downLoadPath);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        QMessageBox::warning(this,"文件下载","文件打开失败，无法下载");
        return;
    }

    long long remainSize=this->m_downFileSize-this->m_hasDownFileSize;
    while(TcpClient::getTcpclient().getTcpsocket().bytesAvailable()>0&&(n=TcpClient::getTcpclient().getTcpsocket().read(buff,remainSize>=sizeof(buff)?sizeof(buff):remainSize))>0)
    {
        //qDebug()<<"remainSize:  "<<remainSize;
        if(n==-1)
            break;
        file.write(buff,n);
        // qDebug()<<"readed:  "<<n;
        this->m_hasDownFileSize+=n;\
        //qDebug()<<"hasreaded:  "<<this->m_hasDownFileSize;
        remainSize=this->m_downFileSize-this->m_hasDownFileSize;
        qDebug()<<"process:  "<<this->m_hasDownFileSize<<"|"<<this->m_downFileSize;
        if(this->m_hasDownFileSize>=this->m_downFileSize)
        {
            this->m_isDownload=false;
            QTimer::singleShot(10,[=](){
                TcpClient::getTcpclient().onreadyDetach();
            });
            break;
        }
    }
    file.close();
    if(this->m_hasDownFileSize>=this->m_downFileSize)
    {
        this->m_isDownload=false;
    }
}

void directoryOPE::initShare()
{
    this->m_shareToFriend->setSharedObj(openwidget::getopewidget().getfriendWidget()->getListFriendItemList());
    this->m_shareToFriend->init();
}

void directoryOPE::chareedFile()
{
    QListWidgetItem* curItem=ui->list_dir->currentItem();
    if(curItem==nullptr)
    {
        QMessageBox::warning(this,"分享文件","请选择要分享的文件");
        return;
    }
    if(curItem->data(FILE_TYPE_ROLE).toInt()==ENUMFILE_TYPE_DIR)
    {
        QMessageBox::warning(this,"分享文件","请选择文件");
        return;
    }
    QString name=curItem->text();
    QString shareFilePath=this->m_currentDir+"/"+name;

    this->m_shareToFriend->setSharedFilePath(shareFilePath);

    this->m_shareToFriend->setHidden(false);
}
