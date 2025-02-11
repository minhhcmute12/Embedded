/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/**
 * Vd: Ví dụ về Enable cho ADC Peripheral(V37)
 */

#include<stdint.h>

//Định địa chỉ peripheral
#define ADC_BASE_ADDR   			0x40012000UL //Memory Map ->address base ADC peripheral Register
#define ADC_CR1_REG_OFFSET 			0x04UL   //address offset-phần bù(ADC Control register)
#define ADC_CR1_REG_ADDR  			(ADC_BASE_ADDR + ADC_CR1_REG_OFFSET )  //address official ADC peripheral Register
//Tính năng muốn kích hoạt của ADC peripheral Register 32 bit là tính năng "SCAN-bit thứ 8"

//Định địa chỉ thanh ghi cấp xung clock
//B1: Xác định System Bus dùng kết nối Peripheral ADC1 và vi điều khiển
//Theo Memory Map và Datasheet(Block Diagram) thì ADC1 sẽ đc kết nối với vxl thông qua APB2 Bus
//B2: Xác định address RCC_Register dùng để kích hoạt xung clock
//Vì thông qua APB2 Bus và muốn kích hoạt peripheral nên RCC Register cần sử dụng là RCC_APB2 Peripheral
//Đọc datasheet -> 7.3 RCC Clock -> 7.3.14 RCC_APB2 Peripheral để xác địa chỉ cần thiết và bit cần kích hoạt
#define RCC_BASE_ADDR              0x40023800UL //Memory Map ->address base RCC_Register
#define RCC_APB2_ENR_OFFSET        0x44UL	//address offset-phần bù(RCC_APB2 Peripheral register)
#define RCC_APB2_ENR_ADDR          (RCC_BASE_ADDR + RCC_APB2_ENR_OFFSET ) //address official RCC_APB2 Peripheral register
//Theo datasheet thì trong 32 bit của RCC_APB2 Peripheral register thì bit 8 dùng để enable ADC1_EN

//Vd như ta cần kích hoạt GPIO và kết nối vi điều khiển thông qua system bus là AHB1 thì
//RCC_Register cần sử dụng là RCC_AHB1 Peripheral

int main(void)
{
	uint32_t *pAdcCr1Reg =   (uint32_t*) ADC_CR1_REG_ADDR;   //biến chứa addres ADC peripheral Register
	uint32_t *pRccApb2Enr =  (uint32_t*) RCC_APB2_ENR_ADDR;  //biến chứa addres RCC_APB2 Peripheral register
	//việc chuyển địa chỉ bit sang dạng con trỏ 32 bit nhằm giúp dễ dàng thực hiện các thao tác chuyển đổi trạng thái bit

	//1.Enable the peripheral clock for ADC1 // Enable cho xung clock ngoại vi ADC1
	*pRccApb2Enr |= ( 1 << 8);			//Set bit thứ 8 của RCC_APB2 Peripheral register lên 1 -> RCC_ADC1

    //2. Modify the ADC cr1 register //Sửa đổi đồng thời enable cho thanh ghi ADC CR1
	*pAdcCr1Reg |= ( 1 << 8);		//Set bit thứ 8 của ADC peripheral Register lên 1 -> SCAN

	for(;;);

	//Tiến hành Run and Debug
	//Vào phần show view "Register" và quan sát 2 thanh ghi
	 //Quan sát biến *pRccApb2Enr -> "RCC-> APB2_EN -> ADC1"
	 //Quan sát biến *pAdcCr1Reg -> "ADC1 -> CR1 -> SCAN "
}
