﻿// GAMES101.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<opencv.hpp>
#include <direct.h>


std::string current_working_directory()
{
    char buff[512];
    _getcwd(buff, 512);
    std::string current_working_directory(buff);
    return current_working_directory;// .substr(0, current_working_directory.rfind("\\"));
}

int main()
{
	{
		using namespace std;
		using namespace cv;
        

  

        cout << "Hello World:" << current_working_directory() << endl;

        Mat src = imread(current_working_directory()+ "\\GAMES101.jpg", IMREAD_COLOR);

        if (src.empty()) {
            cout << "图片未能打开";
        }
        namedWindow("GAMES101", WINDOW_AUTOSIZE);
        imshow("GAMES101", src);
        waitKey(0);
        destroyAllWindows();

        return 0;

      
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
