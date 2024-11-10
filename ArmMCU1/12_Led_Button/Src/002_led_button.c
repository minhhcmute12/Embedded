/*
 * 002_led_button.c (INTERNAL)
 *
 *  Created on: Nov 19, 2023
 *      Author: Minh
 */
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral

#define HIGH			1
#define BTN_PRESSED		HIGH
#define BTN_RELEASE		LOW

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
	GPIOx_Handle_t Gpio_Led; 		//Khai báo biến struct chứa thông tin port và pin muốn điều khiển
	GPIOx_Handle_t GPIOBtn;			//Khai báo biến struct chứa thông tin port và pin(input button) muốn điều khiển

	//Cấu hình thông tin hoạt động của LED muốn điều khiển
	Gpio_Led.pGPIOx = GPIOD;										//Thiết lập port muốn điều khiển
	Gpio_Led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;		//Chân pin của port muốn điều khiển
	Gpio_Led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;			//Chế độ xuất tín hiệu output
	Gpio_Led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; 		//Tốc độ xuất tín hiệu
	Gpio_Led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;		//Chuẩn xuất Push_Pull

	Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//Đầu ra ko cần điện trở kéo(dùng led nội của board)
	//Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;	//Chọn pull-up(kéo lên) vì đầu ra của mạch led là GND

	//Cấu hình xung Clock cho Peripheral Port Led
	GPIO_PeriClockControl(GPIOD, ENABLE);		//Port D và Enalbe=1

	//Gọi hàm khởi tạo hoạt động cấu hình muốn điều khiển(ở đây muốn điều khiển Output Gpio_Led)
	GPIO_Init(&Gpio_Led);


	//Cấu hình thông tin hoạt động của Button muốn điều khiển
	GPIOBtn.pGPIOx = GPIOA;										//Thiết lập port muốn điều khiển
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;		//Chân pin của port muốn điều khiển
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;			//Chế độ xuất tín hiệu input
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; 	//Tốc độ xuất tín hiệu

	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//Đầu vào ko cần điện trở kéo
	//Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;	//Chọn pull-up(kéo lên)

	//Cấu hình xung Clock cho Peripheral Port Button
	GPIO_PeriClockControl(GPIOA, ENABLE);		//Port D và Enalbe=1

	//Gọi hàm kjhởi tạo hoạt động cấu hình muốn điều khiển(ở đây muốn điều khiển Input Button)
	GPIO_Init(&GPIOBtn);

	//Vòng lặp vô hạn chương trình
	while(1)
	{
		//Nếu button đc nhấn
		if(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == BTN_PRESSED)
		{
			delay();	//hàm chống dội, ngăn ngừa button de-bouncing
			GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12); 	//Thiết lập việc sáng tắt chân pin 12 port D peripheral
		}

	}

	return 0;
}

//GPIOBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;		//Chuẩn xuất Push_Pull
//KO sử dụng vì trường này chỉ sử dụng khi output

//Ko cần sử dụng điện trở kéo vì trong sở đồ nút nhấn(internal buuton) board stm32f407Disc1 đã đc thiết kế sẵn.
//Vì vậy nếu bạn sử dụng một external button thì cần phải quan tâm đến điện trở kéo
