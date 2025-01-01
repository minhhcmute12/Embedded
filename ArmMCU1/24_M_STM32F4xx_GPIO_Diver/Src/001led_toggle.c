/*
 * 001led_toggle.c
 *
 *  Created on: Nov 17, 2024
 *      Author: Minh
 */
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

/**
 * Hàm delay
 */
void delay(void)
{
	for(uint32_t i = 0; i<500000; i++);			//Option 1

	//for(uint32_t i = 0; i<500000/2; i++);		//Option 2
}

int main(void)
{
	//Khai báo biến chứa thông tin cấu hình port và pin muốn điều khiển
	GPIOx_Handle_t Gpio_led;

	//Cấu hình thông tin hoạt động của LED muốn điều khiển
	/*
	 * VD1: Led On board
	 */
	/*Gpio_led.pGPIOx = GPIOD;										//Cấu hình port muốn điều khiển
	Gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;		//Cấu hình pin muốn điều khiển
	Gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;			//Cấu hình chế độ xuất tín hiệu
	Gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;		//Tốc độ xuất tín hiệu
	Gpio_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;		//Chuẩn xuất Push-Pull
	Gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//Không sử dụng điện trở kéo vì sử dụng led nội của on board*/

	/*
	 * VD2: Led Out board
	 */
	Gpio_led.pGPIOx = GPIOD;										//Cấu hình port muốn điều khiển
	Gpio_led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;		//Cấu hình pin muốn điều khiển
	Gpio_led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;			//Cấu hình chế độ xuất tín hiệu
	Gpio_led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;		//Tốc độ xuất tín hiệu
	Gpio_led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;		//Chuẩn xuất Push-Pull
	Gpio_led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;		// sử dụng điện trở kéo vì sử dụng led nội của on board


	//Cấu hình xung clock cho Peripheral
	GPIO_PeriClockControl(GPIOD, ENABLE);							//Port D và Enable=1

	//Khởi tạo hoạt động cấu hình muốn điều khiển(cấu hình peripheral register theo cấu hình biến Gpio_led)
	GPIO_Init(&Gpio_led);

	while(1)
	{
		//VD1: Bật sáng tắt led(chân pin 12) port D
		//GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);

		//VD2: Bật sáng tắt led(chân pin 0) port D
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_0);

		//delay();
	}

	return 0;
}

