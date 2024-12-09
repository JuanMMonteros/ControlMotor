
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "global.h"

extern UART_HandleTypeDef huart1;
// Variable de Contorl de Motor
uint8_t buf;
float kp=1;
float ki=1;
float kd=0.01;
float sp=0;
float Ts=0.1;
uint8_t giro= 1;

void SystemClock_Config(void);
void DriverPWM(int outpid);
int PID(void);

int main(void)
{
  HAL_Init();

  SystemClock_Config();


  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  if (HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1) != HAL_OK) {
         Error_Handler();
     }
  if (HAL_TIM_Base_Start(&htim2) != HAL_OK) {
      Error_Handler();
  }
 // HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);

  // Iniciar recepción de datos mediante interrupción
  HAL_UART_Receive_IT(&huart1, &buf, 1);


  int pidout;
  while (1)
  {
	  //sendData(3.14);
	  pidout=PID();
	  DriverPWM(pidout);
	  HAL_Delay(500);
  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }


  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

void DriverPWM(int outpid){
	uint8_t duty=50;
	if (giro == 1){
		duty = 50 + outpid/2;
	}else{
		duty = 50 - outpid/2;
	}
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, duty * (htim3.Init.Period + 1) / 100);
}

int PID(void){
	static float e[3]={0,0,0};
	static float cv[2] = {0.0};
	e[0]=sp - 60*frequency;
	cv[0]= cv[1] + (kp+(kd/Ts))*e[0] + (-kp + ki*Ts - (2*kd/Ts))*e[1] + (kd/Ts)*e[2];
	cv[1]=cv[0];
	e[2]=e[1];
	e[1]=e[0];
	if(cv[0] >= 100)
		cv[0]=100;
	if(cv[0]<0)
		cv[0]=0;
	return (int)cv[0];
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
