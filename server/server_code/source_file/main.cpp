#pragma once
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include<dirent.h>
#include"pthreadPool.h"
#include"opeDB.h"
#include"protocol.h"
#include"opeDIR.h"
#include<fstream>
#include<sys/stat.h>

#define PORT 7911

#define rootpath "/home/stwo/resource"

using namespace std;
void primarytask(void *argc);
void connectNewclient(int *argc);
void set_nonblock(int &fd);
void set_EPOLLET(epoll_event &eev);

int g_epfd;
unordered_map<int,string> onlinea; //在线人数，sockfd和name
pthread_mutex_t lock;
unordered_map<int,string> upLoadTask;

void init()
{
    opeDB* mysql=new opeDB;
    string cmd="update userinfo set stat='0';";
    mysql->connectDB();
    mysql->opeCommand(cmd);
    delete mysql;
    mysql=nullptr;
}

int main(int argc,char* argv[])
{
    init();
    
    pthread_mutex_init(&lock,nullptr);
    signal(SIGPIPE, SIG_IGN);
    int ret=chdir(rootpath);
    //execlp("pwd","pwd",NULL);
    if(ret!=0)
        perror("chdir ");

    pthreadPool pool(3);

    int lfd=socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in saddr;
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORT);
    saddr.sin_addr.s_addr=htonl(INADDR_ANY);
    
    //设置端口复用
    int opt = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) 
    {
    perror("setsockopt SO_REUSEADDR");
    // 可根据情况处理错误
    }   

    bind(lfd,(sockaddr*)&saddr,sizeof(saddr));
    listen(lfd,1024);

    int epfd=epoll_create(10);
    g_epfd=epfd;

    epoll_event event,events[10];
    event.data.fd=lfd;
    //设置非阻塞
    set_nonblock(lfd);

    event.events=EPOLLIN|EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&event);

    while(true)
    {
        int nready=epoll_wait(epfd,events,10,-1);
        for(int i=0;i<nready;++i)
        {
            int *sockfd=new int (events[i].data.fd);
            task_t task;
            if(*sockfd==lfd)
            {
                connectNewclient(sockfd);
            }
            else
            {
                task.function=primarytask;
                task.argc=sockfd;
                pool.add_task_auto_working(task);
            }
        }
    }
    return 0;
}

void connectNewclient(int *argc)
    {
    sockaddr_in caddr;
    socklen_t len=sizeof(caddr);
    int *ptrlfd=(int *)argc;
    int lfd=*ptrlfd;
    delete ptrlfd;
    int cfd;
    while((cfd=accept(lfd,(sockaddr *)&caddr,&len))>0)
    {
        set_nonblock(cfd);

        epoll_event event;
        event.data.fd=cfd;
        set_EPOLLET(event);
        int n= epoll_ctl(g_epfd,EPOLL_CTL_ADD,cfd,&event);
        char ipbuf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &caddr.sin_addr, ipbuf, INET_ADDRSTRLEN); 
        cout<<" 客户端："<<ipbuf<<"连接成功!"<<std::endl;
    } 
}

