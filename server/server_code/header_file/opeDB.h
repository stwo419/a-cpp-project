#include<iostream>
#include<mysql/mysql.h>
#include<string>
#include<vector>
class opeDB
{
    MYSQL* mysql;
    std::string host;
    std::string username;
    std::string pwd;
    std::string DBname;
    uint port;
    bool isconnected;
public:
    opeDB();
    void connectDB();
    bool opeCommand(const std::string &cmd);
    std::vector<std::string> opecommand(const std::string &cmd,int); 
    
    std::pair<int,int> isFriend(std::string &name,std::string &peername);

    void closecon();

    ~opeDB();
};