#include "FOC.h"
#include "math.h"
#include "as5600.h"
#include "main.h"
#include "control.h"
#include "tim.h"
#include "pid.h"
#include "adc.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "adc.h"
#include "LowPass_Filter.h"
#include "ABZ.h"

extern int PP;
extern int DIR;

extern float voltage_limit;
extern float voltage_power_supply;
extern float zero_electric_angle;
extern float Ualpha, Ubeta;
extern float Ua, Ub, Uc;
extern float dc_a, dc_b, dc_c;
extern float Sensor_Angle;
extern float Sensor_Speed;

extern float ABZ_Sensor_Speed;

extern float ABZ_Sensor_Angle;

extern uint32_t adc1_Ia, adc1_Ib, adc1_Ic;
extern uint32_t adc2_Ia, adc2_Ib, adc2_Ic;
extern float current1_A;
extern float current1_B;
extern float current1_C;

extern float volts_to_amps_ratio;
extern float gain_a, gain_b, gain_c;
extern float offset_ia, offset_ib, offset_ic;
extern float i_alpha;
extern float i_beta;
extern float id;
extern float iq;
extern float id_ref;
extern float iq_ref;
extern float Ud;
extern float Uq_new;

uint16_t filtered_ADC1_Ia;
uint16_t filtered_ADC1_Ib;
uint16_t filtered_ADC1_Ic;



/*******************************************
SysTick初始化(模拟micros函数初始化)：

备注：
    1.从0xFFFFFF到0循环计数
    2.开启后无法再使用delay延时函数
*******************************************/
void Systick_CountMode(void)
{
    // 在两个系列上都通用的初始化步骤
    SysTick->LOAD = 0xFFFFFF;                                             // 计数从最大值开始
    SysTick->VAL = 0;                                                     // 清空当前计数器值
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // 使能 SysTick 定时器
}

/*******************************************
电角度求解：
备注：
    1.     电角度      =   机械角度  *  极对数
    2._electricalAngle = shaft_angle * pole_pairs
*******************************************/
float _electricalAngle()
{
    return _normalizeAngle((float)(DIR * PP) * AS5600_GetAngle2PI() - zero_electric_angle);
}
//////debug ABZ Encoder//////
float ABZ_electricalAngle()
{
    return _normalizeAngle((float)(DIR * PP) * ABZ_GetAngle2PI() - zero_electric_angle);
}

/*******************************************
角度归一化处理:
备注：放置过长时间累计误差。
*******************************************/
float _normalizeAngle(float angle)
{
    float a = fmod(angle, 2 * PI);
    return a >= 0 ? a : (a + 2 * PI);
    // 三目运算符。格式：condition ? expr1 : expr2
    // 其中,condition式要求值的条件表达式,如果条件成立,则返回 expr1 的值。可以将三目运算符看作if-else的简化形式
}

/*******************************************
PWM占空比输出计算并完成设置:
*******************************************/
void setPWM(float Ua, float Ub, float Uc)
{
    // 输出电压限幅
    Ua = _constrain(Ua, 0.0f, voltage_limit);
    Ub = _constrain(Ub, 0.0f, voltage_limit);
    Uc = _constrain(Uc, 0.0f, voltage_limit);
    // 占空比计算，输出限幅
    dc_a = _constrain(Ua / voltage_power_supply, 0.0f, 1.0f);
    dc_b = _constrain(Ub / voltage_power_supply, 0.0f, 1.0f);
    dc_c = _constrain(Uc / voltage_power_supply, 0.0f, 1.0f);
    // 电机1 6路PWM占空比设置
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, dc_a * 5500);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, dc_b * 5500);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, dc_c * 5500);

    //__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, dc_a * 5500);
    //__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, dc_b * 5500);
    //__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, dc_c * 5500);
}

/*******************************************
力矩控制:
*******************************************/
void setTorque(float Uq, float angle_el)
{
    // 力矩限幅。
    Uq = _constrain(Uq, -voltage_power_supply / 2, voltage_power_supply / 2);
    // 角度归一化处理。
    angle_el = _normalizeAngle(angle_el);
    // 帕克逆变换
    Ualpha = -Uq * sin(angle_el);
    Ubeta = Uq * cos(angle_el);
    // 克拉克逆变换
    Ua = Ualpha + voltage_power_supply / 2;
    Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + voltage_power_supply / 2;
    Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + voltage_power_supply / 2;
    // PWM赋值
    setPWM(Ua, Ub, Uc);
}

