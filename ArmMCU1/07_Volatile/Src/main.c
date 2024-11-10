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
/**
 * Ví dụ 1: Giới thiệu về từ khóa "Volatile"
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include <stdint.h>

#define SRAM_ADDRESS1   0x20000004U

int main(void)
{

  uint32_t value = 0;
  uint32_t volatile *p = (uint32_t *) SRAM_ADDRESS1;

  while(1)
  {
   value = *p;
   if(value) break;    //nếu value khác 0

  }

  while(1);

  return 0;
}

//Tiến hành Debug và quan sát biến value(trong vong lặp while)