/*
 * 007_spi_message_rcv_it.c
 *
 *  Created on: Dec 3, 2023
 *      Author: Minh
 *
 * Đề: Bo mạch STM32 (Master) nhận được tin nhắn từ bo mạch Arduino (Slave) qua SPI
1) Người dùng nhập tin nhắn bằng thiết bị đầu cuối màn hình nối tiếp Arduino
2) Bo mạch Arduino thông báo cho bo mạch STM32 về tính khả dụng của tin nhắn
3) Thiết bị STM32 đọc và in tin nhắn


 * Note : Follow the instructions to test this code
 * 1. Download this code on to STM32 board , acts as Master
 * 2. Download Slave code (003SPISlaveUartReadOverSPI.ino) on to Arduino board (Slave)
 * 3. Reset both the boards
 * 4. Enable SWV ITM data console to see the message
 * 5. Open Arduino IDE serial monitor tool
 * 6. Type anything and send the message (Make sure that in the serial monitor tool line ending set to carriage return)
 */

#include <string.h>						//Thư viện hàm memset()
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral
#include "stm32f407xx_spi_driver.h"		//Khai báo thư viện xử lý SPI peripheral
#include <stdint.h>
#include <stdio.h>

//Khai báo các biến toàn cục
SPI_Handle_t SPI2handle;
#define MAX_LEN 500
char RcvBuff[MAX_LEN];
volatile char ReadByte;
//char ReadByte;


volatile uint8_t rcvStop = 0;

/*This flag will be set in the interrupt handler of the Arduino interrupt GPIO */
volatile uint8_t dataAvailable = 0;

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */

/**
 * Cấu hình SPI là một GPIO Alternate
 */
void SPI2_GPIOInits(void)
{
	GPIOx_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);


	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);


}

/**
 * Cấu hình thông số SPI
 */
void SPI2_Inits(void)
{
	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BIT;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DS; //Hardware slave management enabled for NSS pin

	SPI_Init(&SPI2handle);
}

/*This function configures the gpio pin over which SPI peripheral issues data available interrupt */
/**
 * Cáu hình nút nhấn là một ngắt
 */
void Slave_GPIO_InterruptPinInit(void)
{
	GPIOx_Handle_t spiIntPin;
	memset(&spiIntPin,0,sizeof(spiIntPin));

	//this is led gpio configuration
	spiIntPin.pGPIOx = GPIOD;
	spiIntPin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	spiIntPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	spiIntPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	spiIntPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_Init(&spiIntPin);

	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5,NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,ENABLE);
}

int main(void)
{

	uint8_t dummy = 0xff;

	Slave_GPIO_InterruptPinInit();

	//this function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	//This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	/*
	* making SSOE 1 does NSS output enable.
	* The NSS pin is automatically managed by the hardware.
	* i.e when SPE=1 , NSS will be pulled to low
	* and NSS pin will be high when SPE=0
	*/
	SPI_SSOEConfig(SPI2,ENABLE);

	SPI_IRQInterruptConfig(IRQ_NO_SPI2,ENABLE);				//Cấu hình Interrupt SPIx là SPI2

	while(1){

		rcvStop = 0;

		while(!dataAvailable); //wait till data available interrupt from transmitter device(slave)

		GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,DISABLE);			//Ngăn cho việc nhấn nút một lần nữa

		//enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2,ENABLE);					//Cho phép xung clock ngắt SPI2


		while(!rcvStop)
		{
			/* fetch the data from the SPI peripheral byte by byte in interrupt mode */
			//Đưa data muốn truyền hoặc nhận và set trạng thái vào biến toàn cục
			while ( SPI_SendDataState_IT(&SPI2handle,&dummy,1) == SPI_BUSY_IN_TX);
			//while ( SPI_ReceiveDataState_IT(&SPI2handle,&ReadByte,1) == SPI_BUSY_IN_RX);
		}


		// confirm SPI is not busy
		while( SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG) );

		//Disable the SPI2 peripheral
		SPI_PeripheralControl(SPI2,DISABLE);						//Dừng ngắt SPI và quay về trạng thái hàm main

		printf("Rcvd data = %s\n",RcvBuff);							//Hiển thị data nhận đc

		dataAvailable = 0;

		GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5,ENABLE);				//Kích hoạt lại nút nhấn cho việc gửi tiếp theo

	}

	return 0;

}

/* Runs when a data byte is received from the peripheral over SPI*/
/**
 * Hàm xử lý ngắt SPIx
 */
void SPI2_IRQHandler(void)
{
	SPI_IRQHangling(&SPI2handle);
}

/**
 * Hàm này sẽ được gọi tự động khi có sự kiện nhận dữ liệu hoàn tất
 */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
{
	static uint32_t i = 0;
	/* In the RX complete event , copy data in to rcv buffer . '\0' indicates end of message(rcvStop = 1) */
	if(AppEv == SPI_EVENT_RX_CMPLT)
	{
		ReadByte = *(pSPIHandle->pRxBuffer);
		RcvBuff[i++] = ReadByte;
		if(ReadByte == '\0' || ( i == MAX_LEN)){
			rcvStop = 1;
			RcvBuff[i-1] = '\0';
			i = 0;
		}
	}

	//Giải thích: Khi việc nhận từng data lần lượt từ hàm SPI_RXE_Interrupt_Handle() hoàn thành thì hàm này sẽ đc gọi
	//Đọc cho đến khi gặp ký hiệu kết thúc chuỗi.
}

/* Slave data available interrupt handler */
void EXTI9_5_IRQHandler(void)
{
	GPIO_IRQHangling(GPIO_PIN_NO_6);
	dataAvailable = 1;
}
