
#include "usart.h"
#include "global.h"

UART_HandleTypeDef huart1;
dataread dat;
//



void sendData(float rpm ){
	datasend dat;
	static uint8_t count=0;
	dat.direccion = 0x1b;
	dat.contador=count;
	dat.ctof.rpm=rpm;
	dat.saltolinea= 10;
	HAL_UART_Transmit(&huart1, (uint8_t *)&dat.direccion, 1 , HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)&dat.contador, 1 , HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)&dat.ctof.dato, 4 , HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, (uint8_t *)&dat.saltolinea, 1 , HAL_MAX_DELAY);
	count++;
	if (count == 255) {
	   count = 0;
	  }

}

void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */

  }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }

}


// Estructura de datos
typedef struct {
    char direccion;
    float dato;
} datarecive;


// Variable para almacenar datos interpretados
datarecive paquete;
uint8_t ind=0;
uint8_t bufer[128];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	 if (huart->Instance == USART1) {

    	bufer[ind]=buf;
    	ind++;

        if((char)buf=='\n'){
        	paquete.direccion = (char)bufer[ind-6];
        	memcpy(&paquete.dato, &bufer[ind-5], sizeof(float));

        	    	 		switch(paquete.direccion){
        	    	 		case 'r':
        	    	 			break;
        	    	 		case 'p':kp=(int)paquete.dato;
        	    	 			break;
        	    	 		case 'd':sendData(3);
        	    	 			break;
        	    	 		case 'i':sendData(paquete.dato);
        	    	 			break;
        	    	 		case 'g':giro=(int)paquete.dato;
        	    	 		    break;
        	    	 		default:sendData(-1);
        	    	 			break;
        	    	   }
        	ind=0; //reset bufer
        }
	  }
	  HAL_UART_Receive_IT(&huart1, &buf, 1);
}



