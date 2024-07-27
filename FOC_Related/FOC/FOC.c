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
SysTick��ʼ��(ģ��micros������ʼ��)��

��ע��
    1.��0xFFFFFF��0ѭ������
    2.�������޷���ʹ��delay��ʱ����
*******************************************/
void Systick_CountMode(void)
{
    // ������ϵ���϶�ͨ�õĳ�ʼ������
    SysTick->LOAD = 0xFFFFFF;                                             // ���������ֵ��ʼ
    SysTick->VAL = 0;                                                     // ��յ�ǰ������ֵ
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk; // ʹ�� SysTick ��ʱ��
}

/*******************************************
��Ƕ���⣺
��ע��
    1.     ��Ƕ�      =   ��е�Ƕ�  *  ������
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
�Ƕȹ�һ������:
��ע�����ù���ʱ���ۼ���
*******************************************/
float _normalizeAngle(float angle)
{
    float a = fmod(angle, 2 * PI);
    return a >= 0 ? a : (a + 2 * PI);
    // ��Ŀ���������ʽ��condition ? expr1 : expr2
    // ����,conditionʽҪ��ֵ���������ʽ,�����������,�򷵻� expr1 ��ֵ�����Խ���Ŀ���������if-else�ļ���ʽ
}

/*******************************************
PWMռ�ձ�������㲢�������:
*******************************************/
void setPWM(float Ua, float Ub, float Uc)
{
    // �����ѹ�޷�
    Ua = _constrain(Ua, 0.0f, voltage_limit);
    Ub = _constrain(Ub, 0.0f, voltage_limit);
    Uc = _constrain(Uc, 0.0f, voltage_limit);
    // ռ�ձȼ��㣬����޷�
    dc_a = _constrain(Ua / voltage_power_supply, 0.0f, 1.0f);
    dc_b = _constrain(Ub / voltage_power_supply, 0.0f, 1.0f);
    dc_c = _constrain(Uc / voltage_power_supply, 0.0f, 1.0f);
    // ���1 6·PWMռ�ձ�����
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, dc_a * 5500);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, dc_b * 5500);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, dc_c * 5500);

    //__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, dc_a * 5500);
    //__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, dc_b * 5500);
    //__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, dc_c * 5500);
}

/*******************************************
���ؿ���:
*******************************************/
void setTorque(float Uq, float angle_el)
{
    // �����޷���
    Uq = _constrain(Uq, -voltage_power_supply / 2, voltage_power_supply / 2);
    // �Ƕȹ�һ������
    angle_el = _normalizeAngle(angle_el);
    // ������任
    Ualpha = -Uq * sin(angle_el);
    Ubeta = Uq * cos(angle_el);
    // ��������任
    Ua = Ualpha + voltage_power_supply / 2;
    Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + voltage_power_supply / 2;
    Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + voltage_power_supply / 2;
    // PWM��ֵ
    setPWM(Ua, Ub, Uc);
}

/*******************************************
FOC���Ƴ�ʼ��:
*******************************************/
void FOC_Init(float power_supply)
{
    voltage_power_supply = power_supply; // ��Դ��ѹ���Ρ�
    // �˴������Զ���Ӳ����ʼ����
    PID_init(); // pid��ʼ����
    
}

/*******************************************
FOC���������ݳ�ʼ��:
*******************************************/
void FOC_AS5600_Init(int _PP, int _DIR)
{
    // ֵ���ݡ�
    PP = _PP;
    DIR = _DIR;
    
    setTorque(3, _2PI);
    HAL_Delay(3000);
   
    setTorque(0, _2PI);

    zero_electric_angle = _electricalAngle(); // �������Ƕȡ�
    Sensor_Speed = AS5600_Get_Speed();        // �ٶȳ�ʼֵ���㡣
}
/////ABZ Encoder debug/////
void FOC_ABZ_Init(int _PP, int _DIR)
{
    // ֵ���ݡ�
    PP = _PP;
    DIR = _DIR;
    
    setTorque(2, _2PI);
    HAL_Delay(3000);
   
    setTorque(0, _2PI);

    zero_electric_angle = ABZ_electricalAngle(); // �������Ƕȡ�
    ABZ_Sensor_Speed = ABZ_Get_Speed();        // �ٶȳ�ʼֵ���㡣
}

