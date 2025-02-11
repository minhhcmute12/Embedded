/*
 * main.c
 *
 *  Created on: Jan 4, 2025
 *  Author: Minh
 *  Send and Receive data use UART Interrupt Peripheral
 */

#include "stm32f4xx_hal.h"				//Thư viện Select MCU
#include "stm32f446xx.h"				//Thư viện define Peripheral
#include "main.h"
#include "string.h"
#include "stdio.h"

#define TRUE  1
#define FALSE 0

void UART2_Init(void);
void Error_Handler(void);

//Biến dùng cho Interrupt
UART_HandleTypeDef huart2; 				//Biến toàn cục UART Handle


int main(void)
{
	//1.Khai báo các biến cấu trúc liên quan đến cấu hình xung clock
	RCC_OscInitTypeDef osc_init;				//Lựa chọn HSE,HSI,LSE,LSI
	RCC_ClkInitTypeDef clk_init;				//Lựa chọn CLK cho các Bus
	char msg[100];								//Tin nhắn gửi thông số clk đã cấu hình

	HAL_Init();						//Call HAL_MspInit()
	UART2_Init();					//Call HAL_UART_MspInit()

	//2.Cấu hình thông số cho các thành phần biến struct "osc_init"
	memset(&osc_init,0,sizeof(osc_init));			//Khởi tạo value thành phần cấu trúc bằng 0
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	//Call hàm OSC và cấu hình
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_Handler();
	}

	//3.Cấu hình thông số cho các thành phần biến struct "clk_init"
	//Cấu hình nhiều xung clk
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	//Cấu hình nguồn lấy xung CLK là HSE
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;		//8MHz
	//Cấu hình thông số cho bộ chia(Prescaler) của AHB,APB1,APB2 Bus
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;			//HSE -> AHB = 4MHz (Đề bài)
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2; 			//AHB -> APB1 = 2MHz (Đề bài)
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2; 			//AHB -> APB2 = 2MHz (Đề bài)

	//Call hàm Clock, đồng thời cấu hình clk và độ trễ(LATENCY) Flash
	if(HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		Error_Handler();
	}

	//4.Tắt HSI clk để chuyển sang cấu hình Clk mới
	__HAL_RCC_HSI_DISABLE();

	//5.Khởi tạo Bộ đếm thời gian hệ thống và ngắt của nó,
	//và khởi động Bộ đếm thời gian tích tắc hệ thống.
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	//6.Cấu hình nguồn clk cho bộ đồng hồ SysTick
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	//7.Call lại UART
	UART2_Init();

	//.Quan sát các thông số đã đc cấu hình
	memset(msg,0,sizeof(msg));
	sprintf(msg,"SYSCLK : %ld\r\n", HAL_RCC_GetSysClockFreq());		//SYSCLK
	HAL_UART_Transmit(&huart2,(uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"HCLK : %ld\r\n", HAL_RCC_GetHCLKFreq());			//HCLK
	HAL_UART_Transmit(&huart2,(uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"APB1 : %ld\r\n", HAL_RCC_GetPCLK1Freq());			//APB1
	HAL_UART_Transmit(&huart2,(uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"APB2 : %ld\r\n", HAL_RCC_GetPCLK2Freq());			//APB2
	HAL_UART_Transmit(&huart2,(uint8_t *) msg, strlen(msg), HAL_MAX_DELAY);

	while(1);
	return 0;
}

/*********************************************************************
 * @fn      	- UART2_Init
 *
 * @brief       - Cấu hình thông số USARTx Peripheral
 *
 * @param[in]   - none
 *
 * @return      - none
 *
 * @Note        - none
 */
void UART2_Init(void)
{
	//Cài đặt từng thông số USARTx
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	//Cấu hình USARTx
	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		//This is a Problem
		Error_Handler();

		/* Giải thích:
		 * Kiểu biến của hàm HAL_UART_Init là một kiểu "Enum" "HAL_StatusTypeDef"
		 * mà "Enum" "HAL_StatusTypeDef" là gồm các lựa chọn trạng thái kết quả hàm
		 */
	}
}

/*********************************************************************
 * @fn      	- Error_Handler
 *
 * @brief       - Xử lý khi cấu hình USARTx xuất hiện lỗi
 *
 * @param[in]   - none
 *
 * @return      - none
 *
 * @Note        - none
 */
void Error_Handler(void)
{
	while(1); //chương trình lặp lại vô hạn
}
