/*
 * 006_spi_cmd_handling.c
 *
 *  Created on: Dec 2, 2023
 *      Author: Minh
 */
/*
 *  Đề tài: Giao tiếp dựa trên lệnh và phản hồi SPI Master(STM32) và SPI Slave(Arduino).
Khi nhấn nút trên master, master sẽ gửi lệnh đến Slave và Slave và Slave sẽ phản hồi theo lệnh
thực hiện
  1. Sử dụng chế độ song công SPI
  2. Bảng ST sẽ ở chế độ SPI master và Arduino sẽ được cấu hình ở chế độ SPI Slave
  3. Sử dụng DFF = 0
  4. Sử dụng quản lý nô lệ phần cứng (SSM=0)
  5. Tốc độ SCLK = 2 MHz, f_CLK = 16Mhz
 */

//command codes
#define COMMAND_LED_CTRL			0x50
#define COMMAND_SENSOR_READ			0x51
#define COMMAND_LED_READ			0x52
#define COMMAND_PRINT				0x53
#define COMMAND_ID_READ				0x54

//Led status
#define LED_ON						1
#define LED_OFF						0

//Arduino analog pins
#define ANALOG_PIN0					0
#define ANALOG_PIN1					1
#define ANALOG_PIN2					2
#define ANALOG_PIN3					3
#define ANALOG_PIN4					4

//Arduino led
//#define LED_PIN_S					LED_BUILTIN		//Led on board Arduino
#define LED_PIN						9

/*
 * Seting SPI2: Datasheet_stm32f407x -> 3. Pinout and pin -> Table 9: Alternate function mapping
 * 	SPI2_MOSI --> PB15
 *  SPI2_MISO --> PB14
 *  SPI2_SCLK --> PB13
 *  SPI2_NSS  --> PB12
 *  ALT function mode --> 5
 */

#include <string.h>						//Thư viện hàm memset()
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral
#include "stm32f407xx_spi_driver.h"		//Khai báo thư viện xử lý SPI peripheral
#include <stdint.h>
#include <stdio.h>

/**
 * Hàm thiết lập tính năng Alternate chuyển đổi chân Pins từ GPIO sang SPI
 */
void SPI2_GPIOInits(){

	GPIOx_Handle_t SPIpins;										//Khai báo 1 biến cấu trúc GPIO Portx muốn dùng AF

	SPIpins.pGPIOx = GPIOB;										//Xác định Port có chân làm chức năng Alternate
	SPIpins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;		//Thiết lập tính năng Alternate Function
	SPIpins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;				//Column AF5
	SPIpins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;	//SPI sử dụng push-pull
	SPIpins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIpins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SPI2_NSS pin
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;		//PB12 làm chân AF5 SPI2_NSS
	GPIO_Init(&SPIpins);										//Thiết lập SPI_NSS pin

	//SPI2_SCLK pin
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;		//PB13 làm chân AF5 SPI2_SCLK
	GPIO_Init(&SPIpins);										//Thiết lập SPI_SCLK pin

	//SPI2_MISO pin
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;		//PB14 làm chân AF5 SPI2_MISO
	GPIO_Init(&SPIpins);										//Thiết lập SPI_MISO pin

	//SPI2_MOSI pin
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;		//PB13 làm chân AF5 SPI2_MOSI
	GPIO_Init(&SPIpins);										//Thiết lập SPI_MOSI pin


	//Nếu chân nào ko sử dụng thì có thể toggle lại
}

/**
 * Khởi tạo các cấu hình cơ bản cho SPI Peripheral
 */
void SPI2_Inits()
{
	SPI_Handle_t SPI2handle;											//Khai báo biến struct cấu hình SPI2

	SPI2handle.pSPIx = SPI2;											//Thiết lập SPIx sử dụng để Send Data
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;				//Full-Duplex
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;		//Master
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;			//Generate SCLK of 2MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BIT;						//Data 8bit
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;						//Cạnh xung xuống
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;						//Nửa chu kỳ đầu
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DS;							//Software Slave Disable(Hardware Slave Enable)

	SPI_Init(&SPI2handle);												//Thiết lập cấu hình SPI2
}

