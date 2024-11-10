/*
 * 003_button_interrupt.c
 *
 *  Created on: Nov 21, 2023
 *      Author: Minh
 */
#include <string.h>						//Thư viện hàm memset()
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral

#define HIGH			1
#define LOW				0
#define BTN_PRESSED		HIGH
#define BTN_RELEASED	LOW
/*
 * Hàm Delay thời gian
 */
void delay(void)
{
	//for(uint32_t i = 0; i< 500000; i++);

	//this will introduce ~200ms delay when system clock 16Mhz
	for(uint32_t i = 0; i< 500000/2; i++);		//giảm thời gian delay
}

int main(void)
{
	GPIOx_Handle_t Gpio_Led; 		//Khai báo biến struct chứa thông tin port và pin muốn điều khiển
	GPIOx_Handle_t GPIOBtn;			//Khai báo biến struct chứa thông tin port và pin(input button) muốn điều khiển

	//Tách riêng 2 biến có thành phần cấu trúc giống nhau ra làm hai biến có địa chỉ riêng biệt
	memset(&Gpio_Led,0,sizeof(Gpio_Led));
	memset(&GPIOBtn,0,sizeof(GPIOBtn));

	/*******************************Cấu hình thông tin hoạt động của LED muốn điều khiển************************/
	Gpio_Led.pGPIOx = GPIOD;										//Thiết lập port muốn điều khiển
	Gpio_Led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;		//Chân pin của port muốn điều khiển
	Gpio_Led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;			//Chế độ xuất tín hiệu output
	Gpio_Led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; 		//Tốc độ xuất tín hiệu
	Gpio_Led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;		//Chuẩn xuất Push_Pull

	Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//Đầu ra ko cần điện trở kéo
	//Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;	//Chọn pull-up(kéo lên) vì đầu ra của mạch led là GND

	//Cấu hình xung Clock cho Peripheral Port Led
	GPIO_PeriClockControl(GPIOD, ENABLE);		//Port D và Enalbe=1

	//Gọi hàm khởi tạo hoạt động cấu hình muốn điều khiển(ở đây muốn điều khiển Output Gpio_Led)
	GPIO_Init(&Gpio_Led);


	/*******************************Cấu hình thông tin hoạt động của Button muốn điều khiển**********************/
	GPIOBtn.pGPIOx = GPIOD;											//Thiết lập port muốn điều khiển
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;			//Chân pin của port muốn điều khiển
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;			//Chế độ xuất tín hiệu input, "interrupt Flow Trigger"
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; 		//Tốc độ xuất tín hiệu

	//GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//Đầu vào ko cần điện trở kéo
	Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;			//Chọn pull-up(kéo lên) thông mạch nút nhấn

	//Cấu hình xung Clock cho Peripheral Port Button
	GPIO_PeriClockControl(GPIOD, ENABLE);		//Port D và Enalbe=1

	//Gọi hàm khởi tạo hoạt động cấu hình muốn điều khiển(ở đây muốn điều khiển Input Button)
	GPIO_Init(&GPIOBtn);


	/******************************Cấu hình IRQ configurations****************************/
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, ENABLE);

	while(1);

	return 0;
}

/**
 * Hàm xử lý IRQ liên quan đến ngắt EXTI9_5
 */
void EXTI9_5_IRQHandler()
{
	delay();								//200ms, chống dội phím ngắt
	GPIO_IRQHangling(GPIO_PIN_NO_5);					//thiết lập ngắt nếu có tín hiệu tại vị trí pin
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);		//thực hiện hàm công việc khi có ngắt xảy ra
}
