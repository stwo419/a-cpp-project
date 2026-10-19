#include"opeDB.h"

opeDB::opeDB()
{
    mysql=mysql_init(nullptr);
    this->host="127.0.0.1";
    this->username="root";
    this->pwd="@Gyq322419";
    this->DBname="account";
    port=3306;
    this->isconnected=false;
}

void opeDB::connectDB()
{
    if(!mysql_real_connect(this->mysql,host.c_str(),username.c_str(),pwd.c_str(),DBname.c_str(),port,nullptr,0))
    {
        std::cerr << "数据库连接失败：" << mysql_error(mysql) << std::endl;
        mysql_close(mysql);
        return;
    }
    this->isconnected=true;
}

bool opeDB::opeCommand(const std::string &cmd)
{
    // std::cout<<"mysql:";
    // std::cout<<cmd<<std::endl;
    if(!this->isconnected)
    {
        this->connectDB();
    }
    std::string head=cmd.substr(0,cmd.find(" "));
    int ret = mysql_query(this->mysql,cmd.c_str());
    //std::cout<<ret<<std::endl;
    if(ret!=0)
    {
        std::cerr << "sql执行失败：" << mysql_error(mysql) << std::endl;
        return false;
    }
    else
    {
        MYSQL_RES* res=mysql_store_result(this->mysql);
        if((head=="update"||head=="insert"||head=="delete")&&res==nullptr)
        {
            mysql_free_result(res);
            return true;
        }
        if((head=="select"||head=="select*")&&mysql_num_rows(res)>0)
        {
            mysql_free_result(res);
            return true;
        }
        else
        {
            mysql_free_result(res);
            return false;
        }
    }
}

void opeDB::closecon()
{
    this->isconnected=false;
}

opeDB::~opeDB()
{
    mysql_close(this->mysql);
    mysql=nullptr;
}

std::vector<std::string> opeDB::opecommand(const std::string &cmd,int)
{
    std::vector<std::string> res;
    if(!this->isconnected)
    {
        this->connectDB();
    }
    int ret=mysql_query(this->mysql,cmd.c_str());
    if(ret!=0)
    {
        std::cout<<"sql:"<<cmd<<"执行失败\n";
        return res;
    }

    MYSQL_RES* mRes=mysql_store_result(this->mysql);
    if(mRes==nullptr)
    {
        std::cout<<"sql:"<<cmd<<"结果集获取失败\n";
        return res;
    }
    MYSQL_ROW row;
    int index=-1;
    while((row=mysql_fetch_row(mRes))!=nullptr)
    {
        res.push_back("");
        index++;
        int n=mysql_num_fields(mRes);
        for(int i=0;i<n;++i)
        {
            res[index].append(row[i]);
            res[index].append(" ");
        }
    }

    mysql_free_result(mRes);
    return res;
}

std::pair<int,int> opeDB::isFriend(std::string &name,std::string &peername)//返回值fist friend second online
{
    int isFriend=-1,isOnline=-1;
    std::string cmd="select * from userFriend where (m_id=(select id from userinfo where name=\'"+name+"\') and fr_id=(select id from userinfo where name=\'"+peername+"\')) or (fr_id=(select id from userinfo where name=\'"+name+"\') and m_id=(select id from userinfo where name=\'"+peername+"\'))";
    int ret=mysql_real_query(this->mysql,cmd.c_str(),cmd.length());
    if(ret!=0)
    {
        std::cout<<"isFriend: sql执行失败\n";
        return std::make_pair(isFriend,isOnline);
    }
    MYSQL_RES* res=mysql_store_result(this->mysql);
    if(mysql_num_rows(res)>0)
    {
        isFriend=1;
    }
    else
        isFriend=0;
    mysql_free_result(res);
    cmd="select stat from userinfo where name=\'"+peername+"\';";
    //std::cout<<cmd<<" "<<peername<<"\n";
    std::vector<std::string> stat=this->opecommand(cmd,1);
    std::cout<<stat.empty()<<"\n";
    if(!stat.empty())
    {
        std::string temp=stat[0].substr(0,stat[0].find(" "));
        isOnline=temp=="1"?1:0;
    }
    else
        isOnline=2;
    return std::make_pair(isFriend,isOnline);
}
