/*
 * 10_i2c_master_rx_testingIT.c
 *
 *  Created on: Jan 8, 2024
 *      Author: Minh
 *  Bài tập: Giao tiếp I2C Master(STM) và I2C Salve(Arduino)
Khi nhấn nút trên master, master sẽ đọc và hiển thị dữ liệu từ Arduino Slave được kết nối. Bước đầu tiên Master lấy được độ
dài của dữ liệu từ Slave để đọc dữ liệu tiếp theo từ Slave
  1. Sử dụng I2C SCL = 100KHz(Chế độ tiêu chuẩn)
  2. Sử dụng điện trở kéo bên trong cho đường SDA và SCL
 */

#include <string.h>						//Thư viện hàm memset()
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral
#include "stm32f407xx_spi_driver.h"		//Khai báo thư viện xử lý SPI peripheral
#include "stm32f407xx_i2c_driver.h"		//Khai báo thư viện xử lý I2C peripheral
#include <stdint.h>
#include <stdio.h>

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

I2C_Handle_t I2C1_Handle;			//biến toàn cục I2C peripheral

#define MY_ADDR	0x61				//Địa chỉ cho STM32 nếu nó là Slave
//Lưu ý: 0xXX bạn có thể chọn địa chỉ tùy ý tuy nhiên cần kiểm tra datasheet để kiểm tra xem có trùng một
//số địa chỉ ko cho phép hay ko (Um10204 3.1.12 Reserved Address)

#define SLAVE_ADDR 0x68			//Địa chỉ Arduino Slave, do phía Arduino lập trình cài đặt

//Vd mẫu data muốn gửi
uint8_t some_Data[] = "We are testing I2C master Tx with interrupt \n";

//Receive Buffer, biến chứa data nhận đc từ Slave, ở đây chỉ nhận tối đa là 32byte ký tự
uint8_t rcv_Buf[32];

//Flag variable - xác định đã hoàn thành việc gửi data từ Master hay chưa
uint8_t rxComplt = RESET;

/*
 * PB6 --> I2C_SCL
 * PB7 --> I2C_SDA
 * ALT function mode : 4
 */

/*********************************************************************
 * @fn      	- I2C1_GPIOInits
 *
 * @brief       - Hàm xử lý Alternate Function từ GPIO pin -> I2C1(SCL,SDA) pin
 *
 * @return      - none
 *
 * @Note        - none
 */
void I2C1_GPIOInits(void)
{
	GPIOx_Handle_t I2CPins;								//Khai báo 1 biến cấu trúc GPIO Portx muốn dùng AF

	I2CPins.pGPIOx = GPIOB;										//Port có chân làm chức năng Alternate
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;		//Thiết lập Port sử dụng Alternate Function
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD; 	//I2C sử dụng Open Drain
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU; 	//pull up
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;				//Column AF4(Check Datasheet)
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;		//Speed

	//SCL Configuration
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;		//PB6 làm chân AF4 I2C_SCL
	GPIO_Init(&I2CPins);						//Thiết lập I2C_SCL cho cấu hình thanh ghi cho Processor

	//SDA Configuration
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;			//PB7 làm chân AF4 I2C_SDA
	GPIO_Init(&I2CPins);						//Thiết lập I2C_SDA cho cấu hình thanh ghi cho Processor
}

/*********************************************************************
 * @fn      	- SPI2_Inits
 *
 * @brief       - Hàm xử lý thiết lập cấu hình I2C do user cài đặt
 *
 * @return      - none
 *
 * @Note        - none
 */
void I2C1_Inits(void)
{
	I2C1_Handle.pI2Cx = I2C1;				//Sử dụng I2C1
	I2C1_Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	I2C1_Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;		//Cài đặt địa chỉ cho STM32
	I2C1_Handle.I2C_Config.I2C_FmDutyCycle = I2C_FM_DUTY_2; //Bộ chia tần số
	I2C1_Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;	//Standard Mode

	I2C_Init(&I2C1_Handle);
}

/*********************************************************************
 * @fn      	- GPIO_ButtonInit
 *
 * @brief       - Hàm xử lý thiết lập cấu hình GPIO do user cài đặt
 *
 * @return      - none
 *
 * @Note        - none
 */
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

	//Gọi hàm khởi tạo hoạt động cấu hình muốn điều khiển(ở đây muốn điều khiển Input Button)
	GPIO_Init(&GPIOBtn);
}

/*
 * Hàm xử lý chính
 */
