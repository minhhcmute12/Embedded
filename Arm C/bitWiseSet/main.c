/*
 * Phương pháp bit_masking(hay gọi là bit mặt nạ):
 * Mục đích của phương pháp này là dùng 1 bit mặt nạ và bitWise với bit data nhằm thay đổi trạng thái của 1 hay nhiều bit data.
 *
 * VD: thay đổi trạng thái bit từ 0 thành 1 hoặc từ 1 thành 0
 * Cách 2(Settings of bit): dùng toán tử bitwise OR(|) với 1 dãy bit mặt ta mong muốn
 * VD: [data] 00111110
 * 				&
 * 	   [mask] 10010000
 * 	   [out]  10111110
 *
 * -> Phương pháp này thường đc sử dụng khi ta muốn đảo trạng thái tất cả các bit bên trong data nguồn
 *
 * Cách 2(Toggling of bit): dùng toán tử bitwise XOR(^) với 1 dãy bit mặt ta mong muốn
 * VD: [data] 00000001
 * 				^
 * 	   [mask] 00000001	: 0x01
 * 	   [out]  00000000
 * 	   			^
 * 	   [mask] 00000001	: 0x01
 * 	   [out]  00000001
 * 	   			^
 * 	   [mask] 00000001	: 0x01
 * 	   [out]  00000000
 *
 * -> Phương pháp này thường dùng khi ta muốn đảo trạng thái bit tại 1 vị trí mong muốn
 * -> Thường đc sủ dụng trong việc đảo trạng thái led
 */

#include<stdio.h>
#include<stdint.h>

void wait_for_user_input(void);


int main(void)
{
	int32_t num1,output;
	printf("Enter a number:");
	scanf("%d",&num1);

	output = num1 | 0x90;
	printf("[input] [output] :0x%x 0x%x\n",num1, output);

    wait_for_user_input();
}


void wait_for_user_input(void)
{

	printf("Press enter key to exit this application");

    while(getchar() != '\n')
    {
    	//just read the input buffer and do nothing
    }
    getchar();

}