void GPIO_ButtonInit()
{
	GPIOx_Handle_t GPIOBtn;			//Khai báo biến struct chứa thông tin port và pin(input button) muốn điều khiển

	//Cấu hình thông tin hoạt động của Button muốn điều khiển
	GPIOBtn.pGPIOx = GPIOA;										//Thiết lập port muốn điều khiển
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;		//Chân pin của port muốn điều khiển
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;			//Chế độ xuất tín hiệu input
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; 	//Tốc độ xuất tín hiệu

	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//Đầu vào ko cần điện trở kéo
	//Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;	//Chọn pull-up(kéo lên)

	//Cấu hình xung Clock cho Peripheral Port Button
	//GPIO_PeriClockControl(GPIOA, ENABLE);		//Port D và Enalbe=1
	//Ko cần vì đã cấu hình xung clock ở hàm dưới cho SPI

	//Gọi hàm khởi tạo hoạt động cấu hình muốn điều khiển(ở đây muốn điều khiển Input Button)
	GPIO_Init(&GPIOBtn);
}

/*
 * Hàm Delay thời gian
 */
void delay(void)
{
	//for(uint32_t i = 0; i< 500000; i++);
	for(uint32_t i = 0; i< 500000/2; i++);		//giảm thời gian delay
}

/*
 * Kiểm tra giá trị ackbyte nhận đc có là giá trị hợp lệ hay không
 */
uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte == 0xF5)
	{
		//ack
		return 1;
	}
	return 0;
}