/***********************************************
����Ƕȿ���:(rad)
***********************************************/
// ABZ������
void ABZ_Set_Angle(float Angle)
{
    // ABZ�������Ƕȶ�ȡ��
    ABZ_Sensor_Angle = ABZ_GetAngle();
    // �Ƕȿ��ơ�
    Angle_Out = Angle_Control((Angle - DIR *  ABZ_Sensor_Angle) * 180.0f / PI);
    // ���������޷���
    Moment_limiting();
    // �������ء�
    setTorque(Angle_Out, ABZ_electricalAngle());

    // �Ƕȴ��ڴ�ӡ�������
    // printf("%.2f,%.2f\n",Sensor_Angle,Angle_target);
}
// AS5600������
void Set_Angle(float Angle)
{
    // AS5600�Ƕȶ�ȡ��
    Sensor_Angle = AS5600_GetAngle();
    // �Ƕȿ��ơ�
    Angle_Out = Angle_Control((Angle - DIR * Sensor_Angle) * 180.0f / PI);
    // ���������޷���
    Moment_limiting();
    // �������ء�
    setTorque(Angle_Out, _electricalAngle());

    // �Ƕȴ��ڴ�ӡ�������
    // printf("%.2f,%.2f\n",Sensor_Angle,Angle_target);
}

/***********************************************
����ٶȿ���:(rad/s)
***********************************************/
// ABZ������
 void ABZ_Set_Speed(float Speed)
 {
     // ��ȡABZ���������������ݡ�
     ABZ_Sensor_Speed = ABZ_Get_Speed();
     // �ٶ�PID����
     Speed_Out = Speed_Control(Speed - ABZ_Sensor_Speed);
     // ���������޷�
     Moment_limiting();
     // ���������ؿ���
     setTorque(Speed_Out, ABZ_electricalAngle());
    
     // �ٶȴ��ڴ�ӡ�������
     // printf("%.2f,%.2f\n",Sensor_Speed,Speed_target);
 }
// AS5600������
void Set_Speed(float Speed)
{
    // AS5600��ȡ���������ݡ�
    Sensor_Speed = AS5600_Get_Speed();
    // �ٶ�PID����
    Speed_Out = Speed_Control(Speed - Sensor_Speed);
    // ���������޷�
    Moment_limiting();
    // ���������ؿ���
    setTorque(Speed_Out, _electricalAngle());
    
    // �ٶȴ��ڴ�ӡ�������
    // printf("%.2f,%.2f\n",Sensor_Speed,Speed_target);
}



/***********************************************
���������ת:
***********************************************/
void Open_Loop_Control(float Uq, float speed)
{
    // ��λ�����յ�������ֵ��
    // char buffer[50]; // ���ڴ洢ת������ַ���
    // int length = 0;
    float angle_el; // ��ʼ����Ƕ�

    while (1)
    {

        // Read_ADC1_Values();
        Send_ADC1_Values(&huart1); // ���� ADC1 ��ֵ����λ��
        // ���µ�Ƕ�
        angle_el += speed;
        angle_el = _normalizeAngle(angle_el);
        // ��������
        setTorque(Uq, angle_el);

        // ��ȡ��ǰADCֵ
        // ���Դ�ӡ��ȷ��ADCֵ�Ѿ�����ȷ��ȡ
        // HAL_UART_Transmit(&huart1, "Toggle LED0!\r\n", sizeof("Toggle LED0!\r\n"),10000);
        // HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart1, uint8_t *adc1_Ia, sizeof(adc1_Ia), 1);
        // length = snprintf(buffer, sizeof(buffer), "ADC1_Ia: %lu\r\n", adc2_Ia);
        // HAL_UART_Transmit(&huart1, (uint8_t *)buffer, length, HAL_MAX_DELAY);
        // ��ʱ��ȷ������Ƶ��

        HAL_Delay(1); // ����ʵ�����������ʱ
    }
}