/*******************************************
FOC控制初始化:
*******************************************/
void FOC_Init(float power_supply)
{
    voltage_power_supply = power_supply; // 电源电压传参。
    // 此处增加自定义硬件初始化。
    PID_init(); // pid初始化。
    
}

/*******************************************
FOC编码器数据初始化:
*******************************************/
void FOC_AS5600_Init(int _PP, int _DIR)
{
    // 值传递。
    PP = _PP;
    DIR = _DIR;
    
    setTorque(3, _2PI);
    HAL_Delay(3000);
   
    setTorque(0, _2PI);

    zero_electric_angle = _electricalAngle(); // 设置零点角度。
    Sensor_Speed = AS5600_Get_Speed();        // 速度初始值计算。
}
/////ABZ Encoder debug/////
void FOC_ABZ_Init(int _PP, int _DIR)
{
    // 值传递。
    PP = _PP;
    DIR = _DIR;
    
    setTorque(2, _2PI);
    HAL_Delay(3000);
   
    setTorque(0, _2PI);

    zero_electric_angle = ABZ_electricalAngle(); // 设置零点角度。
    ABZ_Sensor_Speed = ABZ_Get_Speed();        // 速度初始值计算。
}

/***********************************************
电机角度控制:(rad)
***********************************************/
// ABZ编码器
void ABZ_Set_Angle(float Angle)
{
    // ABZ编码器角度读取。
    ABZ_Sensor_Angle = ABZ_GetAngle();
    // 角度控制。
    Angle_Out = Angle_Control((Angle - DIR *  ABZ_Sensor_Angle) * 180.0f / PI);
    // 控制力矩限幅。
    Moment_limiting();
    // 设置力矩。
    setTorque(Angle_Out, ABZ_electricalAngle());

    // 角度串口打印输出调试
    // printf("%.2f,%.2f\n",Sensor_Angle,Angle_target);
}
// AS5600编码器
void Set_Angle(float Angle)
{
    // AS5600角度读取。
    Sensor_Angle = AS5600_GetAngle();
    // 角度控制。
    Angle_Out = Angle_Control((Angle - DIR * Sensor_Angle) * 180.0f / PI);
    // 控制力矩限幅。
    Moment_limiting();
    // 设置力矩。
    setTorque(Angle_Out, _electricalAngle());

    // 角度串口打印输出调试
    // printf("%.2f,%.2f\n",Sensor_Angle,Angle_target);
}

/***********************************************
电机速度控制:(rad/s)
***********************************************/
// ABZ编码器
 void ABZ_Set_Speed(float Speed)
 {
     // 读取ABZ编码器解算后的数据。
     ABZ_Sensor_Speed = ABZ_Get_Speed();
     // 速度PID控制
     Speed_Out = Speed_Control(Speed - ABZ_Sensor_Speed);
     // 控制力矩限幅
     Moment_limiting();
     // 驱动器力矩控制
     setTorque(Speed_Out, ABZ_electricalAngle());
    
     // 速度串口打印输出调试
     // printf("%.2f,%.2f\n",Sensor_Speed,Speed_target);
 }
// AS5600编码器
void Set_Speed(float Speed)
{
    // AS5600读取解算后的数据。
    Sensor_Speed = AS5600_Get_Speed();
    // 速度PID控制
    Speed_Out = Speed_Control(Speed - Sensor_Speed);
    // 控制力矩限幅
    Moment_limiting();
    // 驱动器力矩控制
    setTorque(Speed_Out, _electricalAngle());
    
    // 速度串口打印输出调试
    // printf("%.2f,%.2f\n",Sensor_Speed,Speed_target);
}



/***********************************************
电机开环旋转:
***********************************************/
void Open_Loop_Control(float Uq, float speed)
{
    // 上位机接收电流波形值。
    // char buffer[50]; // 用于存储转换后的字符串
    // int length = 0;
    float angle_el; // 初始化电角度

    while (1)
    {

        // Read_ADC1_Values();
        Send_ADC1_Values(&huart1); // 发送 ADC1 的值到上位机
        // 更新电角度
        angle_el += speed;
        angle_el = _normalizeAngle(angle_el);
        // 设置力矩
        setTorque(Uq, angle_el);

        // 获取当前ADC值
        // 调试打印：确保ADC值已经被正确读取
        // HAL_UART_Transmit(&huart1, "Toggle LED0!\r\n", sizeof("Toggle LED0!\r\n"),10000);
        // HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart1, uint8_t *adc1_Ia, sizeof(adc1_Ia), 1);
        // length = snprintf(buffer, sizeof(buffer), "ADC1_Ia: %lu\r\n", adc2_Ia);
        // HAL_UART_Transmit(&huart1, (uint8_t *)buffer, length, HAL_MAX_DELAY);
        // 延时，确保控制频率

        HAL_Delay(1); // 根据实际情况调整延时
    }
}

