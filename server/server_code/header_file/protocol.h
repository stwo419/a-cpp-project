#ifndef PROTOCOL_H
#define PROTOCOL_H

#include<vector>
#include<iostream>
typedef unsigned int uint;

#define COM_SUCCESS "success"
#define COM_FAILED "FAILED"

#define REGISTER_SUCCESS "register success"
#define REGISTER_FAILED "register failed"

#define LOGIN_SUCCESS "login success"
#define LOGIN_FAILED "login failed"

#define SEARCH_NO_SUCH_USER "no such user"
#define SEARCH_USER_ONLINE "user online"
#define SEARCH_USER_OFFLINE "user offline"\

#define UNKNOWN_ERROR "unknown error"
#define ADDFRI_FRIEND_EXIST "friend exists"
#define ADDFRI_USER_OFFLINE "user offline"
#define ADDFRI_USER_ONLINE "user online"
#define ADDFRI_USER_NOT_EXIST "user not exists"
#define ADDFRI_AGGRE "agree"
#define ADDFRI_DISAGGRE "disagree"

#define DELETE_FRIEND_SUCCESS " delete friend success"
#define DELETE_FRIEND_FAILED " delete friend failed"

#define DIR_CREATE_DIR_FAILED "create directory failed"
#define DIR_CREATE_DIR_SUCCESS "create directory success"
#define DIR_CREATE_DIR_exists "DIR exists"

#define DEL_DIR_FILE_FAILED "del failed"
#define DEL_DIR_FILE_SUCCESS "del SUCCESS"

#define UPLOADFILE_READY "up_ready"
#define UPLOADFILE_SUCCESS "upload success"
#define UPLOADFILE_FAILED "upload FAILED"

#define DOWNLOADFILE_READY "DOWN_ready"
#define DOWNLOADFILE_SUCCESS "DOWNload success"
#define DOWNLOADFILE_FAILED "DOWNload FAILED"

#define SHAREFILE_SUCCESS "SHAREFILE success"
#define SHAREFILE_FAILED "SHAREFILE FAILED"

enum ENUM_MSG_TYPE
{
    ENUM_MSG_TYPE_MIN=0,
    ENUM_MSG_TYPE_REGIST_REQUEST,   //注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND,   //注册回复
    ENUM_MSG_TYPE_LOGOUT_REQUEST,   //注销请求
    ENUM_MSG_TYPE_LOGOUT_RESPOND,   //注销回复
    ENUM_MSG_TYPE_LOGIN_REQUEST,    //登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,    //登录回复
    ENUM_MSG_TYPE_ALLONLINE_REQUEST,    //查看所有用户请求
    ENUM_MSG_TYPE_ALLONLINE_RESPOND,    //查看所有用户回复
    ENUM_MSG_TYPE_SEARCH_REQUEST,    //搜索用户请求
    ENUM_MSG_TYPE_SEARCH_RESPOND,    //搜索用户回复
    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,    //加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,   //加好友回复
    ENUM_MSG_TYPE_ADD_FRIEND_AGREE,    //加好友同意
    ENUM_MSG_TYPE_ADD_FRIEND_DISAGREE,   //加好友不同意
    ENUM_MSG_TYPE_REFREASHFRIEND_REQUEST,    //刷新好友请求
    ENUM_MSG_TYPE_REFREASHFRIEND_RESPOND,   //刷新好友回复
    ENUM_MSG_TYPE_DELETEFRIEND_REQUEST,    //删除好友请求
    ENUM_MSG_TYPE_DELETEFRIEND_RESPOND,   //删除好友回复
    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST,    //私聊请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND,   //私聊回复
    ENUM_MSG_TYPE_GROUP_CHAT_REQUEST,    //群聊请求
    ENUM_MSG_TYPE_GROUP_CHAT_RESPOND,   //群聊回复
    ENUM_DIR_TYPE_CREATE_DIR_REQUEST,    //增加目录请求
    ENUM_DIR_TYPE_CREATE_DIR_RESPOND,   //添加目录回复
    ENUM_DIR_TYPE_REFREASH_DIR_REQUEST,    //刷新目录请求
    ENUM_DIR_TYPE_REFREASH_DIR_RESPOND,   //刷新目录回复
    ENUM_DIR_TYPE_DELITE_DIR_REQUEST,    //删除目录请求
    ENUM_DIR_TYPE_DELETE_DIR_RESPOND,   //删除目录回复
    ENUM_DIR_TYPE_DELITE_FILE_REQUEST,    //删除文件请求
    ENUM_DIR_TYPE_DELETE_FILE_RESPOND,   //删除文件回复
    ENUM_DIR_TYPE_RENAME_REQUEST,    //重命名请求
    ENUM_DIR_TYPE_RENAME_RESPOND,   //重命名回复

    ENUM_DIR_TYPE_UPLOAD_REQUEST,    //上传请求
    ENUM_DIR_TYPE_UPLOAD_RESPOND,   //上传回复
    ENUM_DIR_TYPE_UPLOAD_DATABLOCK,   //文件数据块上传

    ENUM_DIR_TYPE_DOWNLOAD_REQUEST,    //下载请求
    ENUM_DIR_TYPE_DOWNLOAD_RESPOND,   //下载回复

    ENUM_DIR_TYPE_SHAREFILE_REQUEST,    //分享请求
    ENUM_DIR_TYPE_SHAREFILE_RESPOND,   //分享回复
    ENUM_DIR_TYPE_IF_SHAREFILE_REQUEST,    //是否接收分享请求
    ENUM_DIR_TYPE_IF_SHAREFILE_RESPOND,   //是否接收分享回复

    ENUM_MSG_TYPR_MAX=50,
};

struct PDU
{
    uint uiPDULen;  //总的协议长度
    uint uiMsgType; //消息类型
    char caData[64];
    uint uiMsgLen;  //实际消息长度非字节数
    int caMsg[]; //实际消息
};

PDU* mkPDU(uint uiMsgLen)
{
    uint uiPDULen=sizeof(PDU)+uiMsgLen;
    PDU* pdu = (PDU*)malloc(uiPDULen);
    memset(pdu->caData,'\0',64);
    pdu->uiPDULen=uiPDULen;
    pdu->uiMsgLen=uiMsgLen;
    memset(pdu->caMsg,'\0',pdu->uiMsgLen);
    return pdu;
}

#endif // PROTOCOL_H