/***********************************************
 ���� ADC ��ƫ�����ĺ���:
***********************************************/
void calibrateOffsets()
{
    const int calibration_rounds = 1000;

    // ����0����ʱ��ĵ�ѹ
    offset_ia = 0;
    offset_ib = 0;
    offset_ic = 0;
    // ����1000��
    for (int i = 0; i < calibration_rounds; i++)
    {
        offset_ia += adc1_Ia * (3.3f / 4095.0f); // ��ԭʼֵ���ʵ�ʵ�ѹֵ�ۼӽ�offset_ia
        offset_ib += adc1_Ib * (3.3f / 4095.0f);
        offset_ic += adc1_Ic * (3.3f / 4095.0f);
    }
    // ��ƽ�����õ����
    offset_ia = offset_ia / calibration_rounds;
    offset_ib = offset_ib / calibration_rounds;
    offset_ic = offset_ic / calibration_rounds;

    // ʵ��ʾ��������500mv��Ӧ5A������x10���ʹﵽ�˶�Ӧ�ĵ�����
    volts_to_amps_ratio = 1.0f / 0.01f / 10; // 1/(0.01o+10) ���ò������������Ŵ������档

    // ���õ�������ֵ��
    gain_a = volts_to_amps_ratio * -1;
    gain_b = volts_to_amps_ratio * -1;
    gain_c = volts_to_amps_ratio * -1;
}

/***********************************************
������������
***********************************************/
void Feedback_Current()
{
    // ���㻬���˲������ĵ������������ʵ�ʵ�ѹ��-ƫִ��ѹ��*����
    current1_A = ((filtered_ADC1_Ia * (3.3f / 4095.0f)) - offset_ia) * gain_a;
    current1_B = ((filtered_ADC1_Ib * (3.3f / 4095.0f)) - offset_ib) * gain_b;
    current1_C = ((filtered_ADC1_Ic * (3.3f / 4095.0f)) - offset_ic) * gain_c;
}

/***********************************************
������������ؿ���:
***********************************************/
void Current_Speed(float Uq, float speed)
{

    float angle_el = 0;
    calibrateOffsets(); // У׼���������������
    while (1)
    {
        // uint8_t Senbuff[] = "Serial Output Message by DMA! \r\n";  //�������ݷ�������
        Send_ADC1_Values(&huart1); // ����1 ����������DMA���ͺ�����

        angle_el += speed;                    // �����ۼӸ��µ�Ƕȡ�
        angle_el = _normalizeAngle(angle_el); // �Ƕȹ�һ������

        Current_Loop(Uq, angle_el); // ���õ�����������
        //HAL_Delay(1);               // ��ʱ��
    }
}
/***********************************************
���ڷ���ADC1ֵ
��ע��������
***********************************************/
void Send_ADC1_Values(UART_HandleTypeDef *huart)
{
    char buffer[100];
    int len;

    Read_ADC1_Values(); // ���ȶ�ȡԭʼADC����ֵ��
    // uint8_t Senbuff[] = " DMA OK! \r\n";  //�������ݷ��Ͳ������顣

    //  ��ȡ ADC1 ��ֵ��
    uint16_t new_adc1_Ia = adc1_Ia;
    uint16_t new_adc1_Ib = adc1_Ib;
    uint16_t new_adc1_Ic = adc1_Ic;

    // �����˲�������ȡ�˲����ֵ��
    filtered_ADC1_Ia = ADC_Filter_Update(&adc1_filter_Ia, new_adc1_Ia);
    filtered_ADC1_Ib = ADC_Filter_Update(&adc1_filter_Ib, new_adc1_Ib);
    filtered_ADC1_Ic = ADC_Filter_Update(&adc1_filter_Ic, new_adc1_Ic);

    Feedback_Current(); // ��ȡ��õķ���������

    // ��ʽ���ַ������� ADC1 ��ֵת��Ϊ�ַ�������ӡ�˲����ֵ��
    len = snprintf(buffer, sizeof(buffer), "%u, %u, %u\n", filtered_ADC1_Ia, filtered_ADC1_Ib, filtered_ADC1_Ic);

    // ͨ�����ڷ����ַ�������λ��
    HAL_UART_Transmit(huart, (uint8_t *)buffer, len, 3); // ������������ӡ�ٶ�����ȥ������������������
    // HAL_UART_Transmit_DMA(&huart, (uint8_t *)Senbuff, sizeof(Senbuff));//DMA����������
    // HAL_Delay(1);
}
/***********************************************
//����������
***********************************************/
void Current_Loop(float Uq, float angle_el)
{
    // ���������ת��Ϊ��-������ϵ��
    // ʹ��Clarke�任�����������abc��ת��Ϊ��-������ϵ�ĵ�����i��, i�£���
    i_alpha = current1_A;
    i_beta = (current1_A + 2 * current1_B) / sqrt(3.0f);

    // ����-������ϵ�ĵ���ת��Ϊd-q����ϵ��
    // ʹ��Park�任����-������ϵ�ĵ�����i��, i�£�ת��Ϊd-q����ϵ�ĵ�����id, iq����

    id = i_alpha * cos(angle_el) + i_beta * sin(angle_el);
    iq = -i_alpha * sin(angle_el) + i_beta * cos(angle_el);

    id_ref = 0.0f; // d������ο�ֵΪ0
    iq_ref = Uq;   // q������ο�ֵΪĿ������

    // PID���Ƽ���
    Ud = Current_Control(id_ref - id);
    Uq_new = Current_Control(iq_ref - iq);

    // ʹ����Park�任�����-������ϵ�ĵ�ѹ
    Ualpha = Ud * cos(angle_el) - Uq_new * sin(angle_el);
    Ubeta = Ud * sin(angle_el) + Uq_new * cos(angle_el);
    // ����-������ϵ�ĵ�ѹת��Ϊ�����ѹ��
    // ʹ����Clarke�任����-������ϵ�ĵ�ѹ��v��, v�£�ת��Ϊ�����ѹ��va, vb, vc����

    // ��������任��
    Ua = Ualpha + voltage_power_supply / 2;
    Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + voltage_power_supply / 2;
    Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + voltage_power_supply / 2;

    // �������ѹӦ�����������
    // ��������������ѹӦ�����������PWM�ź��ϡ�
    // PWM��ֵ��
    setPWM(Ua, Ub, Uc);
}

