# ReadMe

<!--Writerside adds this topic when you create a new documentation project.
You can use it as a sandbox to play with Writerside features, and remove it from the TOC when you don't need it anymore.-->

## About this document
此文档将会详细解析软件上各个模块功能，FOC控制原理，硬件电路相关内容。

## 第一章 硬件部分（Hardware）
### 使用的硬件设备
该项目使用 ST公司生产的 STM32G474RE-Nucleo-64 评估板，TI 生产的 BOOSTXL-DRV8301 电机驱动板为主要开发套件。

![](STM32G474-Nucleo.png)

相关资料请访问：
[](https://www.st.com/en/evaluation-tools/nucleo-g474re.html)
（ST的开发板在接口和设计上是深入思考后的产品，在开发中节约了很多时间）

![](TI DRV8301.png)

相关资料请访问：
[](https://www.ti.com.cn/tool/cn/BOOSTXL-DRV8301)

* 由于这是一个前期验证的项目，硬件电路就先使用杜邦线连接了，后期完善后将会重新设计一块PCB。

### MCU 管脚分配

此项目使用了ST的cubeMX进行硬件上面的管脚分配，cubemx使用起来十分方便，强烈推荐。

![stm32cubemx](st cubeMX.png)

### 接线图与实物图
<!--todo 需要增加实物接线图-->

## 第二章 软件部分

### FOC算法（FOC algorithm）

![FOC计算过程.png](FOC计算过程.png)

#### clack变换
![foc控制流程图.png](foc控制流程图.png)

![电流环.png](电流环.png)

本项目暂时使用的是SPWM调制的正弦波，想要提高效率请使用SVPWM调制马鞍波形（效率可以提高15%）。

![坐标变换.png](坐标变换.png)

1，把基变换将其正交化为一个直角坐标系，把三相电流波形的相位变化变成二轴坐标系中α和β矢量长短变化。

![正弦与余弦.png](正弦与余弦.png)

2，三角函数投影计算过程

![三角函数投影.png](三角函数投影.png)

三角函数复习：

![三角函数复习资料.png](三角函数复习资料.png)

摘自：[](https://www.cnblogs.com/sunsky303/p/17248071.html)

![Clarke变换.png](Clarke变换.png)

3，Clarke变换写成矩阵形式

![Clarke变换写成矩阵形式.png](Clarke变换写成矩阵形式.png)

4，此为变换后的波形对比图

![clarke 变换生成的波形与原波形一致.png](clarke 变换生成的波形与原波形一致.png)

参考资料：
[](https://zhuanlan.zhihu.com/p/147659820)

#### Park变换（将变量线性化）

1，让转子旋转起来！

![旋转二维坐标系.png](旋转二维坐标系.png)

![旋转的坐标系映射.png](旋转的坐标系映射.png)

2，写成矩阵形式：

![Park变换矩阵描述形式.png](Park变换矩阵描述形式.png)

3，Park变换前后波形对比，已经可以从设置正弦波变为设置Iq 和 Id了。

![Park变换前后波形对比.png](Park变换前后波形对比.png)

4，Iq代表了期望的力矩输出， Id不需要的，我们希望尽可能把它控制为0。

