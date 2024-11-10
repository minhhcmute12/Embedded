/*
 * 001led_toggle.c (OPEN DRAIN)
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
	Gpio_Led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;		//Chuẩn xuất Open Drain

	//Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;		//KO dùng điện trở nội kéo
	//Giải thích: nếu ko sử dụng NO_PUPD thì khi ở chế độ Open-Drain(Vcc_P-Mos và Gnd_N_Mos), trong khối Output Control
	//có một cổng đảo nên khi xung 1 qua sẽ thành 0, lúc này cổng Gnd_N_Mos sẽ đc kích hoạt, có nghĩa là lúc này 2 đầu
	//của mạch nối led đang đều cắm về Gnd, sẽ làm led ko sáng
	//Chú ý: mạch ko sử dụng Vcc_P-Mos vì sẽ gây là hiện tượng Open-Drain(Cổng để hở)

	//Giải pháp 1: sử dụng một điện trở kéo lên,
	//Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	//Tuy nhiên sau khi thực hiện, dù led vẫn sáng nhưng cường độ sáng rất bé. Nguyên nhân là khi dùng kéo lên mặc dù sẽ
	//có dòng điện từ Vcc_PU -> Gnd_Led, nhưng khi xung clock tới thì cổng Gnd_N_Mos vẫn sẽ đc kích hoạt, dẫn tới 2 đầu
	//mạch vẫn có lúc cắm về 2 đầu GND trong chu kỳ dài hơn

	//Giải pháp 2: Sử dụng một Điện trở kéo ngoại: VCC->Rx(320Ohm or 470Ohm) -> Phía trước chân Pin -> Led -> Gnd
	//Cách nối: Lấy 5V của board -> Rx(320Ohm or 470Ohm) -> Nối vào PD12
	Gpio_Led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	//Lưu ý: Không nên sử dụng Open Drain cho chương trình nhúng

	//Cấu hình xung Clock cho Peripheral
	GPIO_PeriClockControl(GPIOD, ENABLE);		//Port D và Enalbe=1

	//Gọi hàm kjhởi tạo hoạt động cấu hình muốn điều khiển(ở đây muốn điều khiển Gpio_Led)
	GPIO_Init(&Gpio_Led);

	//Vòng lặp vô hạn
	while(1)
	{
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12); 	//Thiết lập việc sáng tắt chân pin 12 port D peripheral
		delay();
	}
	return 0;
}

