/*
 * 12_usart_txtesting.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Minh
 *
 *  Exercise: Write a program to send some message over UART from STM32 board to Arduino board.
 *  The Arduino board will display the message (on Arduino serial monitor) sent from the ST board.
 	 Baudrate: 115200bps
 	 Frame format: 1 stop bit, 8 bit, no parity

 	 Arduino Sketch: 001UARTRxString.ino
 */
/*
 * Sơ đồ kết nói:
 * STM32: PA2(Tx)-----Logic analyzer------pin_0 :Arduino
 * 		  PA3(Rx)-----Logic analyzer------pin_1
 */
#include<stdio.h>
#include<string.h>
#include "stm32f407xx.h"
#include "stm32f407xx_usart_driver.h"	//Khai báo thư viện xử lý USART peripheral
#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral
#include "stm32f407xx_spi_driver.h"		//Khai báo thư viện xử lý SPI peripheral
#include "stm32f407xx_i2c_driver.h"		//Khai báo thư viện xử lý I2C peripheral

char msg[1024] = "UART Tx testing...\n\r";			//nội dung muốn truyền

USART_Handle_t usart2_handle;		//Sử dụng USART2

//Cấu hình ban đầu do user cài đặt cho USART peripheral
void USART2_Init(void)
{
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_9600;
	usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
	usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART_Init(&usart2_handle);
}

//Thiết lập GPIO Alternate để cấu hình chân TX và RX
void USART2_GPIOInit(void)
{
	GPIOx_Handle_t usart_gpios;

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode= 7;				//AF7 for USART2_xx

	//USART2 TX
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber  = GPIO_PIN_NO_2;
	GPIO_Init(&usart_gpios);

	//USART2 RX
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	GPIO_Init(&usart_gpios);

}

//Thiết lập cấu hình button
void GPIO_ButtonInit(void)
{
	GPIOx_Handle_t GPIOBtn,GpioLed;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	//Đã kích hoạt xung clọc GPIOA khi UART_Init()
	GPIO_Init(&GPIOBtn);

	//this is led gpio configuration
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	//GPIO_PeriClockControl(GPIOD,ENABLE);
	GPIO_Init(&GpioLed);
}

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}


int main(void)
{
	GPIO_ButtonInit();

	USART2_GPIOInit();

    USART2_Init();

    USART_PeripheralControl(USART2,ENABLE);

    while(1)
    {
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		USART_SendData(&usart2_handle,(uint8_t*)msg,strlen(msg));
    }

	return 0;
}
