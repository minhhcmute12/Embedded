/*
 * main.c
 *
 *  Created on: 2021/01/11
 *      Author: Minh
 */

#include <stdio.h>
#include "math.h"

int main(void)
{
	printf("Add : %X\n",math_add(0x0FFF1111, 0x0FFF1111));

	printf("Sub : %d\n",math_sub(0x0FFF1111, 0x0FFF1111));

	printf("Mul : %I64x\n",math_mul(0x0FFF1111, 0x0FFF1111));

	printf("Div : %f\n",math_div(100,8));

	return 0;
}
