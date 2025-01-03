/*
 * 13_usart_it_testing.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Minh
 */

#include <stdio.h>
#include <string.h>
#include "stm32f407xx.h"
#include "stm32f407xx_usart_driver.h"	//Khai báo thư viện xử lý USART peripheral
#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral
#include "stm32f407xx_spi_driver.h"		//Khai báo thư viện xử lý SPI peripheral
#include "stm32f407xx_i2c_driver.h"		//Khai báo thư viện xử lý I2C peripheral


//we have 3 different messages that we transmit to arduino
//you can by all means add more messages
char *msg[3] = {"hihihihihihi123", "Hello How are you ?" , "Today is Monday !"};

//reply from arduino will be stored here
char rx_buf[1024] ;

USART_Handle_t usart2_handle;


//This flag indicates reception completion
uint8_t rxCmplt = RESET;

uint8_t g_data = 0;

extern void initialise_monitor_handles();

//Cấu hình USART2 do thông số user cài đặt
void USART2_Init(void)
{
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
	usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART_Init(&usart2_handle);
}

//Cấu hình USART Alternate Functione
void USART2_GPIOInit(void)
{
	GPIOx_Handle_t usart_gpios;

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode =7;

	usart_gpios.GPIO_PinConfig.GPIO_PinNumber  = GPIO_PIN_NO_2;
	GPIO_Init(&usart_gpios);

	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	GPIO_Init(&usart_gpios);

}

//Cấu hình input nút nhấn
void GPIO_ButtonInit(void)
{
	GPIOx_Handle_t GPIOBtn,GpioLed;

	//this is btn gpio configuration
	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

	//this is led gpio configuration
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD,ENABLE);

	GPIO_Init(&GpioLed);

}

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

int main(void)
{
	uint32_t cnt = 0;

	//initialise_monitor_handles();

	GPIO_ButtonInit();

	USART2_GPIOInit();
    USART2_Init();

    USART_IRQInterruptConfig(IRQ_NO_USART2,ENABLE);

    USART_PeripheralControl(USART2,ENABLE);

    //printf("Application is running\n");

    //do forever
    while(1)
    {
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		//Next message index ; make sure that cnt value doesn't cross 2 - Chỉ mục tin nhắn tiếp theo; đảm bảo rằng giá trị cnt không vượt qua 2
		cnt = cnt % 3;										//Nghĩa là mỗi lần chỉ gửi một index tin nhắn của mảng cnt

		//First lets enable the reception in interrupt mode - this code enables the receive interrupt
		//Trước tiên, hãy kích hoạt việc nhận ở chế độ ngắt - mã này cho phép ngắt nhận
		while (USART_ReceiveDataIT(&usart2_handle,rx_buf,strlen(msg[cnt])) != USART_READY );
		//Mục đích sử dụng hàm "USART_ReceiveDataIT" là để set bit RXNEIE từ đó kích hoạt hàm "USART2_IRQHandler"

		//Send the msg indexed by cnt in blocking mode - Gửi tin nhắn được lập chỉ mục bởi cnt ở chế độ chặn
    	USART_SendData(&usart2_handle,(uint8_t*)msg[cnt],strlen(msg[cnt]));

    	//printf("Transmitted : %s\n",msg[cnt]);


    	//Now lets wait until all the bytes are received from the arduino .
    	//When all the bytes are received rxCmplt will be SET in application callback
    	//Bây giờ hãy đợi cho đến khi nhận được tất cả byte từ arduino.
    	//Khi tất cả các byte được nhận, rxCmplt sẽ được THIẾT LẬP trong cuộc gọi lại ứng dụng
    	while(rxCmplt != SET);

    	//just make sure that last byte should be null otherwise %s fails while printing
    	//chỉ cần đảm bảo rằng byte cuối cùng phải là null nếu không %s sẽ bị lỗi khi in
    	rx_buf[strlen(msg[cnt])+ 1] = '\0';

    	//Print what we received from the arduino
    	//printf("Received    : %s\n",rx_buf);

    	//invalidate(vô hiệu hóa) the flag
    	rxCmplt = RESET;

    	//move on to next message indexed in msg[]
    	cnt ++;
    }

	return 0;
}

//Hàm lấy mẫu từ file startup_stm32f407
void USART2_IRQHandler(void)
{
	USART_IRQHandling(&usart2_handle);
}

/**
 * Triển khai thông báo cho người dùng các sự kiện diễn ra
 */
void USART_ApplicationEventCallback( USART_Handle_t *pUSARTHandle,uint8_t ApEv)
{
   if(ApEv == USART_EVENT_RX_CMPLT)
   {
		rxCmplt = SET;

   }
   else if (ApEv == USART_EVENT_TX_CMPLT)
   {
	   ;
   }
}
