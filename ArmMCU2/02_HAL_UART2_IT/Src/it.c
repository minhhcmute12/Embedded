/*
 * it.c
 *
 *  Created on: Jan 4, 2025
 *      Author: Minh
 */

#include "main.h"					//chứa thư viện stm32f4xx_hal.h

//extern biến được khai báo sẽ được định nghĩa ở một file khác
extern UART_HandleTypeDef huart2;

/*********************************************************************
 * @fn      	- SysTick_Handler
 *
 * @brief       - Cấu hình xung stick_clock trong ứng dụng
 *
 * @param[in]   - none
 *
 * @return      - none
 *
 * @Note        - Hàm này ghi đề hàm nằm trong file "startup_stm32f446retx.s"
 */
void SysTick_Handler(void)
{
	HAL_IncTick(); 					//Sử dụng làm cơ sở thời gian ứng dụng.
	HAL_SYSTICK_IRQHandler();		//Sử dụng làm cơ sở thời gian ngắt trong ứng dụng.

	/* Giải thích:
	 * Tăng giá trị biến uwTick: Hàm này tăng giá trị của biến toàn cục uwTick lên 1. Biến
	 * uwTick là một biến đếm thời gian được sử dụng bởi HAL để cung cấp các hàm quản lý
	 * thời gian như HAL_Delay() và HAL_GetTick().
	 */
}

/*********************************************************************
 * @fn      	- USART2_IRQHandler
 *
 * @brief       - Hàm xử lý hành động nếu có ngắt UART xảy ra
 *
 * @param[in]   - none
 *
 * @return      - none
 *
 * @Note        - Hàm này ghi đề hàm nằm trong file "startup_stm32f446retx.s"
 */
void USART2_IRQHandler()
{
	HAL_UART_IRQHandler(&huart2);		//Hàm xử lý khi có ngắt UART
}


