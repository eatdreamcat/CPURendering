# CPU Rendering

## 2023.5.15 项目创建

主要是搭建了 Opencv 的环境

https://github.com/eatdreamcat/CPURendering/releases/tag/2023.5.15

## 2023.5.17 双 Buffer，FPS，Clear

为了能更好的模拟 CPU 跟 GPU 的交互
我创建了 GPUDevice 类

在 main 中添加了主循环，实现了在循环中交换 BackBuffer 跟 FrontBuffer，也做了帧率的显示，方便后期进行优化的时候看帧率。
TODO：
用多线程来模拟

https://github.com/eatdreamcat/CPURendering/releases/tag/2023.5.17

## 2023.5.18 多线程学习

今天计划先支持一下多线程需求，后面可能会用来模拟 GPU 对屏幕分块执行的过程
这里遇到一个问题，代码如下：

```
        atomic<UINT> n = 0;

        for (int i = 0; i < threadCount; ++i) {
            t[i] = thread([&n](int index){

                for (int j = 0; j < 10000000; j++) {
                    n++;

                    if (n == 999999) {

                        n++;
                    }
                }

                }, i);
        }


```

判断并不是原子操作。理论上 n 的值应该是 10000001，但是实际结果却有可能是 10000002.
这里判断满足后再+1 的操作，至少是有两个指令，也就是说，当线程 1 满足这个判断，但是还没执行 n++，这一瞬间，线程 2 也执行了判断语句，也满足，于是最终结果就是会有两个线程都额外执行了一次 n++。

这里想到一个办法，用 lock_guard 在一个作用域内上锁，将判断放到这个作用域内，但是我的代码里有很长的循环，每一次循环都要加锁，会导致性能特别差，所以又加了一个原子 flag 变量来记录
但是不知道是不是还有更好的方案

```
  atomic<UINT> n = 0;
        atomic<bool> flag = false;
        std::mutex kMutex;

        for (int i = 0; i < threadCount; ++i) {
            t[i] = thread([&n, &flag, &kMutex](int index){


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

                }, i);
        }

```

写以上这些代码，也只是想初步认识下 C++的多线程，至于后面 Rendering 的时候会不会遇到这种问题再看吧。。。

https://github.com/eatdreamcat/CPURendering/releases/tag/2023.5.18

## 2023.5.19 通过 y=kx+b 绘制直线

这个绘制线条只是测试一下，实际不能用这种方案
一是效率问题，二是不好处理垂直 x 轴的线，因为此时 k 不存在

TODO：
使用 Bresenham 来画线
使用 scanline 填充

https://github.com/eatdreamcat/CPURendering/releases/tag/2023.5.19

## 2023.5.21 通过 Bresenham 算法绘制线条

相比直接用直线公式绘制，bresenham 的计算中没有乘法，只有加法
因此更加高效以及易于硬件实现
本身是离散化以统计学的思想来做的，比如绘制的时候，x 确定下，y 只有两种可能性，因此直接判断可能性。

https://github.com/eatdreamcat/CPURendering/releases/tag/2023.5.21


## 2023.5.22 FBX加载，以及Mesh数据的获取

这里只是简单的Mesh获取，递归的Traver场景，将每个Mesh的vertex跟indices设置到vbo
TODO:
normals，tangents，uv，vertexColor
culling，cliping，transform
optimization

https://github.com/eatdreamcat/CPURendering/releases/tag/2023.5.22