/***********************************************
 查找 ADC 零偏移量的函数:
***********************************************/
void calibrateOffsets()
{
    const int calibration_rounds = 1000;

    // 查找0电流时候的电压
    offset_ia = 0;
    offset_ib = 0;
    offset_ic = 0;
    // 读数1000次
    for (int i = 0; i < calibration_rounds; i++)
    {
        offset_ia += adc1_Ia * (3.3f / 4095.0f); // 把原始值算出实际电压值累加进offset_ia
        offset_ib += adc1_Ib * (3.3f / 4095.0f);
        offset_ic += adc1_Ic * (3.3f / 4095.0f);
    }
    // 求平均，得到误差
    offset_ia = offset_ia / calibration_rounds;
    offset_ib = offset_ib / calibration_rounds;
    offset_ic = offset_ic / calibration_rounds;

    // 实际示波器测量500mv对应5A，所以x10倍就达到了对应的电流。
    volts_to_amps_ratio = 1.0f / 0.01f / 10; // 1/(0.01o+10) 设置采样电阻和运算放大器增益。

    // 设置电流正负值。
    gain_a = volts_to_amps_ratio * -1;
    gain_b = volts_to_amps_ratio * -1;
    gain_c = volts_to_amps_ratio * -1;
}

/***********************************************
反馈电流计算
***********************************************/
void Feedback_Current()
{
    // 计算滑动滤波处理后的电流，（（求出实际电压）-偏执电压）*增益
    current1_A = ((filtered_ADC1_Ia * (3.3f / 4095.0f)) - offset_ia) * gain_a;
    current1_B = ((filtered_ADC1_Ib * (3.3f / 4095.0f)) - offset_ib) * gain_b;
    current1_C = ((filtered_ADC1_Ic * (3.3f / 4095.0f)) - offset_ic) * gain_c;
}

/***********************************************
电机电流环力矩控制:
***********************************************/
void Current_Speed(float Uq, float speed)
{

    float angle_el = 0;
    calibrateOffsets(); // 校准电机并算出电流误差
    while (1)
    {
        // uint8_t Senbuff[] = "Serial Output Message by DMA! \r\n";  //定义数据发送数组
        Send_ADC1_Values(&huart1); // 串口1 阻塞发送与DMA发送函数。

        angle_el += speed;                    // 开环累加更新电角度。
        angle_el = _normalizeAngle(angle_el); // 角度归一化处理。

        Current_Loop(Uq, angle_el); // 设置电流环参数。
        //HAL_Delay(1);               // 延时。
    }
}
/***********************************************
串口发送ADC1值
备注：待测试
***********************************************/
void Send_ADC1_Values(UART_HandleTypeDef *huart)
{
    char buffer[100];
    int len;

    Read_ADC1_Values(); // 首先读取原始ADC采样值。
    // uint8_t Senbuff[] = " DMA OK! \r\n";  //定义数据发送测试数组。

    //  读取 ADC1 的值。
    uint16_t new_adc1_Ia = adc1_Ia;
    uint16_t new_adc1_Ib = adc1_Ib;
    uint16_t new_adc1_Ic = adc1_Ic;

    // 更新滤波器并获取滤波后的值。
    filtered_ADC1_Ia = ADC_Filter_Update(&adc1_filter_Ia, new_adc1_Ia);
    filtered_ADC1_Ib = ADC_Filter_Update(&adc1_filter_Ib, new_adc1_Ib);
    filtered_ADC1_Ic = ADC_Filter_Update(&adc1_filter_Ic, new_adc1_Ic);

    Feedback_Current(); // 读取算好的反馈电流。

    // 格式化字符串，将 ADC1 的值转换为字符串，打印滤波后的值。
    len = snprintf(buffer, sizeof(buffer), "%u, %u, %u\n", filtered_ADC1_Ia, filtered_ADC1_Ib, filtered_ADC1_Ic);

    // 通过串口发送字符串到上位机
    HAL_UART_Transmit(huart, (uint8_t *)buffer, len, 3); // 功能正常，打印速度慢，去除后两个参数正常。
    // HAL_UART_Transmit_DMA(&huart, (uint8_t *)Senbuff, sizeof(Senbuff));//DMA传输正常。
    // HAL_Delay(1);
}
/***********************************************
//电流环控制
***********************************************/
void Current_Loop(float Uq, float angle_el)
{
    // 将三相电流转换为α-β坐标系。
    // 使用Clarke变换将三相电流（abc）转换为α-β坐标系的电流（iα, iβ）。
    i_alpha = current1_A;
    i_beta = (current1_A + 2 * current1_B) / sqrt(3.0f);

    // 将α-β坐标系的电流转换为d-q坐标系。
    // 使用Park变换将α-β坐标系的电流（iα, iβ）转换为d-q坐标系的电流（id, iq）。

    id = i_alpha * cos(angle_el) + i_beta * sin(angle_el);
    iq = -i_alpha * sin(angle_el) + i_beta * cos(angle_el);

    id_ref = 0.0f; // d轴电流参考值为0
    iq_ref = Uq;   // q轴电流参考值为目标力矩

    // PID控制计算
    Ud = Current_Control(id_ref - id);
    Uq_new = Current_Control(iq_ref - iq);

    // 使用逆Park变换计算α-β坐标系的电压
    Ualpha = Ud * cos(angle_el) - Uq_new * sin(angle_el);
    Ubeta = Ud * sin(angle_el) + Uq_new * cos(angle_el);
    // 将α-β坐标系的电压转换为三相电压。
    // 使用逆Clarke变换将α-β坐标系的电压（vα, vβ）转换为三相电压（va, vb, vc）。

    // 克拉克逆变换。
    Ua = Ualpha + voltage_power_supply / 2;
    Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + voltage_power_supply / 2;
    Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + voltage_power_supply / 2;

    // 将三相电压应用于逆变器。
    // 将计算出的三相电压应用于逆变器的PWM信号上。
    // PWM赋值。
    setPWM(Ua, Ub, Uc);
}

