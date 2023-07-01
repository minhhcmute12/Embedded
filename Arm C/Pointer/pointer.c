/*
 * pointer.c
 *
 *  Created on: 2021/01/12
 *      Author: Minh
 */
#include <stdio.h>

/*
	1. Create a char type variable and initialize it to value 100
	2. Print the address of the above variable.
	3. Create a pointer variable and store the address of the above variable
	4. Perform read operation on the pointer variable to fetch 1 byte of data from the pointer
	5. Print the data obtained from the read operation on the pointer.
	6. Perform write operation on the pointer to store the value 65
	7. Print the value of the variable defined in step 1

	1. Tạo một biến kiểu char và khởi tạo nó với giá trị 100
	2. In ra địa chỉ của biến trên.
	3. Tạo biến con trỏ và lưu địa chỉ của biến trên
	4. Thực hiện thao tác đọc trên biến con trỏ để lấy 1 byte dữ liệu từ con trỏ
	5. In dữ liệu thu được từ thao tác đọc trên con trỏ.
	6. Thực hiện thao tác ghi trên con trỏ để lưu giá trị 65
	7. In ra giá trị của biến đã xác định ở bước 1
*/
int main(void)
{
	char data = 100;
	printf("Value of data is: %d\n",data);
	printf("Address of the variable data is: %p\n",&data);			//%p: địa chỉ mà con trỏ đang chứa

	//Create a pointer variable and store the address of the above variable
	char* pAddress = &data;

	char value = *pAddress;
	printf("Value of value is: %d\n",value);

	*pAddress = 65;
	printf("Value of data is: %d\n",data);

	return 0;
}

