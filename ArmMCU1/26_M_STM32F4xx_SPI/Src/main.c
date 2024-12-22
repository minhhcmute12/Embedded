/*
 * Vd1: 004_spi_testing.c
 *
 *  Created on: Nov 28, 2023
 *      Author: Minh
 *
 *==Exercise to Test SPI Send Data API(V145)
 *	1. Test the SPI_SendData API to send the string "Hello World" and use the below configuration
 * 	 	 1. SPI-2 Master mode
 * 	 	 2.SCLK = max possible
 * 	 	 3. DFF = 0 and DFF = 1
*/
/*
 * Seting SPI2: Datasheet_stm32f407x -> 3. Pinout and pin -> Table 9: Alternate function mapping
 * 	SPI2_MOSI --> PB15
 *  SPI2_MISO --> PB14
 *  SPI2_SCLK --> PB13
 *  SPI2_NSS  --> PB12
 *  ALT function mode --> 5
 */

#include <stdint.h>
#include "stm32f407xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include <string.h>						//Thư viện hàm memset()
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral
#include "stm32f407xx_spi_driver.h"		//Khai báo thư viện xử lý SPI peripheral

/**
 * Hàm thiết lập tính năng Alternate chuyển đổi chân Pins từ GPIO sang SPI
 * Datasheet: Table 9. Alternate function mapping
 */
void SPI2_GPIOInits(){

	GPIOx_Handle_t SPIpins;										//Khai báo 1 biến cấu trúc Portx muốn dùng AF

	SPIpins.pGPIOx = GPIOB;										//Xác định Port có chân làm chức năng Alternate
	SPIpins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;		//Thiết lập tính năng Alternate Function
	SPIpins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;				//Column AF5
	SPIpins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;	//SPI sử dụng push-pull
	SPIpins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIpins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//Thiết lập cấu hình SPI2 -> Tra bảng Table 9. Alternate function mapping

	//SPI2_SCLK pin
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;		//PB13 làm chân AF5 SPI2_SCLK
	GPIO_Init(&SPIpins);										//Thiết lập SPI_SCLK pin

	//SPI2_MISO pin
//	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;		//PB14 làm chân AF5 SPI2_MISO
//	GPIO_Init(&SPIpins);										//Thiết lập SPI_MISO pin

	//SPI2_MOSI pin
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;		//PB13 làm chân AF5 SPI2_MOSI
	GPIO_Init(&SPIpins);										//Thiết lập SPI_MOSI pin

	//SPI2_NSS pin
	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;		//PB12 làm chân AF5 SPI2_NSS
	GPIO_Init(&SPIpins);										//Thiết lập SPI_NSS pin

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
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;			//Generate SCLK of 8MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BIT;						//Data 8bit
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;						//Cạnh xung xuống
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;						//Nửa chu kỳ đầu
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;							//Software Slave Enable

	SPI_Init(&SPI2handle);												//Thiết lập cấu hình SPI2
}


int main(void)
{
	char user_data[] = "Hello world";

	//1. This is function is used initialize the GPIO pins to behave(thay thế) as SPI2 pins
	SPI2_GPIOInits();

	//2. Khởi tạo chức năng SPI2
	SPI2_Inits();

	//2.1 This makes NSS signal internally high(Sử dụng trong trường hợp NSS ko nối với Slave nào)
	SPI_SSIConfig(SPI2,ENABLE);							//Cấu hình NSS lên HIGH để phòng tránh lỗi MODF

	//3. Enable the SPI Peripheral worked
	SPI_PeripheralControl(SPI2, ENABLE);

	//4. Send Data
	SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

	//5. Let comfirm SPI, nếu 1 trả về thì nó sẽ lặp liên tục cho đến khi 0 đc trả về
	while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	//5.1 Disable the SPI Peripheral worked
	SPI_PeripheralControl(SPI2, DISABLE);			//Đóng cổng cho phép truyền

	while(1);

	return 0;
}
