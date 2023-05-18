#pragma once
#include <iostream>
#include<opencv.hpp>
#include <opencv2/core/opengl.hpp>
#include <direct.h>
#include <time.h>
#include <cmath>
#include <thread>
#include <wrl.h>
#include <shellapi.h>

#include <vector>

#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_LEAK_CHECK_DF
#include <stdlib.h>

#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif


using namespace cv;

#define UINT unsigned int


inline std::string current_working_directory()
{
    char buff[512];
    _getcwd(buff, 512);
    std::string current_working_directory(buff);
    return current_working_directory;// .substr(0, current_working_directory.rfind("\\"));
}