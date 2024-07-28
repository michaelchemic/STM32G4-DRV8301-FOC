# ReadMe

<!--Writerside adds this topic when you create a new documentation project.
You can use it as a sandbox to play with Writerside features, and remove it from the TOC when you don't need it anymore.-->

## About this document
此文档将会详细解析软件上各个模块功能，FOC控制原理，硬件电路相关内容。

## 第一章 硬件部分（Hardware）
### 使用的硬件设备
该项目使用 ST公司生产的 STM32G474RE-Nucleo-64 评估板，TI 生产的 BOOSTXL-DRV8301 电机驱动板为主要开发套件。

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/STM32G474-Nucleo.png)

相关资料请访问：（https://www.st.com/en/evaluation-tools/nucleo-g474re.html)

（ST的开发板在接口和设计上是深入思考后的产品，在开发中节约了很多时间）

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/TI%20DRV8301.png)

相关资料请访问：(https://www.ti.com.cn/tool/cn/BOOSTXL-DRV8301)

* 由于这是一个前期验证的项目，硬件电路就先使用杜邦线连接了，后期完善后将会重新设计一块PCB。

### MCU 管脚分配

此项目使用了ST的cubeMX进行硬件上面的管脚分配，cubemx使用起来十分方便，强烈推荐。

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/st%20cubeMX.png))

### 接线图与实物图
<!--todo 需要增加实物接线图-->

## 第二章 软件部分

### FOC算法（FOC algorithm）

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/FOC%E8%AE%A1%E7%AE%97%E8%BF%87%E7%A8%8B.png)

#### clack变换
![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/Clarke%E5%8F%98%E6%8D%A2.png)

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/%E7%94%B5%E6%B5%81%E7%8E%AF.png)

本项目暂时使用的是SPWM调制的正弦波，想要提高效率请使用SVPWM调制马鞍波形（效率可以提高15%）。

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/%E5%9D%90%E6%A0%87%E5%8F%98%E6%8D%A2.png)

1，把基变换将其正交化为一个直角坐标系，把三相电流波形的相位变化变成二轴坐标系中α和β矢量长短变化。

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/%E6%AD%A3%E5%BC%A6%E4%B8%8E%E4%BD%99%E5%BC%A6.png)

2，三角函数投影计算过程

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/%E4%B8%89%E8%A7%92%E5%87%BD%E6%95%B0%E6%8A%95%E5%BD%B1.png)

三角函数复习：

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/%E4%B8%89%E8%A7%92%E5%87%BD%E6%95%B0%E5%A4%8D%E4%B9%A0%E8%B5%84%E6%96%99.png)

摘自：[](https://www.cnblogs.com/sunsky303/p/17248071.html)

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/Clarke%E5%8F%98%E6%8D%A2.png)

3，Clarke变换写成矩阵形式

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/Clarke%E5%8F%98%E6%8D%A2%E5%86%99%E6%88%90%E7%9F%A9%E9%98%B5%E5%BD%A2%E5%BC%8F.png)

4，此为变换后的波形对比图

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/clarke%20%E5%8F%98%E6%8D%A2%E7%94%9F%E6%88%90%E7%9A%84%E6%B3%A2%E5%BD%A2%E4%B8%8E%E5%8E%9F%E6%B3%A2%E5%BD%A2%E4%B8%80%E8%87%B4.png)

参考资料：(https://zhuanlan.zhihu.com/p/147659820)

#### Park变换（将变量线性化）

1，让转子旋转起来！

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/%E6%97%8B%E8%BD%AC%E4%BA%8C%E7%BB%B4%E5%9D%90%E6%A0%87%E7%B3%BB.png)

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/%E6%97%8B%E8%BD%AC%E7%9A%84%E5%9D%90%E6%A0%87%E7%B3%BB%E6%98%A0%E5%B0%84.png)

2，写成矩阵形式：

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/Park%E5%8F%98%E6%8D%A2%E7%9F%A9%E9%98%B5%E6%8F%8F%E8%BF%B0%E5%BD%A2%E5%BC%8F.png)

3，Park变换前后波形对比，已经可以从设置正弦波变为设置Iq 和 Id了。

![image](https://github.com/michaelchemic/STM32G4-DRV8301-FOC/blob/main/Readme/Writerside/images/Park%E5%8F%98%E6%8D%A2%E5%89%8D%E5%90%8E%E6%B3%A2%E5%BD%A2%E5%AF%B9%E6%AF%94.png)

4，Iq代表了期望的力矩输出， Id不需要的，我们希望尽可能把它控制为0。


