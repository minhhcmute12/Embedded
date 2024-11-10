/*
 * 001led_toggle.c (PUSH_PULL)
 *
 *  Created on: Nov 19, 2023
 *      Author: Minh
 */
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral

/*
 * Hàm Delay thời gian
 */
void delay(void)
{
	for(uint32_t i = 0; i< 500000; i++);
	//for(uint32_t i = 0; i< 500000/2; i++);		//giảm thời gian delay
}

int main(void)
{
	GPIOx_Handle_t Gpio_Led;		//Khai báo biến struct chứa thông tin port và pin muốn điều khiển

	//Cấu hình thông tin hoạt động của LED muốn điều khiển
	Gpio_Led.pGPIOx = GPIOD;										//Thiết lập port muốn điều khiển
	Gpio_Led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;		//Chân pin của port muốn điều khiển
	Gpio_Led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;			//Chế độ xuất tín hiệu output
	Gpio_Led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; 		//Tốc độ xuất tín hiệu
	Gpio_Led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;		//Chuẩn xuất Push_Pull

	Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//Đầu ra ko cần điện trở kéo(dùng led nội của board)
	//Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;	//Chọn pull-up(kéo lên) vì đầu ra của mạch led là GND

	//Cấu hình xung Clock cho Peripheral
	GPIO_PeriClockControl(GPIOD, ENABLE);		//Port D và Enalbe=1

	//Gọi hàm khởi tạo hoạt động cấu hình muốn điều khiển(ở đây muốn điều khiển Gpio_Led)
	GPIO_Init(&Gpio_Led);

	//Vòng lặp vô hạn
	while(1)
	{
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12); 	//Thiết lập việc sáng tắt chân pin 12 port D peripheral
		delay();
	}
	return 0;
}

