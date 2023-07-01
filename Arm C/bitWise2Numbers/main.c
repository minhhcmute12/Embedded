/*
 * Toán tử Bitwise: Các toán tử kỹ thuật số và các phép dịch bit
 * Toán tử AND(&) : ngõ ra bằng 1 khi cả 2 ngõ vào đều bằng 1
 * Toán tử OR(|) : ngõ ra bằng 1 khi chỉ cần 1 trong 2 ngõ vào bằng 1 hoặc cả 2 ngõ đều bằng 1
 * Toán tử NOT(!) : giá trị ngõ ra sẽ ngược với giá trị ngõ vào
 * Toán tử XOR (^) : giá trị ngõ ra bằng 1 khi và chỉ khi 1 trong 2 ngõ vào là 1
 */

#include<stdio.h>
#include<stdint.h>  //thư viện định nghĩa lại cách khai báo biến
				//mục đích giúp người lập trình biết đc ngay số bit- số byte mà biến đó sẽ sử dụng
			//vd: int8_t = char hay int32_t = int
			//khi quên thì lập trình viên nên vào thư viện để xem lại cách định nghĩa biến

void wait_for_user_input(void);


int main(void)
{
	int32_t num1 , num2;
	printf("Enter 2 numbers(give space between 2 nos):");
	scanf("%d %d",&num1,&num2);

	printf("Bitwise AND(&) : %d\n",(num1 & num2));
	printf("Bitwise OR (|) : %d\n",(num1 | num2));
	printf("Bitwise XOR(^) : %d\n",(num1 ^ num2));
	printf("Bitwise NOT(~) : %d\n",(~num1));

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
