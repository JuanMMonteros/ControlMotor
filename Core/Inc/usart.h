/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include "stm32f1xx_hal.h"


typedef struct {

	uint8_t direccion;
	uint8_t contador;
	union charfloat{
		float   rpm;
		uint8_t dato[4];
	}ctof;
	uint8_t    saltolinea ;
}datasend;

typedef struct {
		char direccion;
		union charfloa{
				float   rpm;
				uint8_t dato[4];
			}ctof;
		char  saltolinea;
	}dataread;


void MX_USART1_UART_Init(void);
void sendData(float);
void processUARTBuffer(void);



#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

