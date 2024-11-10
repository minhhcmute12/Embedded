/*
 * volatile_interrupt.c
 *
 *  Created on: Oct 22, 2023
 *      Author: Minh
 */
/**
 * Ví dụ về ngắt và sử dụng volatile để xử lý vấn đề value biến có thể thay đổi bất cứ lúc nào
 */

#include <stdint.h>
#include <stdio.h>

//global shared variable between main code and ISR
uint8_t volatile g_button_pressed = 0;   //biến toàn cục xử lý ngắt

uint32_t g_button_press_count =0;

void button_init(void);

//Thiết lập các cấu hình địa chỉ các port sử dụng với từ khóa 'volatile'
//Theo Memory map thì địa chỉ bắt đầu của EXTI là 0x40013C00 kết nối vi xử lý thông qua APB2 Bus
uint32_t volatile *pEXTTIPendReg			= (uint32_t*) (0x40013C00 + 0x14);
uint32_t volatile *pClkCtrlReg				= (uint32_t*) (0x40023800 + 0x30);
uint32_t volatile *pClkCtrlRegApb2			= (uint32_t*) (0x40023800 + 0x44);
uint32_t volatile *pGPIOAModeReg 			= (uint32_t*) (0x40020000 + 0x00);
uint32_t volatile *pEXTIMaskReg 			= (uint32_t*) (0x40013C00 + 0x00);
uint32_t volatile *pEXTTIEdgeCtrlReg		= (uint32_t*) (0x40013C00 + 0x08);
uint32_t volatile *pNVICIRQEnReg 			= (uint32_t*) 0xE000E100;

int main(void)
{
	button_init();   //hàm cấu hình chân ngắt GPIOA

	while(1)
	{
		//Disable interrupt  //tắt chế độ cho phép sử dụng ngắt thanh ghi pEXTIMaskReg
		*pEXTIMaskReg &= ~( 1 << 0);

		if(g_button_pressed){   //nếu có nút đc cấu hình ngắt được nhấn

			//Some delay until button debouncing gets over
			for(uint32_t volatile i=0;i<500000/2;i++);  //delay chương trình

			g_button_press_count++;  //tăng biến counter

			printf("Button is pressed : %lu\n",g_button_press_count); //in value biến counter

			g_button_pressed = 0;	//reset lại giá trị biến ngắt, ko còn đc nhấn
		}

		//Enable interrupt //bật chế độ cho phép sử dụng ngắt thanh ghi pEXTIMaskReg
		*pEXTIMaskReg |= ( 1 << 0);
		//Phải bật/tắt thanh ghi pEXTIMaskReg vì lúc này vd muốn cho ta thấy cách hoạt động khi có
		//từ khóa 'volatile'. Nếu bật thanh ghi pEXTIMaskReg thì khi có nút đc nhấn thì trình xử lý
		//sẽ tự động nhảy vào hàm EXTI0_IRQHandler để xử lý rồi sau đó mới quay lại hàm if.
		//Trên thực tế, khi tạo ngắt chỉ cần viết hàm ngắt chứ ko cần viết hàm if, việc viết hàm if
		//ở vd này chỉ minh họa việc sử dụng 'volatile'.
	}

	//Giải thích hoạt động chương trình khi "KHÔNG và CÓ" dùng từ khóa "Volatile"
	//Kết quả: Khi chưa thiết lập từ khóa 'volatile' cho các địa chỉ thanh ghi thì khi chạy với mức
	//tối ưu hóa càng cao thì việc nhấn nút ngắt càng xử lý chậm và không chính xác. Xuất hiện tình trạng
	//nhiệm vụ ngắt không đc thực hiện nhưng giá trị biến g_button_press_count vẫn tăng lên. Nguyên nhân là
	//tối ưu hóa trình biên dịch cho rằng biến ngắt là một biến không đc sử dụng trong chương trình, và bỏ
	//qua ko xử lý trong quá trình tối uu hóa.
	//còn biến g_button_press_count là một biến có sử dụng do nó nằm trong một biểu thức tính toán.

	//Và sau khi thêm từ khóa 'volatile' vào thì dù ở mức tối ưu hóa nào thì trình biên dịch
	//đều sẽ thực hiện lệnh ngắt chính xác do nó phải thực hiện lệnh có biến g_button_pressed(đang đc
	//khai báo dùng 'volatile')
}

//Cấu hình địa chỉ thanh ghi cho nút nhấn
void button_init(void)
{
  //GPIOA clock enable
  *pClkCtrlReg |= ( 1 << 0);

  //Syscfg clock enable
  *pClkCtrlRegApb2 |= ( 1 << 14);

  //Edge detection configuration - Cấu hình phát hiện cạnh xung lên là nhấn
  *pEXTTIEdgeCtrlReg |= ( 1 << 0);

  //EXTI interrupt enable
  *pEXTIMaskReg |= ( 1 << 0);
  //KO cần phải kích hoạt bit tương ứng của EXTI0 trên Pending Register vì bit0 sẽ tự động set lên 1
  //nếu như EXTI0 đc set lên 1

  //NVIC irq enable
  *pNVICIRQEnReg |= ( 1 << 6);
}

/* This is button interrupt handler-Hàm xử lý nút nhấn ngắt_interrupt thông qua EXTI0*/
void EXTI0_IRQHandler(void)
{
	//Make this flag SET . if button pressed
	g_button_pressed = 1;  //lưu giá trị để biết đang đc nhấn

	//Clearing of EXTI interrupt peding bit - xóa bit tương ứng bên trong Pending register
	*pEXTTIPendReg |= ( 1 << 0);
}


