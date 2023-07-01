/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


/*
Address of clock control register(AHB1ENR)
0x40023800+0x30 => 	0x40023830

Address of the GPIO mode register (used to control mode)
0x40020C00 + 0x00 	=>	0x40020C00

Address of the GPIO output data register (used to wroite)
0x40020C00 + 0x14  =>  0x40020C14
*/

#include <stdint.h>

int main(void)
{
	//Phần thiết lập các cấu hình ban đầu cho hệ thống nhúng
    uint32_t *pClkCtrlReg = (uint32_t*)0x40023830;
    uint32_t *pPortDmodeReg = (uint32_t*)0x40020C00;
    uint32_t *pPortDoutReg = (uint32_t*)0x40020C14;


    //II. Su dung phuong phap dich bit
    //1.Bat dong ho cho thiet bi ngoai vi AHB1ENR set bit 3
    *pClkCtrlReg |= ( 1 << 3 );

    //2.Cau hinh che do chan IO lam dau ra
    //a.clear the bit 24 and 25
    *pPortDmodeReg &= ~ (3 << 24 ) ;    //3 = 11 : set bit 24,25 ve 0

    //b. set bit 24 len 1
    *pPortDmodeReg |= ( 1 << 24 );		//dich bit cho chan 24 len 1
    //Kết thúc cấu hình ban đầu cho hệ thống

    //Xử lý các hành động sẽ lặp đi lặp lại trong hệ thống
    while(1)
    {
    //3. Set chan 12 GPIO len 1 cho LED sang
    *pPortDoutReg |= ( 1 << 12 );		//dich bit cho chan 12 len 1

    //thiet lap ham delay led
    //tao ra n vong lap, để để làm chậm nhịp xử lý
    for(uint32_t i = 0 ; i < 300000 ; i++);

    //Turn off LED
    *pPortDoutReg &= ~( 1 << 12 );

    for(uint32_t i = 0 ; i < 300000 ; i++);
    }
}

