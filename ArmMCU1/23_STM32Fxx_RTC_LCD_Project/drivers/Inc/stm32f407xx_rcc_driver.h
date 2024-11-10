/*
 * stm32f407xx_rcc_driver.h
 *
 *  Created on: Jan 27, 2024
 *      Author: Minh
 */

#ifndef INC_STM32F407XX_RCC_DRIVER_H_
#define INC_STM32F407XX_RCC_DRIVER_H_

#include "stm32f407xx.h"	//Thư viện lớp Driver của vđk

//This returns the APB1 Bus Clock value
uint32_t RCC_GetPCLK1Value(void);

//This returns the APB2 Bus Clock value
uint32_t RCC_GetPCLK2Value(void);

uint32_t RCC_GetPLLOutputClock(void);

#endif /* INC_STM32F407XX_RCC_DRIVER_H_ */
