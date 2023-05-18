# CPU Rendering

## 2023.5.15 项目创建

主要是搭建了Opencv的环境

https://github.com/eatdreamcat/CPURendering/releases/tag/2023.5.15

## 2023.5.17 双Buffer，FPS，Clear

为了能更好的模拟CPU跟GPU的交互
我创建了GPUDevice类

在main中添加了主循环，实现了在循环中交换BackBuffer跟FrontBuffer，也做了帧率的显示，方便后期进行优化的时候看帧率。
TODO：
 用多线程来模拟

https://github.com/eatdreamcat/CPURendering/releases/tag/2023.5.17


## 2023.5.18 多线程

今天计划先支持一下多线程需求，后面可能会用来模拟GPU对屏幕分块执行的过程
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

判断并不是原子操作。理论上n的值应该是10000001，但是实际结果却有可能是10000002.
这里判断满足后再+1的操作，至少是有两个指令，也就是说，当线程1满足这个判断，但是还没执行n++，这一瞬间，线程2也执行了判断语句，也满足，于是最终结果就是会有两个线程都额外执行了一次n++。

这里想到一个办法，用lock_guard在一个作用域内上锁，将判断放到这个作用域内，但是我的代码里有很长的循环，每一次循环都要加锁，会导致性能特别差，所以又加了一个原子flag变量来记录
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

写以上这些代码，也只是想初步认识下C++的多线程，至于后面Rendering的时候会不会遇到这种问题再看吧。。。