void set_nonblock(int &fd)
{
    int flags=fcntl(fd,F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(fd,F_SETFL,flags);
}

void set_EPOLLET(epoll_event &eev)
{
    eev.events=EPOLLIN | EPOLLET|EPOLLONESHOT;
}

string getPath(int sockfd)
{
    string name=onlinea[sockfd];
    string root=rootpath;
    string path=root+"/"+name;
    return path;
}

void keepDirExists(int sockfd)
{
    string path=getPath(sockfd);
    //execlp("pwd","pwd",NULL);
    DIR* dir=opendir(path.c_str());
    if(dir==nullptr)
    { 
        int ret=mkdir(path.c_str(),0777);
        if(ret!=0)
        {
            perror("ckdir:");
        }
        
    }
    else
        closedir(dir);
}

int getSockfd(string peername)
{
    int peerSockfd=-1;
        pthread_mutex_lock(&lock);
        for(auto it=onlinea.begin();it!=onlinea.end();++it)
            if(it->second==peername)
            {
                peerSockfd=it->first;
                break;
            }
        pthread_mutex_unlock(&lock);
    return peerSockfd;
}

void repWithoutMsg(int sockfd,string achive)
{
    PDU* pdu=mkPDU(0);
    if(achive==REGISTER_SUCCESS)
        pdu->uiMsgType=ENUM_MSG_TYPE_REGIST_RESPOND;
    else if(achive==REGISTER_FAILED)
        pdu->uiMsgType=ENUM_MSG_TYPE_REGIST_RESPOND;
    else if(achive==LOGIN_SUCCESS)
        pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;
    else if(achive==LOGIN_FAILED)
        pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;
        
    strncpy(pdu->caData,achive.c_str(),achive.length());
    send(sockfd,(char *)pdu,pdu->uiPDULen,0);
    free(pdu);
    pdu=nullptr;
}

void registerEvent(int sockfd,PDU* pdu)
{
    //cout<<caname<<"   "<<capwd<<endl;
    string name(pdu->caData),pwd(pdu->caData+32);
    //cout<<name<<"   "<<pwd<<endl;
    std::string cmd="insert into userinfo (name,pwd) values ('"+name+"','"+pwd+"');";

    opeDB* mysql=new opeDB;
    mysql->connectDB();
    if(mysql->opeCommand(cmd))
    {
        //cout<<"succesful sqlcmd"<<endl;
        repWithoutMsg(sockfd,REGISTER_SUCCESS);
        keepDirExists(sockfd);
    }
    else
    {
        //cout<<"succesful sqlcmd"<<endl;
        repWithoutMsg(sockfd,REGISTER_FAILED);
    }
    delete mysql;
    mysql=nullptr;
}

void LogInEvent(int sockfd,PDU* pdu)
{
    string name(pdu->caData),pwd(pdu->caData+32);
    //cout<<name<<"   "<<pwd<<endl;
    std::string cmd="select * from userinfo where name='"+name+"' and pwd ='"+pwd+"' and stat=0;";
    
    opeDB* mysql=new opeDB;
    mysql->connectDB();
    if(mysql->opeCommand(cmd))
    {
        cmd="update userinfo set stat=1 where name=\'"+name+"\'";
        mysql->opeCommand(cmd);
        pthread_mutex_lock(&lock);
        onlinea.insert(make_pair(sockfd,name));
        pthread_mutex_unlock(&lock);
        repWithoutMsg(sockfd,LOGIN_SUCCESS);
        keepDirExists(sockfd);
    }
    else
    {
        std::cout<<"登录失败 "<<endl;
        repWithoutMsg(sockfd,LOGIN_FAILED);
    }
    delete mysql;
    mysql=nullptr;
}

void quitonline_Stat0(int sockfd)
{
    pthread_mutex_lock(&lock);
    auto it=onlinea.find(sockfd);
    if(it==onlinea.end())
    {
        //cout<<"no it"<<endl;
        pthread_mutex_unlock(&lock);
    }
    else
    {
        //cout<<"yes it"<<endl;
        string cmd="update userinfo set stat=0 where name=\'"+it->second+"\'";
        opeDB* mysql=new opeDB;
        mysql->connectDB();
        mysql->opeCommand(cmd);
        onlinea.erase(it);
        pthread_mutex_unlock(&lock);
        delete mysql;
    }
}

void showOnlineUserEvent(int &sockfd,PDU* Pdu)
{
    std::string cmd="select name from userinfo where stat=1";
    opeDB* mysql=new opeDB;
    mysql->connectDB();
    std::vector<std::string> ans=mysql->opecommand(cmd,1);
    int n=ans.size();
    PDU* pduback=mkPDU(n*32);
    pduback->uiMsgType=ENUM_MSG_TYPE_ALLONLINE_RESPOND;
    
    for(int i=0;i<n;++i)
    {
        strncpy((char *)pduback->caMsg+i*32,ans[i].c_str(),ans[i].length());
        //cout<<ans[i]<<"    6666666666 \n";
    }
    send(sockfd,(char *)pduback,pduback->uiPDULen,0);

    free(pduback);
    pduback=nullptr;
    delete mysql;
    mysql=nullptr;
    // cout<<cmd<<endl;
}
 
void SearchEvent(int &sockfd,PDU* pdu)
{
    std::string dpuname(pdu->caData);
    std::string cmd="select name,stat from userinfo where name=\'"+dpuname+"\'";
    opeDB* mysql=new opeDB;
    mysql->connectDB();
    std::vector<std::string> ans=mysql->opecommand(cmd,1);
    PDU* pduback=mkPDU(0);
    pduback->uiMsgType=ENUM_MSG_TYPE_SEARCH_RESPOND;
    string name,stat;
    if(ans.empty())
        strncpy(pduback->caData,SEARCH_NO_SUCH_USER,sizeof(SEARCH_NO_SUCH_USER));
    else
    {
        name=ans[0].substr(0,ans[0].find(" "));
        stat=ans[0].substr(ans[0].find(" ")+1,ans[0].rfind(" ")-ans[0].find(" ")-1);
        string data;
        if(stat=="0")
            data=SEARCH_USER_OFFLINE;
        else 
            data=SEARCH_USER_ONLINE;
        cout<<data<<endl;
        strncpy(pduback->caData,data.c_str(),data.length());
         cout<<name<<" "<<stat<<endl;
    }

    send(sockfd,(char *)pduback,pduback->uiPDULen,0);
    free(pduback);
    pduback=nullptr;
    delete mysql;
    mysql=nullptr;
}

void addFriendevent(int &sockfd,PDU* Pdu)
{
    string myname=onlinea[sockfd];
    string peername(Pdu->caData);
    //cout<<myname<<"/"<<peername<<"1"<<endl;
    opeDB* mysql=new opeDB;
    mysql->connectDB();
    std::pair<int,int> result=mysql->isFriend(myname,peername);
    PDU* pduback=nullptr;
    if(result.first==-1&&result.second==-1)
    {   
        pduback=mkPDU(0);
        pduback->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
        strncpy(pduback->caData,UNKNOWN_ERROR,sizeof(UNKNOWN_ERROR));
    }
    else if(result.first==1)
    {
        pduback=mkPDU(0);
        pduback->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
        strncpy(pduback->caData,ADDFRI_FRIEND_EXIST,sizeof(ADDFRI_FRIEND_EXIST));
    }
    else if(result.first==0&&result.second==1)
    {

        int peerSockfd=-1;
        pthread_mutex_lock(&lock);
        for(auto it=onlinea.begin();it!=onlinea.end();++it)
            if(it->second==peername)
            {
                peerSockfd=it->first;
                break;
            }
        pthread_mutex_unlock(&lock);
        
        memset(Pdu->caData,'\0',64);
        strncpy(Pdu->caData,myname.c_str(),myname.length());
        strncpy((char*)Pdu->caData+32,peername.c_str(),peername.length());
        Pdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
        send(peerSockfd,(char *)Pdu,Pdu->uiPDULen,0);
        goto HERE;
    }
    else if(result.first==0&&result.second==0)
    {
        pduback=mkPDU(0);
        pduback->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
        strncpy(pduback->caData,ADDFRI_USER_OFFLINE,sizeof(ADDFRI_USER_OFFLINE));
    }
    else if(result.first==0&&result.second==2)
    {
        pduback=mkPDU(0);
        pduback->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
        strncpy(pduback->caData,ADDFRI_USER_NOT_EXIST,sizeof(ADDFRI_USER_NOT_EXIST));
    }
    // cout<<"pass id else"<<endl;
    send(sockfd,(char *)pduback,pduback->uiPDULen,0);
    // cout<<"pass id else"<<endl;
    HERE:
    free(pduback);
    pduback=nullptr;
    // cout<<"pass id else"<<endl;
    delete mysql;
    mysql=nullptr;
}

void addFriend(int &sockfd,PDU* Pdu)
{
    std::string requestname(Pdu->caData),resondname(Pdu->caData+32);
    int peerSockfd=-1;
    pthread_mutex_lock(&lock);
    for(auto it=onlinea.begin();it!=onlinea.end();++it)
    {    
        cout<<it->first<<"|"<<it->second.size()<<"|"<<endl;
        cout<<requestname<<"*"<<resondname.size()<<"*"<<endl;
        if(it->second==resondname)
        {
            
            peerSockfd=it->first;
            break;
        }
    }
    pthread_mutex_unlock(&lock);
    if(peerSockfd==-1)
    {
        cout<<"未获得对方sockfd"<<endl;
        return ;
    }
    PDU* pduback=mkPDU(0);
    if(Pdu->uiMsgType==ENUM_MSG_TYPE_ADD_FRIEND_DISAGREE)
    {
        strncpy(pduback->caData,ADDFRI_DISAGGRE,sizeof(ADDFRI_DISAGGRE));
    }
    else
    {
        opeDB* mysql=new opeDB;
        mysql->connectDB();
        string cmd="insert into userFriend values ((select id from userinfo where name=\'"+requestname+"\') , (select id from userinfo where name=\'"+resondname+"\'))";
        if(mysql->opeCommand(cmd))
        {
            strncpy(pduback->caData,ADDFRI_AGGRE,sizeof(ADDFRI_AGGRE));
        }
        else
        {
            strncpy(pduback->caData,UNKNOWN_ERROR,sizeof(UNKNOWN_ERROR));
        }
    }
    pduback->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
    send(peerSockfd,(char *)pduback,pduback->uiPDULen,0);
    free(pduback);
    pduback=nullptr;
}

void reFreashFriendEvent(int &sockfd,PDU* Pdu)
{
    string name(Pdu->caData);
    string cmd="select name,stat from userinfo where (id IN(select fr_id from userFriend where m_id=(select id from userinfo where name=\'"+name+"\')) or id IN(select m_id from userFriend where fr_id=(select id from userinfo where name=\'"+name+"\')));";
    opeDB* mysql=new opeDB;
    mysql->connectDB();
    std::vector<std::string> ret=mysql->opecommand(cmd,1);
    int n=ret.size();
    PDU* pduback=mkPDU(n*33);
    pduback->uiMsgType=ENUM_MSG_TYPE_REFREASHFRIEND_RESPOND;
    for(int i=0;i<n;++i)
    {
        //cout<<"////////"<<ret[i]<<endl;
        string mname=ret[i].substr(0,ret[i].find(" "));
        string stat=ret[i].substr(ret[i].find(" ")+1,1);
        //cout<<"refreash:"<<mname<<"|"<<stat<<"|"<<endl;
        strncpy((char *)pduback->caMsg+i*33,mname.c_str(),mname.length());
        strncpy((char *)pduback->caMsg+i*33+32,stat.c_str(),stat.length());
    }
    send(sockfd,(char *)pduback,pduback->uiPDULen,0);

    free(pduback);
    pduback=nullptr;
    delete mysql;
    mysql=nullptr;
}

void DeleteFriendEvent(int &sockfd,PDU *Pdu)
{
    std::string peername(Pdu->caData);
    std::string myname(onlinea[sockfd]);
    std::string cmd="delete from userFriend where (m_id=(select id from userinfo where name=\'"+myname+"\') and fr_id=(select id from userinfo where name=\'"+peername+"\')) or (fr_id=(select id from userinfo where name=\'"+myname+"\')and m_id=(select id from userinfo where name=\'"+peername+"\'));";
    // cout<<"refreash:"<<myname<<"|"<<peername<<"|"<<endl;
    // cout<<cmd<<endl;
    opeDB* mysql=new opeDB;
    mysql->connectDB();
    PDU* pduback=mkPDU(0);
    pduback->uiMsgType=ENUM_MSG_TYPE_DELETEFRIEND_RESPOND;
    if(mysql->opeCommand(cmd))
    {
        strncpy(pduback->caData,DELETE_FRIEND_SUCCESS,sizeof(DELETE_FRIEND_SUCCESS));
    }
    else
    {
        strncpy(pduback->caData,DELETE_FRIEND_FAILED,sizeof(DELETE_FRIEND_FAILED));
    }

    send(sockfd,(char *)pduback,pduback->uiPDULen,0);
    
    delete mysql;
    mysql=nullptr;
    free(pduback);
    pduback=nullptr;
}

void PrivateChatEvent(int &sockfd,PDU* Pdu)
{
    string peername(Pdu->caData);

    int peerSockfd=-1;
    pthread_mutex_lock(&lock);
     for(auto it=onlinea.begin();it!=onlinea.end();++it)
        if(it->second==peername)
        {
            peerSockfd=it->first;
            break;
        }
    pthread_mutex_unlock(&lock);

    PDU* pduback=mkPDU(Pdu->uiMsgLen);
    pduback->uiMsgType=ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND;
    strncpy((char *)pduback->caMsg,(char *)Pdu->caMsg,Pdu->uiMsgLen);
    strncpy(pduback->caData,onlinea[sockfd].c_str(),onlinea[sockfd].length());
    send(peerSockfd,(char *)pduback,pduback->uiPDULen,0);
    // cout<<(char *)Pdu->caMsg<<endl;
    // cout<<pduback->caData<<":"<<(char *)pduback->caMsg<<endl;
    free(pduback);
    pduback=nullptr;
}

void GroupChatEvent(int sockfd,PDU *Pdu)
{
    string name(Pdu->caData);
    PDU* pduback=mkPDU(Pdu->uiMsgLen);
    pduback->uiMsgType=ENUM_MSG_TYPE_GROUP_CHAT_RESPOND;
    strncpy((char*)pduback->caMsg,(char *)Pdu->caMsg,Pdu->uiMsgLen);
    strncpy(pduback->caData,name.c_str(),name.size());

    string cmd="select name from userinfo where (id IN(select fr_id from userFriend where m_id=(select id from userinfo where name=\'"+name+"\')) or id IN(select m_id from userFriend where fr_id=(select id from userinfo where name=\'"+name+"\'))) and stat=\'1\';";
    opeDB* mysql=new opeDB;
    mysql->connectDB();
    vector<string> ret=mysql->opecommand(cmd,1);
    int n=ret.size();
    //cout<<n<<endl;
    for(int i=0;i<n;++i)
    {
        string peername=ret[i].substr(0,ret[i].find(" "));
        int sendsockfd=getSockfd(peername);
        //cout<<sendsockfd<<endl;
        send(sendsockfd,(char *)pduback,pduback->uiPDULen,0);
    }
    delete mysql;
    mysql=nullptr;
    free(pduback);
    pduback=nullptr;
}

void createDirEvent(int &sockfd,PDU *Pdu)
{
    string currentPath((char *)Pdu->caMsg);
    string NewName(Pdu->caData);
    string Newpath=currentPath+"/"+NewName;
    // std::cout<<"current dir:"<<currentPath<<endl;
    DIR* dir=opendir(currentPath.c_str());
    PDU* pduback=mkPDU(0);
    pduback->uiMsgType=ENUM_DIR_TYPE_CREATE_DIR_RESPOND;
    if(opendir(Newpath.c_str())!=nullptr)
    {
        strncpy(pduback->caData,DIR_CREATE_DIR_exists,sizeof(DIR_CREATE_DIR_exists));
        goto flags;
    }
    if(dir==nullptr)
    {
        strncpy(pduback->caData,DIR_CREATE_DIR_FAILED,sizeof(DIR_CREATE_DIR_FAILED));
    }
    else
    {
        int ret=mkdir(Newpath.c_str(),0777);
        if(ret!=0)
        {
            strncpy(pduback->caData,DIR_CREATE_DIR_FAILED,sizeof(DIR_CREATE_DIR_FAILED));
        }
        else
        {
            strncpy(pduback->caData,DIR_CREATE_DIR_SUCCESS,sizeof(DIR_CREATE_DIR_SUCCESS));
        }
        closedir(dir);
    }
    flags:
    std::cout<<"current dir:"<<currentPath<<endl;
    send(sockfd,(char *)pduback,pduback->uiPDULen,0);
    free(pduback);
    pduback=nullptr;
}

void refreashDirEvent(int &sockfd,PDU *Pdu)
{
    string currentPath(Pdu->caData);
    opeDIR dir(currentPath);
    vector<dirent> ret=dir.getCurrentFileInfo();
    int n=ret.size();
    PDU* pduback=mkPDU(n*32);
    pduback->uiMsgType=ENUM_DIR_TYPE_REFREASH_DIR_RESPOND;
    for(int i=0;i<n;++i)
    {
        dirent mydirent=ret[i];
        string name=mydirent.d_name;
        strncpy((char *)pduback->caMsg+i*32,name.c_str(),name.size());
        if(mydirent.d_type==DT_DIR)
            strncpy((char *)pduback->caMsg+i*32+31,"1",1);
        else 
            strncpy((char *)pduback->caMsg+i*32+31,"0",1);
    }
    send(sockfd,(char *)pduback,pduback->uiPDULen,0);
    free(pduback);
    pduback=nullptr;
}

bool dirIsEmpty(string path)
{
    int count=0;
    DIR* dir=opendir(path.c_str());
    dirent* dirent= nullptr;
    while((dirent=readdir(dir))!=nullptr)
    {
        count++;
    }
    if(dir!=nullptr)
        closedir(dir);
    return count==2?true:false;
}

bool removeAnyFile(string path)
{
    DIR* dir=opendir(path.c_str());
    if(dir==nullptr)
        return false;
    dirent* mydirent= nullptr;
    bool childBool=true;
    while((mydirent=readdir(dir))!=nullptr)
    {
        string name=(char *)mydirent->d_name;
        if(name=="."||name=="..")
            continue;
        string delpath=path+"/"+name;
        if(mydirent->d_type==DT_DIR)      //是文件夹就递归
        {
                childBool=removeAnyFile(delpath)&&childBool;
                rmdir(delpath.c_str());
        }
        else                            //否则直接删除
        {
            return remove(delpath.c_str())==0?true:false;
        }
    }
    if(dir!=nullptr)
        closedir(dir);
    return childBool&&true;
}

void delFileEvent(int& sockfd,PDU* Pdu)
{
    string curpath((char *)Pdu->caMsg);
    string name(Pdu->caData);
    string filepath=curpath+"/"+name;
    // cout<<filepath<<endl;
    PDU* pduback=mkPDU(0);
    pduback->uiMsgType=ENUM_DIR_TYPE_DELETE_FILE_RESPOND;
    int fd=open(filepath.c_str(),O_RDONLY);
    if(fd==-1)
    {
        strncpy(pduback->caData,DEL_DIR_FILE_FAILED,sizeof(DEL_DIR_FILE_FAILED));
    }
    else
    {
        close(fd);
        remove(filepath.c_str());
        strncpy(pduback->caData,DEL_DIR_FILE_SUCCESS,sizeof(DEL_DIR_FILE_SUCCESS));
    }
    send(sockfd,(char *)pduback,pduback->uiPDULen,0);

    free(pduback);
    pduback=nullptr;
}

void delDirEvent(int& sockfd,PDU* Pdu)
{
    string curpath((char *)Pdu->caMsg);
    string name(Pdu->caData);
    string filepath=curpath+"/"+name;
    // cout<<filepath<<endl;
    bool isok=removeAnyFile(filepath);
    int ret=rmdir(filepath.c_str());
    PDU* pduback=mkPDU(0);
    pduback->uiMsgType=ENUM_DIR_TYPE_DELETE_DIR_RESPOND;
    if(!isok||(ret!=0))
    {
        strncpy(pduback->caData,DEL_DIR_FILE_FAILED,sizeof(DEL_DIR_FILE_FAILED));
    }
    else
    {
        strncpy(pduback->caData,DEL_DIR_FILE_SUCCESS,sizeof(DEL_DIR_FILE_SUCCESS));
    }
    send(sockfd,(char *)pduback,pduback->uiPDULen,0);

    free(pduback);
    pduback=nullptr;
}

void renameEvent(int& sockfd,PDU* Pdu)
{
    string oldName(Pdu->caData);
    string newName(Pdu->caData+32);
    string curPath((char *)Pdu->caMsg);
    string oldp=curPath+"/"+oldName;
    string newp=curPath+"/"+newName;
    PDU* pduback=mkPDU(0);
    pduback->uiMsgType=ENUM_DIR_TYPE_RENAME_RESPOND;
    if(rename(oldp.c_str(),newp.c_str())==0)
        strncpy(pduback->caData,COM_SUCCESS,sizeof(COM_SUCCESS));
    else
        strncpy(pduback->caData,COM_FAILED,sizeof(COM_FAILED));

    send(sockfd,(char *)pduback,pduback->uiPDULen,0);
    free(pduback);
    pduback=nullptr;
}

void upLoadEvent(int& sockfd,PDU* Pdu)
{
    
    string upFileName(Pdu->caData);
    string upFileSize(Pdu->caData+48);
    string curPath((char *)Pdu->caMsg);
    // cout<<curPath<<endl;
    string newFile=curPath+"/"+upFileName;
    PDU* pduback=mkPDU(0);
    pduback->uiMsgType=ENUM_DIR_TYPE_UPLOAD_RESPOND;

 cout<<"upLoadTask debug"<<endl;

    if(upLoadTask.find(sockfd)!=upLoadTask.end())   //正在进行文件传输任务
    {
        strncpy(pduback->caData,UPLOADFILE_FAILED,sizeof(UPLOADFILE_FAILED));
        send(sockfd,(char *)pduback,pduback->uiPDULen,0);
        free(pduback);
        pduback=nullptr;
 cout<<"upLoadTask failed"<<endl;
        pthread_mutex_lock(&lock);
        auto it=upLoadTask.find(sockfd);
        upLoadTask.erase(it);
        pthread_mutex_unlock(&lock);
        return;
    }
    else
    {
        strncpy(pduback->caData,UPLOADFILE_READY,sizeof(UPLOADFILE_READY));
        pthread_mutex_lock(&lock);
        upLoadTask.insert(make_pair(sockfd,newFile+" "+upFileSize));
        pthread_mutex_unlock(&lock);
        send(sockfd,(char *)pduback,pduback->uiPDULen,0);
        free(pduback);
        pduback=nullptr;
    }
//上传文件的就绪工作
    int ffd=open(newFile.c_str(),O_CREAT|O_WRONLY|O_TRUNC,0644);
    close(ffd);

//文件上传的准备工作结束

//直接读取不用pdu
    long long fullSize=std::stoll(upFileSize);
    int bn=0;
    char buff[4096];
    ffd=open(newFile.c_str(),O_WRONLY);
    long long recved=0;
cout<<recved<<"once    getcharNumber: "<<bn<<"   fullsize: "<<fullSize<<" "<<upFileSize<<endl;

    while(bn=recv(sockfd,buff,sizeof(buff),0))
    {
        if(bn==-1)
        {
            usleep(1000);
cout<<"upLoadEvent no message"<<endl;
            continue;
        }
        if(bn==0)
            return;
        recved+=bn;
        write(ffd,buff,bn);
cout<<sockfd<<" "<<recved<<"    getcharNumber: "<<bn<<"   fullsize: "<<fullSize<<" "<<upFileSize<<endl;
        if(recved>=fullSize)
        {
            PDU* pduback=mkPDU(0);
            pduback->uiMsgType=ENUM_DIR_TYPE_UPLOAD_RESPOND;
            strncpy(pduback->caData,UPLOADFILE_SUCCESS,sizeof(UPLOADFILE_SUCCESS));
            send(sockfd,(char *)pduback,pduback->uiPDULen,0);
            free(pduback);
            pduback=nullptr;

            pthread_mutex_lock(&lock);
            auto it=upLoadTask.find(sockfd);
            upLoadTask.erase(it);
            pthread_mutex_unlock(&lock);
            break;
        }
    }
//结束接收
    close(ffd);
}

void upLoadDataBlockEvent(int& sockfd,PDU* Pdu)
{

    pthread_mutex_lock(&lock);
    string filePath=upLoadTask[sockfd];
    pthread_mutex_unlock(&lock);
cout<<filePath<<endl;
    string sFilesize=filePath.substr(filePath.find(" ")+1,filePath.size()-filePath.find(" ")-1);
    filePath=filePath.substr(0,filePath.find(" "));
    string str_receivedSize(Pdu->caData);
    string str_cursize(Pdu->caData+32);
//cout<<Pdu->caData<<"    here      9999999"<<endl;
    long long recvSize=std::stoi(str_receivedSize.c_str());
    long long fullsize=std::stoi(sFilesize.c_str());
    long long getsize=std::stoi(str_cursize.c_str());
    string fileData((char *)Pdu->caMsg,getsize);

cout<<getsize<<" "<<recvSize<<" "<<fullsize<<" "<<recvSize/4096<<endl;
    int ffd=open(filePath.c_str(),O_APPEND|O_WRONLY);
    write(ffd,fileData.c_str(),fileData.size());
    close(ffd);

    if(recvSize>=fullsize)
    {
        PDU* pduback=mkPDU(0);
        pduback->uiMsgType=ENUM_DIR_TYPE_UPLOAD_RESPOND;
        strncpy(pduback->caData,UPLOADFILE_SUCCESS,sizeof(UPLOADFILE_SUCCESS));
        send(sockfd,(char *)pduback,pduback->uiPDULen,0);
        free(pduback);
        pduback=nullptr;

        pthread_mutex_lock(&lock);
        auto it=upLoadTask.find(sockfd);
        upLoadTask.erase(it);
        pthread_mutex_unlock(&lock);
    }
}

long long getFileSize(string filePath)
{
    long long size=0;
    struct stat st;
    stat(filePath.c_str(),&st);
    size=st.st_size;
    return size;
}

void DOWNLoadEvent(int& sockfd,PDU* Pdu)
{
    string curPath((char *)Pdu->caMsg);
    string name(Pdu->caData);
    string filePath=curPath+"/"+name;
cout<<"path:"<<filePath<<endl;
    int ffd=open(filePath.c_str(),O_RDONLY);
    if(!ffd)    //文件打开失败
    {
        PDU* pduback=mkPDU(0);
        pduback->uiMsgType=ENUM_DIR_TYPE_DOWNLOAD_RESPOND;
        memcpy(pduback->caData,DOWNLOADFILE_FAILED,sizeof(DOWNLOADFILE_FAILED));
        send(sockfd,(char*)pduback,pduback->uiPDULen,0);
        free(pduback);
        pduback=nullptr;
        return;
    }
//打开成功
    PDU* pduback=mkPDU(0);
    pduback->uiMsgType=ENUM_DIR_TYPE_DOWNLOAD_RESPOND;
    memcpy(pduback->caData,DOWNLOADFILE_READY,sizeof(DOWNLOADFILE_READY));
    string fileSize=std::to_string(getFileSize(filePath));
cout<<"filesize:"<<fileSize<<endl;
    memcpy(pduback->caData+48,fileSize.c_str(),fileSize.size());
    send(sockfd,(char*)pduback,pduback->uiPDULen,0);
    
    sleep(1);

//循环发送文件信息
    char buf[4096];
    int n=0;
    long long  sended=0;
    long long  fullsize=getFileSize(filePath);
    while((n=read(ffd,buf,sizeof(buf)))>0)
    {
usleep(10);
        int sendsize=0;
        sended+=n;
        sendsize=send(sockfd,buf,n,0);
cout<<"process:"<<(long double)sended/fullsize*100<<"\t   |  sended size:"<<sended<<endl;
    }
    close(ffd);
//循环发送文件信息结束
    
    if(sended>=fullsize)
        memcpy(pduback->caData,DOWNLOADFILE_SUCCESS,sizeof(DOWNLOADFILE_SUCCESS));
    else
        memcpy(pduback->caData,DOWNLOADFILE_FAILED,sizeof(DOWNLOADFILE_FAILED));
    send(sockfd,(char*)pduback,pduback->uiPDULen,0);
cout<<"=========downLoad return msg========="<<endl;
    free(pduback);
    pduback=nullptr;
}

bool SEND(int &sockfd,PDU *pdu,int Len)
{
    int hasSend=0;
    int remain=Len;
    while(hasSend<pdu->uiPDULen)
    {
        remain=pdu->uiPDULen-hasSend;
        int n=send(sockfd,(char *)pdu+hasSend,remain,0);
        if(n<0)
            return false;
        hasSend+=n;
        if(hasSend>=pdu->uiPDULen)
        {
            return true;
        }   
    }
    if(hasSend>=pdu->uiPDULen)
    {
       return true;
    }
}

void shareFileEvent(int& sockfd,PDU* Pdu)
{
    string sendName(Pdu->caData,48);
    sendName=sendName.c_str();
    string sendCount(Pdu->caData+48);
    int count=std::stoi(sendCount);
//std::cout<<sendName.size()<<"   "<<sendName<<endl;
    string shareFilePath((char *)Pdu->caMsg,64);
    shareFilePath=shareFilePath.c_str();
//std::cout<<shareFilePath.size()<<"   "<<count<<endl; 
    vector<string> vecShareObj;
    for(int i=0;i<count;++i)
    {
        string singleObj((char *)Pdu->caMsg+64+i*32,32);
        singleObj=singleObj.c_str();
        vecShareObj.push_back(singleObj);
// cout<<i<<":"<<singleObj<<endl;
    }

    PDU* pdureq=mkPDU(0);
    pdureq->uiMsgType=ENUM_DIR_TYPE_IF_SHAREFILE_REQUEST;
    strncpy(pdureq->caData,shareFilePath.c_str(),shareFilePath.size());
    for(int i=0;i<count;++i)
    {
        int sockfd=getSockfd(vecShareObj[i]);
        SEND(sockfd,pdureq,pdureq->uiPDULen);
    }

    free(pdureq);
    pdureq=nullptr;
}

void getSharedFileEvent(int& sockfd,PDU* Pdu)
{
    string sendPath(Pdu->caData);
    string recvPath;
    recvPath="./"+onlinea[sockfd]+"/shared";
    //如果没有相关目录，则创建
    DIR* dir=opendir(recvPath.c_str());
    if(dir==nullptr)
        mkdir(recvPath.c_str(),0755);
    closedir(dir);
    string fileName=sendPath.substr(sendPath.find_last_of("/")+1,sendPath.size()-sendPath.find_last_of("/")-1);
    recvPath+="/"+fileName;
    int sendfd=open(sendPath.c_str(),O_RDONLY);
    if(sendfd==-1)
    {
        cout<<"getShareFile: open sendfile failed\n";
        return;
    }
    int recvfd=open(recvPath.c_str(),O_CREAT|O_TRUNC|O_WRONLY,0644);
        if(sendfd==-1)
    {
        cout<<"getShareFile: open recvfile failed\n";
        return;
    }

//可拓展分享失败的情形
    int n=0;
    char buf[4096];
    while((n=read(sendfd,buf,sizeof(buf)))>0)
    {
        write(recvfd,buf,n);
    }
    PDU* pduback=mkPDU(0);
    pduback->uiMsgType=ENUM_DIR_TYPE_SHAREFILE_RESPOND;
    strncpy(pduback->caData,SHAREFILE_SUCCESS,sizeof(SHAREFILE_SUCCESS));
    SEND(sockfd,pduback,pduback->uiPDULen);
}

int RECV(int &sockfd,char *pdu,int start,int needByteCount,int ifBlock=0)
{
    int hasRecv=start;
    int fullRecv=needByteCount+start;
    int remain=fullRecv-hasRecv;
    int n=0;
    while(hasRecv<fullRecv)
    {
        n=recv(sockfd,pdu+hasRecv,remain,ifBlock);
        if(n>0)
        {
            hasRecv+=n;
            remain=fullRecv-hasRecv;
        }
        else if(n==0)
            return n;
        else if(hasRecv==start)
            return -1;
        if(hasRecv>=fullRecv)
            return fullRecv;
    }
    if(n<=0)
        return n;
}

void primarytask(void *argc)
{
    int *fd=(int *)argc;
    int sockfd=*fd;
    delete fd;

    
while(true)
{
    //从sockfd接受所有数据
   
    uint uiPDULen=0;
    int n = RECV(sockfd,(char *)&uiPDULen,0,sizeof(uint),0);
cout<<"\nmain task 1-----read byte:"<<n<<"----plan read byte count:"<<sizeof(uint)<<"----read result"<<uiPDULen<<endl;
    if(n==0)
    {
        quitonline_Stat0(sockfd);
        std::cout<<"连接断开或出现错误"<<std::endl;
        epoll_ctl(g_epfd,EPOLL_CTL_DEL,sockfd,nullptr);
        close(sockfd);
        return;
    }
    if(n<0&&errno==EAGAIN)
    {
        break;
    }

cout<<"pdu.camsgLen: "<<uiPDULen-sizeof(PDU)<<endl;
    PDU* pdu=mkPDU(uiPDULen-sizeof(PDU));

    pdu->uiPDULen=uiPDULen;
    //std::cout<<pdu->uiPDULen<<std::endl;
    n= RECV(sockfd,(char *)pdu,sizeof(uint),uiPDULen-sizeof(uint),0);
    if(n==0)
    {
        quitonline_Stat0(sockfd);
        std::cout<<"连接断开或出现错误"<<std::endl;
        epoll_ctl(g_epfd,EPOLL_CTL_DEL,sockfd,nullptr);
        close(sockfd);
        return;
    }
    if(n<0&&errno==EAGAIN)
    {
        break;
    }
cout<<"main task 2-----read byte:"<<n<<"----read byte count:"<<uiPDULen-sizeof(uint)<<endl;
    //从sockfd接受所有数据
    switch (pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_REQUEST:
            //respond(sockfd);
            registerEvent(sockfd,pdu);
        break;
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
            //cout<<"login"<<endl;
            LogInEvent(sockfd,pdu);
        break;
    case ENUM_MSG_TYPE_ALLONLINE_REQUEST:
            //cout<<"showOnlineUserEvent"<<endl;
            showOnlineUserEvent(sockfd,pdu);
        break;
    case ENUM_MSG_TYPE_SEARCH_REQUEST:
            SearchEvent(sockfd,pdu);
        break;
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
            addFriendevent(sockfd,pdu);
        break;
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:
    case ENUM_MSG_TYPE_ADD_FRIEND_DISAGREE:
            addFriend(sockfd,pdu);
        break;
    case ENUM_MSG_TYPE_REFREASHFRIEND_REQUEST:
            reFreashFriendEvent(sockfd,pdu);
        break;
    case ENUM_MSG_TYPE_DELETEFRIEND_REQUEST:
            DeleteFriendEvent(sockfd,pdu);
        break;
    case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
            PrivateChatEvent(sockfd,pdu);
        break;
    case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:
            GroupChatEvent(sockfd,pdu);
        break;
    case ENUM_DIR_TYPE_CREATE_DIR_REQUEST:
            createDirEvent(sockfd,pdu);
        break;
    case ENUM_DIR_TYPE_REFREASH_DIR_REQUEST:
            refreashDirEvent(sockfd,pdu);
        break;
    case ENUM_DIR_TYPE_DELITE_FILE_REQUEST:
            delFileEvent(sockfd,pdu);
        break;
    case ENUM_DIR_TYPE_DELITE_DIR_REQUEST:
            delDirEvent(sockfd,pdu);
        break;
    case ENUM_DIR_TYPE_RENAME_REQUEST:
            renameEvent(sockfd,pdu);
        break;
    case ENUM_DIR_TYPE_UPLOAD_REQUEST:
            upLoadEvent(sockfd,pdu);
        break;
    case ENUM_DIR_TYPE_UPLOAD_DATABLOCK:
//            upLoadDataBlockEvent(sockfd,pdu);
        break;
    case ENUM_DIR_TYPE_DOWNLOAD_REQUEST:
            DOWNLoadEvent(sockfd,pdu);
        break;
    case ENUM_DIR_TYPE_SHAREFILE_REQUEST:
            shareFileEvent(sockfd,pdu);
        break;
    case ENUM_DIR_TYPE_IF_SHAREFILE_RESPOND:
            getSharedFileEvent(sockfd,pdu);
        break;
    default:
        std::cout<<pdu->uiMsgType<<"default"<<std::endl;
        break;
    }
    

    free(pdu);
    pdu=nullptr;
}
    epoll_event ev;
    ev.data.fd=sockfd;
    ev.events=EPOLLIN|EPOLLET|EPOLLONESHOT;
    epoll_ctl(g_epfd,EPOLL_CTL_MOD,sockfd,&ev);
cout<<"============该线程结束============="<<endl;
}