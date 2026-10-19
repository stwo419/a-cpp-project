#include"opeDIR.h"
opeDIR::opeDIR(std::string path)
{
    this->currentpath=path;
    this->dir=opendir(this->currentpath.c_str());
    if(this->dir==nullptr)
    {
        std::cout<<"open dir failed\n";
    }
}

std::vector<dirent> opeDIR::getCurrentFileInfo()
{
    if(dir==nullptr)
        return {};
    std::vector<dirent> ans;
    dirent *dirent=nullptr;
    while((dirent=readdir(this->dir))!=nullptr)
    {
        ans.push_back(*dirent);
    }
    return ans;
}

opeDIR::~opeDIR()
{
    if(this->dir!=nullptr)
    {
        closedir(this->dir);
    }
}