// ///////////////////////////////////////////////////////////////////////////
// /***********************************************
// 棘轮(Ratchet_Wheel)模式：调整比例P值设置不同刚度(rigidity)
// DFOC_M0_setTorque(Kp*(target-DFOC_M0_Angle()));

// 力矩设置：DFOC_M0_setTorque
// 换算成力矩的系数：Kp
// 要锁定位置的目标角度：target
// 获取编码器读到的电机当前角度：DFOC_M0_Angle
// ***********************************************/

// /*arduino dengfoc
// float attractor_distance = 45*PI/180.0;
// float target = round(DFOC_M0_Angle() / attractor_distance) * attractor_distance;
// DFOC_M0_setTorque(Kp * (target - DFOC_M0_Angle()));
// */

// void Ratchet_Wheel(float Angle)
// {
//     // 8个顿挫点,角度值变成弧度值。
//     float attractor_distance = Angle * PI / 180.0f;
//     // 设置P值,电机抖动增加。
//     float P = 5;
//     // 计算目标值。
//     float target = round(AS5600_GetAngle() / attractor_distance) * attractor_distance;
//     // 刚度控制。
//     setTorque(P * (target - AS5600_GetAngle()), _electricalAngle());
// }

// /***********************************************
// 边界限制力矩(Boundary_Moment_limitation)模式：
// 1，读取当前电机角度
// 2，设定允许旋钮的转动范围
// 3，将角度制转换为弧度制

// ***********************************************/

// void Boundary_Moment_limitation(float angle_range)
// {
//     float P = 5;
//     // 设定允许旋钮的转动范围 90°
//     // float angle_range = 90.0;
//     // 设定允许旋钮的转动范围中间值 45°
//     float angle_range2 = angle_range / 2;
//     // 中间值转弧度制
//     float limit_range = angle_range2 / 360.0f * _2PI;

//     float L1 = zero_electric_angle + limit_range;
//     float L2 = zero_electric_angle - limit_range;

//     if (AS5600_GetAngle() > L2 && AS5600_GetAngle() < L1)
//     {
//         setTorque(0, _electricalAngle()); // 无动作
//         // 6个顿挫点,角度值变成弧度值
//         // float attractor_distance = 15 * PI / 180.0;
//         // 设置P值
//         // float P = 5;
//         // float target = round(get_Angle()/attractor_distance)*attractor_distance;
//         //刚度控制
//         setTorque(P*(target - get_Angle()),_electricalAngle());
//     }
//     else
//     {

//         if (AS5600_GetAngle() < L2)
//             setTorque(P * (L2 - AS5600_GetAngle()), _electricalAngle());
//         if (AS5600_GetAngle() > L1)
//             setTorque(P * (L1 - AS5600_GetAngle()), _electricalAngle());
//     }
// }

/***********************************************
阻尼(damping)模式：
***********************************************/

/***********************************************
顺滑(smooth)模式：
***********************************************/

/***********************************************
失重(weightlessness)模式：
***********************************************/
