/*
 * main.c
 *
 *  Created on: Jan 4, 2025
 *      Author: Minh
 */

#include "stm32f4xx_hal.h"				//Thư viện Select MCU
#include "stm32f446xx.h"				//Thư viện define Peripheral
#include "main.h"
#include "string.h"

void SystemCLockConfig(void);
void UART2_Init(void);
void Error_Handler(void);
uint8_t convert_to_capital(uint8_t data);

UART_HandleTypeDef huart2; 				//Biến toàn cục UART Handle

char *user_data = "The application is running \n\r";			//Nội dung để gử

int main(void)
{
	HAL_Init();						//Call HAL_MspInit()
	SystemCLockConfig();
	UART2_Init();					//Call HAL_UART_MspInit()

	//Tx
	uint16_t len_of_data = strlen(user_data);
	/*if(HAL_UART_Transmit(&huart2, (uint8_t*)user_data, len_of_data, HAL_MAX_DELAY) != HAL_OK)
	{
		Error_Handler();
	}*/
	HAL_UART_Transmit(&huart2, (uint8_t*)user_data, len_of_data, HAL_MAX_DELAY);

	//Rx
	uint8_t rcvd_data;											//Lưu nội dung 1 byte nhận
	uint8_t data_buffer[100];									//Mảng lưu nội dung nhận
	uint32_t count=0;											//Đếm index của mảng
	while(1)
	{
		HAL_UART_Receive(&huart2, &rcvd_data, 1, HAL_MAX_DELAY);	//1: nhận từng byte(chứ)
		if(rcvd_data == '\r')
		{
			//Kiểm tra đã đến ký tự cuối cùng
			break;			//thoát khởi vòng lặp
		}
		else
		{
			data_buffer[count++] = convert_to_capital(rcvd_data);  //chữ thường -> In Hoa
		}
	}
	data_buffer[count++] = '\r';		//đưa con trỏ xuống dòng và về đầu dòng
	HAL_UART_Transmit(&huart2, data_buffer, count , HAL_MAX_DELAY);  //gửi lại tin nhắn đc nhận

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
