/*
 * msp.c
 *
 *  Created on: Jan 4, 2025
 *      Author: Minh
 */
#include "stm32f4xx_hal.h"

/*********************************************************************
 * @fn      	- HAL_MspInit
 *
 * @brief       - Configure System Exception
 *
 * @param[in]   - none
 *
 * @return      - none
 *
 * @Note        - Thiết lập các cấu hình ban đầu cho hệ thống, bao gồm các
 * cấu hình liên quan đến ngắt (interrupt) và xử lý lỗi trên bộ xử lý ARM Cortex-M4.
 */
void HAL_MspInit(void)
{
	/* Khởi tạo các khai báo cụ thể liên quan Low Level Processor */
	//1.Setup the priority grouping of the arm cortex mx processor - "stm32f4xx_hal_cortex.h"
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2.Enable the required system exception of the arm cortex mx processor
	//usg fault, memory fault and bus fault of system exception
	SCB->SHCSR |= (0x7 << 16);		//0x7=111 -> Set 3 bit cùng lúc bắt đầu từ bit 16

	//3.Configure the priority for the "system exception"
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);

	/*
	 * 0,0 : Ưu tiên mức cao nhất và mức phụ cao nhất, nghĩa là khi có System Exception
	 * thì sẽ ưu tiên mức cao nhất đc NVIC kích hoạt.
	 * SCB: System Control Block(CortexM4_UserGuide->4.3)
	 */
}

/*********************************************************************
 * @fn      	- HAL_UART_MspInit
 *
 * @brief       - Configure USARTx Peripheral
 *
 * @param[in]   - none
 *
 * @return      - none
 *
 * @Note        - none
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;

	/* Khởi tạo Low Level Processor of UART Peripheral */
	//1.Enable the clock for the USART2 and GPIOA peripheral
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2.Do the pin GPIO configurations for USART
	//UART_Tx
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &gpio_uart);
	//UART_Rx
	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	//3.Enable the IRQ and setup the priority(NVIC settings)
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);

}