int main(void)
{
	uint8_t command_code;							//biến chứa các lệnh command từ Master->Slave
	uint8_t len;									//biến chứa giá trị len từ Slave->Master

	//GPIO But Int
	GPIO_ButtonInit();

	//I2C pin Alternate Function
	I2C1_GPIOInits();

	//I2C Peripheral User Configuration
	I2C1_Inits();

	//I2C IRQ configuration(Interrupt)
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV,ENABLE);	//Event Config
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER,ENABLE);	//Error Config

	//Enable the I2C Peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	//Set Acking bit is made 1 after PE=1
	I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);
	//Giải thích: Theo Datasheet bit field ACK của CR1 Register thì bit ACK này được set và xóa bằng phần mềm
	//và thứ hai là Bit ACK này bị xóa bằng phần cứng khi PE=0.
	//Nghĩa là khi PE=0 thì bit Ack luôn luôn là 0(ko đc phép thay đổi-> ko đc phép nhận data)
	//Thêm nữa vì ta làm hàm I2C1_Init trước khi thực hiện I2C_PeripheralControl() nên dù trong hàm I2C1_Init
	//có lệnh set bit ack lên 1 nhưng vì bit PE lúc này đang là 0 nên ko thể set bit ack 1
	//vì vầy, ta cần set PE=1 trước khi set bit ack lên 1(nên tối ưu hóa lại code)

	//printf("Application is running\n");

	while(1)
	{
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		/*********Send**********/
		/* 1. Master gửi mã lệnh 0x51 để đọc độ dài Len(1byte) của dữ liệu gửi từ salve */
		//thiết lập mã command code
		command_code = 0x51;
		//gửi 0x51 từ Master->Slave (while vì kiểm tra bus đang empty hay chưa)
		while(I2C_MasterSendDataIT(&I2C1_Handle, &command_code,1, SLAVE_ADDR,I2C_ENABLE_SR) != I2C_READY);

		//nhận len từ Slave->Master
		//while(I2C_MasterReceiveDataIT(&I2C1_Handle,&len,1,SLAVE_ADDR,I2C_ENABLE_SR) != I2C_READY);
		//&len: con trỏ, cho phép thay đổi value len trong quá trình thực thi hàm
		//rxComplt = RESET;	//reset trạng thái đã hoàn thành rx hay chưa

		/*********Receive**********/
		/* 2. Master gửi mã lệnh 0x52 để đọc dữ liệu hoàn chỉnh từ Slave */
		//thiết lập mã command code
		command_code = 0x52;
		//gửi 0x52 từ Master->Slave
		while(I2C_MasterSendDataIT(&I2C1_Handle, &command_code,1, SLAVE_ADDR,I2C_ENABLE_SR) != I2C_READY);
		//nhận data từ Slave->Master
		//while(I2C_MasterReceiveDataIT(&I2C1_Handle,rcv_Buf,len,SLAVE_ADDR,I2C_DISABLE_SR) != I2C_READY);

		//Nếu rx chưa hoàn thành sẽ lặp và dừng chương trình tại đây
		/*while(rxComplt != SET)
		{

		}*/

		//Vì sử dụng %s để hiển thị nội dung data nên vị trí cuối cùng của mảng phải là giá trị null'\0'
		//rcv_Buf[len+1] = '\0';

		//printf("Data: %s\n",rcv_Buf);		//hiển thị nội dung data Slave->Master

		rxComplt = RESET;
	}
}

//Thiết lập các hành động I2C interrupt bằng cách vào file "startupcode_stm32f407" để
//tìm các hàm sẵn do nhà sản xuất viết sẵn như là cho I2C1
//word	I2C1_EV_IRQHandler           			/* I2C1 event interrupt                                               */
//word	I2C1_ER_IRQHandler 						/* I2C1 error interrupt

/*
 * Lưu ý: cứ mỗi byte data(2 ký tự) truyền hoặc nhận thì ngắt sẽ đc kích hoạt cho đến khi hoàn thành
 */
/**
 * Triển khai I2C Event Interrupt
 */
void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHangling(&I2C1_Handle);
}

/**
 * Triển khai I2C Error Interrupt
 */
void I2C1_ER_IRQHandler(void)
{
	I2C_ER_IRQHangling(&I2C1_Handle);
}

/**
 * Triển khai thông báo cho người dùng các sự kiện diễn ra
 */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv)
{
	if(AppEv == I2C_EV_TX_CMPLT)
	{
		//printf("Tx is Complete\n");
	}
	else if(AppEv == I2C_EV_RX_CMPLT)
	{
		//printf("Rx is Complete\n");

		//chờ đến khi rx đc thực hiện xong sẽ kích hoạt I2C_ApplicationEventCallback
		rxComplt = SET;	//đông thời kích hoạt cho việc tiếp theo
	}
	else if(AppEv == I2C_ERROR_AF)
	{
		//printf("Error: Ack Failure\n");

		//in master ack failure happens when slave fails to send ack for the byte sent from the master
		I2C_CloseSendData(pI2CHandle);		//Khi có lỗi thì đóng chương trình gửi

		//Generate the stop condition - Tạo stop condition cho I2C1 cho bus empty
		I2C_GenerateStopCondition(I2C1);

		//Hang in infinite loop - tính năng do người dạy đang lập trình mong muốn
		while(1);		//khi có lỗi này xảy ra thì tạo vòng lặp vô hạn dừng chương trình

		//Để phát sinh lỗi này thường là ko đúng địa chỉ của bên phía Slave
		//Nên ta sửa lại địa chỉ Slave khác vs địa chỉ Arduino

	}
	else if(AppEv == I2C_ERROR_ARLO)
	{
		//printf("Error: \n");
	}

}