// ///////////////////////////////////////////////////////////////////////////
// /***********************************************
// ����(Ratchet_Wheel)ģʽ����������Pֵ���ò�ͬ�ն�(rigidity)
// DFOC_M0_setTorque(Kp*(target-DFOC_M0_Angle()));

// �������ã�DFOC_M0_setTorque
// ��������ص�ϵ����Kp
// Ҫ����λ�õ�Ŀ��Ƕȣ�target
// ��ȡ�����������ĵ����ǰ�Ƕȣ�DFOC_M0_Angle
// ***********************************************/

// /*arduino dengfoc
// float attractor_distance = 45*PI/180.0;
// float target = round(DFOC_M0_Angle() / attractor_distance) * attractor_distance;
// DFOC_M0_setTorque(Kp * (target - DFOC_M0_Angle()));
// */

// void Ratchet_Wheel(float Angle)
// {
//     // 8���ٴ��,�Ƕ�ֵ��ɻ���ֵ��
//     float attractor_distance = Angle * PI / 180.0f;
//     // ����Pֵ,����������ӡ�
//     float P = 5;
//     // ����Ŀ��ֵ��
//     float target = round(AS5600_GetAngle() / attractor_distance) * attractor_distance;
//     // �նȿ��ơ�
//     setTorque(P * (target - AS5600_GetAngle()), _electricalAngle());
// }

// /***********************************************
// �߽���������(Boundary_Moment_limitation)ģʽ��
// 1����ȡ��ǰ����Ƕ�
// 2���趨������ť��ת����Χ
// 3�����Ƕ���ת��Ϊ������

// ***********************************************/

// void Boundary_Moment_limitation(float angle_range)
// {
//     float P = 5;
//     // �趨������ť��ת����Χ 90��
//     // float angle_range = 90.0;
//     // �趨������ť��ת����Χ�м�ֵ 45��
//     float angle_range2 = angle_range / 2;
//     // �м�ֵת������
//     float limit_range = angle_range2 / 360.0f * _2PI;

//     float L1 = zero_electric_angle + limit_range;
//     float L2 = zero_electric_angle - limit_range;

//     if (AS5600_GetAngle() > L2 && AS5600_GetAngle() < L1)
//     {
//         setTorque(0, _electricalAngle()); // �޶���
//         // 6���ٴ��,�Ƕ�ֵ��ɻ���ֵ
//         // float attractor_distance = 15 * PI / 180.0;
//         // ����Pֵ
//         // float P = 5;
//         // float target = round(get_Angle()/attractor_distance)*attractor_distance;
//         //�նȿ���
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
����(damping)ģʽ��
***********************************************/

/***********************************************
˳��(smooth)ģʽ��
***********************************************/

/***********************************************
ʧ��(weightlessness)ģʽ��
***********************************************/
