#ifndef _DIR_H
#define _DIR_H
#include<dirent.h>
#include<iostream>
#include<vector>

class opeDIR
{
    DIR* dir;
    std::string currentpath;
public:
    opeDIR(std::string path);

    std::vector<dirent> getCurrentFileInfo();

    ~opeDIR();
};



#endif