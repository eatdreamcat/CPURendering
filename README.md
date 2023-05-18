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