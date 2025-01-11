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

#define TRUE  1
#define FALSE 0

void SystemCLockConfig(void);
void UART2_Init(void);
void Error_Handler(void);
uint8_t convert_to_capital(uint8_t data);

//Biến dùng cho Interrupt
UART_HandleTypeDef huart2; 				//Biến toàn cục UART Handle
uint8_t recv_it_data;
uint8_t count;
uint8_t reception_complete = FALSE;

uint8_t data_buffer[100];					//Biến toàn cục lưu data đc nhận về sau ngắt

char *user_data = "The application is running \n\r";			//Nội dung để gửi

int main(void)
{
	HAL_Init();						//Call HAL_MspInit()
	SystemCLockConfig();
	UART2_Init();					//Call HAL_UART_MspInit()

	//Tx
	uint16_t len_of_data = strlen(user_data);
	HAL_UART_Transmit(&huart2, (uint8_t*)user_data, len_of_data, HAL_MAX_DELAY);

	//Rx Interrupt - Chờ đến khi nhận hết data bằng ngắt
	while(reception_complete != TRUE)
		HAL_UART_Receive_IT(&huart2, &recv_it_data, 1);  //Hàm đặt trước nếu có ngắt xảy ra

	while(1);
	return 0;
}

/*********************************************************************
 * @fn      	- SystemCLockConfig
 *
 * @brief       - Cấu hình
 *
 * @param[in]   - none
 *
 * @return      - none
 *
 * @Note        - none
 */
void SystemCLockConfig(void)
{

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
 * @fn      	- convert_to_capital
 *
 * @brief       - Chuẩn hóa ký tự nhận đc từ ký tự thường thanh ký tự In Hoa
 *
 * @param[in]   - none
 *
 * @return      - none
 *
 * @Note        - none
 */
uint8_t convert_to_capital(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		data = data - ('a' - 'A');
	}
	return data;
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

/*********************************************************************
 * @fn      	- HAL_UART_RxCpltCallback
 *
 * @brief       - Đc call khi xử lý UART Interrupt hoàn thành
 *
 * @param[in]   - none
 *
 * @return      - none
 *
 * @Note        - hàm đã đc "weak" trong file "stm32f4xx_hal_uart.c"
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(recv_it_data == '\r')
	{
		//Gửi lại nội dung đc nhận
		reception_complete = TRUE;
		data_buffer[count++] = '\r';
		HAL_UART_Transmit(&huart2, data_buffer, count, HAL_MAX_DELAY);
	}
	else
	{
		data_buffer[count++] = recv_it_data;
	}

	/* Giải thích:
	 * Cứ mỗi lần phát hiện có 1 byte(1 chữ) sẽ nhận thì ngắt xảy ra
	 * Các bước xử lý ngắt nhận data sẽ nằm ở file it.c
	 * Sau cùng hàm callback đc gọi để xử lý 1byte đã nhận
	 * Liên tục lặp lại ngắt cho đến khi nhận hết chuỗi byte data
	 */
}
