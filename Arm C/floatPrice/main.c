/*
 * main.c
 *
 *  Created on: 2021/01/12
 *      Author: Minh
 *      Giới thiệu cách hoạt động lưu một số float, double trong nhúng
 */
#include <stdio.h>

int main(void)
{
	float number = 45.78976834578;
	printf("Number = %0.2f\n",number);			//0.2f: lấy 2 số sau dấu thập phân
	printf("Number = %0.2e\n",number);			//e: hiển thị số theo cơ số 10, số e mũ n

	double chargeE = -1.60217662e-19;				//dien tich 1 eclectron
	printf("chargeE = %0.28lf\n",chargeE);
	printf("chargeE = %0.8le\n",chargeE);

	return 0;
}

