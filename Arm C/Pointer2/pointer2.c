 /*
 * pointer2.c
 *
 *  Created on: 2021/01/12
 *      Author: Minh
 */

#include <stdio.h>

long long int g_data = 0xFFFEABCD11112345; //FFFE byte6-7 - ABCD byte4-5 - 1111 byte2-3 - 2345 byte0-1

int main(void)
{
/*
	char *pAddress = (char*)&g_data;
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);  //gia tri 0x45 hex

	pAddress = pAddress + 5;
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);
*/

	/*short *pAddress = (short*)&g_data;
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);  //gia tri 0x45 hex

	pAddress = pAddress + 2;
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);
*/

	int *pAddress = (int*)&g_data;
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);  //gia tri 0x45 hex

	pAddress = pAddress + 1;   //trong con tro kiểu int +1 di chuyen 4byte(4 địa chỉ)
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);

	//Giải thích việc + 1 mà di chuyển 4 byte
	//Vì con trỏ đc khai báo là kiểu int nên nó cần 4 byte để lưu dữ liệu
	//mà 1 địa chỉ sẽ lưu 1 byte, nên 4 địa chỉ sẽ chiếm 4 byte
	//khi +1 con trỏ nghĩa là ta cần đến con trỏ tiếp theo nên ta cần
	//di chuyển qua 4 byte mà con trỏ hiện tại đang chiếm để lưu biến int.
	//Vì vậy tùy thuộc vào kiểu dữ liệu con trỏ mà số địa chỉ chạy sẽ khác nhau.

	//Kết quả việc +1 con trỏ:
//	Value at address 0000000000403010
//	Value at address 0000000000403010 is: 11112345
//	Value at address 0000000000403014
//	Value at address 0000000000403014 is: fffeabcd
	//Ta thấy rằng con trỏ x sẽ nằm ở địa chỉ 0403010
	//và con trỏ x+1 sẽ nằm ở vị trí 0403014, cách 4byte(địa chỉ) so vs con trỏ chuẩn x

	//Nếu con trỏ x là 1 biến kiểu char thì vị trí con trỏ x+1 chỉ cách 1 byte(địa chỉ)
	//vì biến kiểu char chỉ có 1 byte lưu dữ liệu

	//=> vì vậy tùy vào biến con trỏ thuộc kiểu dữ liệu nào thì vị trí x+1 sẽ khác nhau
}
