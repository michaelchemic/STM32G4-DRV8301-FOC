/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/**
 *此项目硬件使用ST官方开发板stm32G474RETX，TI DRV8301开发板，兼容6PWM/3PWM的栅极驱动器。
 *此项目软件参考Odrive和SimpleFOC相关库。
 * https://odriverobotics.com/
 * http://dengfoc.com/
 * www.simplefoc.com/
 * // TODO总体待完成项：
 * 1，ADC DMA传输。//完成
 * 2，电流值串口打印。//完成
 * 3，pid调参。
 * 4，ADC自动采集电源电压。//TODO 硬件需要加分压电阻采集母线电压
 * 5，CAN通信协议及其相关协议。
 *
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fdcan.h"
#include "i2c.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "foc.h"
#include "as5600.h"
#include "string.h"
#include "stdio.h"
#include "ABZ.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// 根据不同电机参数设置
int PP = 12;  // pole_pairs极对数 磁极数/2
int DIR = 1; // 无刷电机纠偏旋转方向

extern float raw_angle; // 获取AS5600原始角度值。

// 改进：使用ADC采集设置电源电压值。
float voltage_limit = 12;        // 用户设置：电压限幅12V，设置不超过供电电压。
float voltage_power_supply = 12; // 用户设置：供电电压12V，这里后期可以改成使用ADC采集电源上面的动态电压。

float zero_electric_angle = 0; // 零点角度值。

float Ualpha, Ubeta = 0;            // 帕克逆变换生成Ualpha，Ubeta。
float Ua = 0, Ub = 0, Uc = 0;       // 克拉克逆变换生成Ua，Ub，Uc。
float dc_a = 0, dc_b = 0, dc_c = 0; // SPWM占空比设置。

float Sensor_Angle = 0; // 解算出来的无刷电机运行状态下的实际角度。
float Sensor_Speed = 0; // 解算出来的无刷电机实际速度。

uint32_t adc1_Ia = 0, adc1_Ib = 0, adc1_Ic = 0; // 电机1相电流采样电阻采集到的实际电流。
uint32_t adc2_Ia = 0, adc2_Ib = 0, adc2_Ic = 0; // 电机2相电流采样电阻采集到的实际电流。
uint32_t adc3_VBUS = 0;

float current1_A = 0; // 通过计算得到的A相实际电流。
float current1_B = 0; // 通过计算得到的B相实际电流。
float current1_C = 0; // 通过计算得到的C相实际电流。

float volts_to_amps_ratio = 0; // 计算得到的放大倍数。

float gain_a = 0, gain_b = 0, gain_c = 0; // 电流增益系数。

float offset_ia = 0, offset_ib = 0, offset_ic = 0; // 电流采集失调电压误差补偿。

// α-β坐标系的电流（iα, iβ）。
float i_alpha = 0;
float i_beta = 0;

// d-q坐标系的电流（id, iq）。
float id = 0;
float iq = 0;

// 设定PID参考值
float id_ref = 0; // d轴电流参考值为0
float iq_ref = 0; // q轴电流参考值为目标力矩

// 经过PID后输出的目标电流值与目标力矩值
float Ud = 0;
float Uq_new = 0;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
//test
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USART3_UART_Init();
  MX_FDCAN1_Init();
  MX_ADC3_Init();
  MX_FDCAN2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  
  
//    HAL_TIM_Base_Start_IT(&htim3);
//    // 配置 NVIC 中断优先级和使能
//    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(TIM3_IRQn);


  // 定义ADC滤波器结构体
  // ADC_Filter_t adc_filter;
  // 初始化滤波器
  ADC_Filter_Init(&adc1_filter_Ia);
  ADC_Filter_Init(&adc1_filter_Ib);
  ADC_Filter_Init(&adc1_filter_Ic);
  FOC_Init(voltage_power_supply); // 设置电源电压。
  FOC_AS5600_Init(PP, DIR);       // 极对数，正补偿方向。
  TIM2_M1_ABZ();//TIM2 ABZ编码器接口初始化。
  TIM3_M2_ABZ();//TIM2 ABZ编码器接口初始化。
  // G474可以带两路电机，此处控制DRV8301 EN脚。
  HAL_GPIO_WritePin(M1_EN_GPIO_Port, M1_EN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M2_EN_GPIO_Port, M2_EN_Pin, GPIO_PIN_SET);

  // char *msg = "Hello, World!\r\n";//串口1发送测试

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);//串口1发送测试
    // HAL_Delay(1000); // 1 second delay
//  volatile  int speed=100;
//      if (AS5600_ReadRawAngle(&hi2c1, &raw_angle) == HAL_OK)
//      {

// //    //   Set_Angle(5); // 位置闭环。
// //     
//        //Set_Speed(speed);//速度闭环。
//      }  

    //UpdataEncoderCnt();

    //Count_M1_ABZ();//ABZ调试计数
   Count_M2_ABZ();

    //  Send_ADC1_Values(&huart1);
    //  读取新的ADC值（假设Read_ADC_Value()是你获取ADC值的函数）

    // Open_Loop_Control(1.0f, -0.1f); // 开环控制顺时针，设置电压和速度。
    // Open_Loop_Control(1.0f, -0.01f); // 开环控制逆时针，设置电压和速度。
    // UpdataEncoderCnt();
    // Read_ADC3_Values();//电源电压采集测试。
    // Read_ADC2_Values();//电源电压采集测试。

    // Read_ADC1_Values();//电源电压采集测试。

    // HAL_Delay(1);

    // Current_Speed(2.0f, 0.1f);

    // Read_ADC2_Values();

    // HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    // HAL_Delay(10);
    // HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    // HAL_Delay(10);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
