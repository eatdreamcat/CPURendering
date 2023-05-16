#pragma once
#include <iostream>
#include<opencv.hpp>
#include <direct.h>


using namespace cv;

#define UINT unsigned int


inline std::string current_working_directory()
{
    char buff[512];
    _getcwd(buff, 512);
    std::string current_working_directory(buff);
    return current_working_directory;// .substr(0, current_working_directory.rfind("\\"));
}