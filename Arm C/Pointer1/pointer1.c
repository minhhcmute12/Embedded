/*
 * pointer1.c
 *
 *  Created on: 2021/01/12
 *      Author: Minh
 */
#include <stdio.h>

long long int g_data = 0xFFEABCD11112345; //FFFE byte7-8 - ABCD byte4-5 - 1111 byte2-3 - 2345 byte0-1
											//lưu ý: 1byte = 8bit = 45 = 0010 1101

int main(void)
{
	char *pAddress1;
	//Giải thích về việc ép kiểu con trỏ:
	//Nếu ta ghi: pAddress1 = &g_data;  //không có ép kiểu
	//thì chương trình vẫn hoạt động nhưng nó sẽ đưa ra 1 warning là có thể mất mát dữ liễu trong quá trình gán
	//Tức là ta dùng 1 biến kiểu char(1byte) để đọc về 1 biến kiểu long long int(8byte) thì chỉ lưu đc
	//data đến giới hạn kiểu char, byte vượt quá kiểu byte sẽ ko đc lưu.
	//Vì vậy cần tiến hành ép kiểu để thông báo trước cho chương trình về định dạng của biến đc gán về

	pAddress1 = (char*)&g_data;		//(char*) : ép kiểu con trỏ từ kiểu số nguyên về kiểu ký tự
	printf("Value at address %p is: %x\n",pAddress1,*pAddress1);  //*pAddress1=45
	//%x : Số nguyên hex không dấu (hệ 16)
	//Có nghĩa là chuyển value đang chứa trong địa chỉ đc lưu trong con trỏ sang hệ số 16
	//Tuy nhiên g_data đã đc chuyển sang hệ 16 nên sau khi chuyển đổi sang cùng 1 giá trị
	//=45 vì biến char chỉ lấy 1 byte nên nó sẽ lấy giá trị 1 byte cuối là 45

	int *pAddress2;
	pAddress2 = (int*)&g_data;
	printf("Value at address %p is: %x\n",pAddress2,*pAddress2);  //*pAddress2=11 11 23 45
	//Biến int lấy 4 byte

	short *pAddress3;
	pAddress3 = (short*)&g_data;
	printf("Value at address %p is: %x\n",pAddress3,*pAddress3);	//*pAddress3=23 45
	//biến short lấy 2 byte

	long long *pAddress4;
	pAddress4 = (long long*)&g_data;
	printf("Value at address %p is: %I64x\n",pAddress4,*pAddress4);	//*pAddress3= f fe ab cd 11 11 23 45
	//biến long long lấy 8 byte

	return 0;
}
//Lưu ý: ở trong lập trình stm32 vì sử dụng kiểu 32 bit nên thường sử dụng các kiểu biến nằm trong
//thư viện stdint.h. Vì vậy sẽ ít sử dụng các kiểu biến tường minh phía trên.
