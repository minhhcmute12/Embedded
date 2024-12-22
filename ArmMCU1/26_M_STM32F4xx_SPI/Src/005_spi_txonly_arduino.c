/*
 * 005_spi_txonly_arduino.c
 *
 *  Created on: Nov 29, 2023
 *      Author: Minh
 *
 *  + Bài tập: Giao tiếp SPI Master(STM32) và SPI Slave(Arduino).
Khi nhấn nút trên master, master sẽ gửi chuỗi dữ liệu đến Arduino Slave được kết nối.
Dữ liệu mà Arduino nhận được sẽ được hiển thị trên cổng nối tiếp Arduino.
  1.Sử dụng chế độ song công hoàn toàn SPI
  Bảng 2.ST sẽ ở chế độ SPI master và Arduino sẽ được cấu hình cho Chế độ SPI Slave
  3.Sử dụng DFF=0
  4.Sử dụng quản lý nô lệ phần cứng (SSM = 0)
  5. Tốc độ SCLK = 2 MHz, fclk = 16 MHz
Trong bài tập này, chủ nhân sẽ không nhận được bất cứ thứ gì cho nô lệ. Vì vậy bạn không thể cấu hình chân MISO
LƯU Ý: Slave không biết chủ sẽ gửi bao nhiêu byte dữ liệu. Vì vậy, trước tiên chủ nhân sẽ gửi số byte
thông tin mà nô lệ sẽ nhận được tiếp theo.
 */
/*
 * Seting SPI2: Datasheet_stm32f407x -> 3. Pinout and pin -> Table 9: Alternate function mapping
 * 	SPI2_MOSI --> PB15
 *  SPI2_MISO --> PB14(Not Use)
 *  SPI2_SCLK --> PB13
 *  SPI2_NSS  --> PB12
 *  ALT function mode --> 5
 */

#include <stm32f407xx_spi_driver.h>		//Khai báo thư viện xử lý SPI peripheral
#include <stm32f407xx.h>
#include <string.h>						//Thư viện hàm memset()
#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral

/**
 * Hàm thiết lập tính năng Alternate chuyển đổi chân Pins từ GPIO sang SPI
 */
void SPI2_GPIOInits(){

	GPIOx_Handle_t SPIpins;										//Khai báo 1 biến cấu trúc Portx muốn dùng AF

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
//	SPIpins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;		//PB14 làm chân AF5 SPI2_MISO
//	GPIO_Init(&SPIpins);										//Thiết lập SPI_MISO pin

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
	GPIO_PeriClockControl(GPIOA, ENABLE);		//Port A và Enalbe=1
	//Ko cần vì đã cấu hình xung clock ở hàm dưới cho SPI

	//Gọi hàm kjhởi tạo hoạt động cấu hình muốn điều khiển(ở đây muốn điều khiển Input Button)
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

int main(void)
{
	char user_data[] = "Hello world";

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

		//4. First send length information
		uint8_t dataLen = strlen(user_data);
		SPI_SendData(SPI2, &dataLen, 1);

		//4.1 Send Data
		SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

		//5. Let comfirm SPI, nếu 1 trả về thì nó sẽ lặp liên tục cho đến khi 0 đc trả về
		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

		//5.1 Disable the SPI Peripheral worked
		SPI_PeripheralControl(SPI2, DISABLE);			//Đóng cổng cho phép truyền
	}

	return 0;
}

