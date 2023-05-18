// Cpu Rendering.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"
#include "GPUDevice.h"

#ifndef RUN_TIME
#define RUN_TIME 1
#endif // !RUN_TIME


int main()
{

#if RUN_TIME
    {

        using namespace std;

        auto width = 1280;
        auto height = 720;

        GPUDevice gpu;




        gpu.InitWindow("Cpu Rendering");
        gpu.CreateFrameBuffers(height, width);

        auto keyCode = 0;



        while (keyCode != 27) {


            gpu.BeforeRendering();
            gpu.Clear();

            gpu.OnRendering();
            gpu.Present();
            keyCode = pollKey();
        
        }

      /*  gpu.BeforeRendering();
        gpu.Clear();

        gpu.OnRendering();
        gpu.Present();*/

        waitKey();
        destroyAllWindows();

        /* auto p = new int*[2];
         p[0] = new int;
         p[1] = new int;
         delete p[0];
         delete p[1];
         delete []p;*/
    }
#else
    {
        using namespace std;

        auto start = clock();

        const int threadCount = 10;
        thread t[threadCount];
        

        atomic<UINT> n = 0;
        atomic<UINT> complete = 0;
        atomic<bool> flag = false;
        std::mutex kMutex;
       
        for (int i = 0; i < threadCount; ++i) {
            t[i] = thread([&n, &flag, &kMutex, &complete](int index){

               
                for (int j = 0; j < 10000000; j++) {
                    n++;    
                    {
                        if (!flag) {
                            std::lock_guard<std::mutex> lock(kMutex);
                            if (n == 999999) {
                                flag = true;
                                n++;

                            }
                        }
                    }
                }

                ++complete;

                }, i);
            t[i].detach();
        }

        thread wait = thread([&complete,&threadCount]() {
            while (complete < threadCount);
        });
       
        wait.join();
        cout << "cost:" << clock() - start << ", value:" << n << endl;
       

    }
#endif

    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();

    return 0;
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
