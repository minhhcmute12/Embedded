/*
 * Viết hàm xử lý array
 * */

#include<stdio.h>
#include<stdint.h>

void array_display(uint8_t const *const pArray, uint32_t nItmes);

int main(void)
{
	//Khai báo đồng thời set value ban đầu cho các phân tử mảng
	uint8_t someData[10] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

	//Vd2: Set value cho phần tử mảng
	for(uint32_t i = 0 ; i < 10 ; i++)
	{
		someData[i] = i;
	}

	//Độ dài của mảng và độ dài của 1 phần tử của mảng
	printf("%I64u \t %I64u \n",sizeof(someData),sizeof(uint8_t)); //10 và 1

	uint32_t nItems = sizeof(someData) / sizeof(uint8_t);

	array_display(someData,nItems);    //vd1: Hiển thị toàn bộ
	array_display(&someData[2],nItems-2);  //vd3: hiển thì từ vị trí và số lần lặp mong muốn

	return 0;
}

//const *const: ko cho phép thay đổi giá trị và địa chỉ của biến mảng đưa vào
void array_display(uint8_t const *const pArray, uint32_t nItmes)
{
	//lets display the contents of the received array
	for(uint32_t i = 0 ; i < nItmes ; i++)
	{
		printf("%x\t",pArray[i]);
	}

}
