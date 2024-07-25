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
 *����ĿӲ��ʹ��ST�ٷ�������stm32G474RETX��TI DRV8301�����壬����6PWM/3PWM��դ����������
 *����Ŀ����ο�Odrive��SimpleFOC��ؿ⡣
 * https://odriverobotics.com/
 * http://dengfoc.com/
 * www.simplefoc.com/
 * // TODO���������
 * 1��ADC DMA���䡣//���
 * 2������ֵ���ڴ�ӡ��//���
 * 3��pid���Ρ�
 * 4��ADC�Զ��ɼ���Դ��ѹ��//TODO Ӳ����Ҫ�ӷ�ѹ����ɼ�ĸ�ߵ�ѹ
 * 5��CANͨ��Э�鼰�����Э�顣
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

// ���ݲ�ͬ�����������
int PP = 12;  // pole_pairs������ �ż���/2
int DIR = 1; // ��ˢ�����ƫ��ת����

extern float raw_angle; // ��ȡAS5600ԭʼ�Ƕ�ֵ��

// �Ľ���ʹ��ADC�ɼ����õ�Դ��ѹֵ��
float voltage_limit = 12;        // �û����ã���ѹ�޷�12V�����ò����������ѹ��
float voltage_power_supply = 12; // �û����ã������ѹ12V��������ڿ��Ըĳ�ʹ��ADC�ɼ���Դ����Ķ�̬��ѹ��

float zero_electric_angle = 0; // ���Ƕ�ֵ��

float Ualpha, Ubeta = 0;            // ������任����Ualpha��Ubeta��
float Ua = 0, Ub = 0, Uc = 0;       // ��������任����Ua��Ub��Uc��
float dc_a = 0, dc_b = 0, dc_c = 0; // SPWMռ�ձ����á�

float Sensor_Angle = 0; // �����������ˢ�������״̬�µ�ʵ�ʽǶȡ�
float Sensor_Speed = 0; // �����������ˢ���ʵ���ٶȡ�

uint32_t adc1_Ia = 0, adc1_Ib = 0, adc1_Ic = 0; // ���1�������������ɼ�����ʵ�ʵ�����
uint32_t adc2_Ia = 0, adc2_Ib = 0, adc2_Ic = 0; // ���2�������������ɼ�����ʵ�ʵ�����
uint32_t adc3_VBUS = 0;

float current1_A = 0; // ͨ������õ���A��ʵ�ʵ�����
float current1_B = 0; // ͨ������õ���B��ʵ�ʵ�����
float current1_C = 0; // ͨ������õ���C��ʵ�ʵ�����

float volts_to_amps_ratio = 0; // ����õ��ķŴ�����

float gain_a = 0, gain_b = 0, gain_c = 0; // ��������ϵ����

float offset_ia = 0, offset_ib = 0, offset_ic = 0; // �����ɼ�ʧ����ѹ������

// ��-������ϵ�ĵ�����i��, i�£���
float i_alpha = 0;
float i_beta = 0;

// d-q����ϵ�ĵ�����id, iq����
float id = 0;
float iq = 0;

// �趨PID�ο�ֵ
float id_ref = 0; // d������ο�ֵΪ0
float iq_ref = 0; // q������ο�ֵΪĿ������

// ����PID�������Ŀ�����ֵ��Ŀ������ֵ
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
//    // ���� NVIC �ж����ȼ���ʹ��
//    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(TIM3_IRQn);


  // ����ADC�˲����ṹ��
  // ADC_Filter_t adc_filter;
  // ��ʼ���˲���
  ADC_Filter_Init(&adc1_filter_Ia);
  ADC_Filter_Init(&adc1_filter_Ib);
  ADC_Filter_Init(&adc1_filter_Ic);
  FOC_Init(voltage_power_supply); // ���õ�Դ��ѹ��
  FOC_AS5600_Init(PP, DIR);       // ������������������
  TIM2_M1_ABZ();//TIM2 ABZ�������ӿڳ�ʼ����
  TIM3_M2_ABZ();//TIM2 ABZ�������ӿڳ�ʼ����
  // G474���Դ���·������˴�����DRV8301 EN�š�
  HAL_GPIO_WritePin(M1_EN_GPIO_Port, M1_EN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M2_EN_GPIO_Port, M2_EN_Pin, GPIO_PIN_SET);

  // char *msg = "Hello, World!\r\n";//����1���Ͳ���

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);//����1���Ͳ���
    // HAL_Delay(1000); // 1 second delay
//  volatile  int speed=100;
//      if (AS5600_ReadRawAngle(&hi2c1, &raw_angle) == HAL_OK)
//      {

// //    //   Set_Angle(5); // λ�ñջ���
// //     
//        //Set_Speed(speed);//�ٶȱջ���
//      }  

    //UpdataEncoderCnt();

    //Count_M1_ABZ();//ABZ���Լ���
   Count_M2_ABZ();

    //  Send_ADC1_Values(&huart1);
    //  ��ȡ�µ�ADCֵ������Read_ADC_Value()�����ȡADCֵ�ĺ�����

    // Open_Loop_Control(1.0f, -0.1f); // ��������˳ʱ�룬���õ�ѹ���ٶȡ�
    // Open_Loop_Control(1.0f, -0.01f); // ����������ʱ�룬���õ�ѹ���ٶȡ�
    // UpdataEncoderCnt();
    // Read_ADC3_Values();//��Դ��ѹ�ɼ����ԡ�
    // Read_ADC2_Values();//��Դ��ѹ�ɼ����ԡ�

    // Read_ADC1_Values();//��Դ��ѹ�ɼ����ԡ�

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