int main(void)
{
	//char user_data[] = "Hello world";

	uint8_t dummy_write = 0xff;			//dummy bits - mã giả
	uint8_t dummy_read;

	//0. Khởi tạo cấu hình nút nhấn cho phép truyền
	GPIO_ButtonInit();

	//1. This is function is used initialize the GPIO pins to behave(thay thế) as SPI2 pins
	SPI2_GPIOInits();

	//2. Khởi tạo chức năng SPI2
	SPI2_Inits();

	//2.1 Cấu hình SSOE(high) để tạo điều kiện cho phép NSS=Low
	/*
	 * Making SSOE 1 does NSS output enable
	 * The NSS pin is automatically managed by the hardware
	 * i.e: when SPE=1, NSS will be pulled to low
	 * and NSS pin will be high when SPE=0
	 */
	SPI_SSOEConfig(SPI2, ENABLE);

	//Vòng lặp chương trình
	while(1)
	{
		//2.1 This makes NSS signal internally high(Sử dụng trong trường hợp NSS ko nối với Slave nào)
		//SPI_SSIConfig(SPI2,ENABLE);							//Cấu hình NSS lên HIGH để phòng tránh lỗi MODF
		//Vì NSS sẽ với với Slave Arduino nên ko cần cấu hình lên mức cao

		//Wait till button is pressed
		//nếu ko nhấn(=0) sẽ ở vòng lặp này và nếu nhấn(=1) sẽ thoát khỏi vòng lặp và làm công việc bên dưới
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		delay();			//200ms delay

		//3. Enable the SPI Peripheral worked
		SPI_PeripheralControl(SPI2, ENABLE);

		//4. Control
		//(1) CMD_LED_CTRL <pin no> <Value>
		uint8_t commandcode = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t args[2];

		//send command
		SPI_SendData(SPI2, &commandcode, 1);

		//do dummy read to claer off the RXNE, đọc để xóa bit RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//send some dummy bits (1 byte) to fetch the response from the slave
		//gửi một số bit giả (1 byte) để lấy phản hồi từ slave
		SPI_SendData(SPI2, &dummy_write, 1);

		//receive
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if(SPI_VerifyResponse(ackbyte))			//kiểm tra giá trị ackbyte hợp lệ nhận đc
		{
			//Send argument
			args[0] = LED_PIN;		//led on board
			args[1] = LED_ON;
			//argu[1] = LED_OFF;

			//send argument
			SPI_SendData(SPI2,args,2);
		}
		//end of CMD_LED_CTRL

		//(2) CMD_SENSOR_READ <analog pin number>
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		delay();			//200ms delay
		commandcode = COMMAND_SENSOR_READ;
		//send command
		SPI_SendData(SPI2, &commandcode, 1);

		//do dummy read to claer off the RXNE, đọc để xóa bit RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//send some dummy bits (1 byte) to fetch the response from the slave
		//gửi một số bit giả (1 byte) để lấy phản hồi từ slave
		SPI_SendData(SPI2, &dummy_write, 1);

		//receive ackbyte
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if(SPI_VerifyResponse(ackbyte))			//kiểm tra giá trị ackbyte hợp lệ nhận đc
		{
			//Send argument
			args[0] = ANALOG_PIN0;

			//send argument
			SPI_SendData(SPI2,args,1);

			//do dummy read to claer off the RXNE, đọc để xóa bit RXNE
			SPI_ReceiveData(SPI2, &dummy_read, 1);

			//insert some delay so that slave can ready with the data
			delay();

			//send some dummy bits (1 byte) to fetch the response from the slave
			//gửi một số bit giả (1 byte) để lấy phản hồi từ slave
			SPI_SendData(SPI2, &dummy_write, 1);

			uint8_t analog_read;			//biến nhận giá trị đọc đc
			SPI_SendData(SPI2, &analog_read, 1);
		}
		//end of CMD_SENSOR_READ

		//(3)  CMD_LED_READ 	 <pin no(1) >
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = COMMAND_LED_READ;

		//send command
		SPI_SendData(SPI2,&commandcode,1);

		//do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2,&dummy_read,1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2,&dummy_write,1);

		//read the ack byte received
		SPI_ReceiveData(SPI2,&ackbyte,1);

		if( SPI_VerifyResponse(ackbyte))
		{
			args[0] = LED_PIN;

			//send arguments
			SPI_SendData(SPI2,args,1); //sending one byte of

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI2,&dummy_read,1);

			//insert some delay so that slave can ready with the data
			delay();

			//Send some dummy bits (1 byte) fetch the response from the slave
			SPI_SendData(SPI2,&dummy_write,1);

			uint8_t led_status;
			SPI_ReceiveData(SPI2,&led_status,1);

			printf("COMMAND_READ_LED %d\n",led_status);  //hiển thị data phản hồi từ Slave
			//Cần cấu hình lại debug để hiển thị printf
		}
		//end of CMD_LED_READ

		//(4) CMD_PRINT 		<len(2)>  <message(len) >

		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = COMMAND_PRINT;

		//send command
		SPI_SendData(SPI2,&commandcode,1);

		//do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2,&dummy_read,1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2,&dummy_write,1);

		//read the ack byte received
		SPI_ReceiveData(SPI2,&ackbyte,1);

		uint8_t message[] = "Hello ! How are you ??";
		if( SPI_VerifyResponse(ackbyte))
		{
			args[0] = strlen((char*)message);

			//send arguments
			SPI_SendData(SPI2,args,1); //sending length

			//send message
			SPI_SendData(SPI2,message,args[0]);

			printf("COMMAND_PRINT Executed \n");

		}
		//end of CMD_PRINT

		//(5) CMD_ID_READ
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		commandcode = COMMAND_ID_READ;

		//send command
		SPI_SendData(SPI2,&commandcode,1);

		//do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2,&dummy_read,1);

		//Send some dummy byte to fetch the response from the slave
		SPI_SendData(SPI2,&dummy_write,1);

		//read the ack byte received
		SPI_ReceiveData(SPI2,&ackbyte,1);

		uint8_t id[11];
		uint32_t i=0;
		if( SPI_VerifyResponse(ackbyte))
		{
			//read 10 bytes id from the slave
			for(  i = 0 ; i < 10 ; i++)
			{
				//send dummy byte to fetch data from slave
				SPI_SendData(SPI2,&dummy_write,1);
				SPI_ReceiveData(SPI2,&id[i],1);
			}

			id[11] = '\0';

			printf("COMMAND_ID : %s \n",id);

		}
		//end of CMD_ID_READ


		//5. Let comfirm SPI, nếu 1 trả về thì nó sẽ lặp liên tục cho đến khi 0 đc trả về
		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

		//5.1 Disable the SPI Peripheral worked
		SPI_PeripheralControl(SPI2, DISABLE);			//Đóng cổng cho phép truyền
	}

	return 0;
}

