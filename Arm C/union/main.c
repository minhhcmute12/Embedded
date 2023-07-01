/*
 * Union(V166)
 * Union là một kiểu dữ liệu đặc biệt trong ngôn ngữ lập trình C, cho phép lưu trữ các giá trị
 * khác nhau trong cùng một vùng nhớ. Có nghĩa là các thành phần đc khai báo trong Union sẽ
 * phải dùng chung với nhau 1 vùng nhớ(nói cách khác sẽ cùng chung vs nhau 1 giá trị).
 *
 * Union giống như một cấu trúc(struct) vì chúng cũng cho phép lưu trữ các thành phần khác nhau.
 * Tuy nhiên, trong union, chỉ có một thành phần được sử dụng tại một thời điểm(chi có 1 value của
 * 1 thành phần đc lưu tại 1 thời điểm).

 * Một union được định nghĩa bởi từ khóa union và bao gồm một hoặc nhiều thành viên (members),
 * mỗi thành viên có thể là một kiểu dữ liệu khác nhau. Khi khai báo một biến union, bộ nhớ
 * sẽ được cấp phát cho thành viên lớn nhất trong union.
 *
 * Union thường được sử dụng khi cần lưu trữ các giá trị khác nhau trong cùng một biến hoặc
 * khi cần giảm thiểu sử dụng bộ nhớ. Union cũng có thể được sử dụng để tạo ra các kiểu dữ liệu
 * mới bằng cách kết hợp các thành viên của chúng.
 * */

#include<stdint.h>
#include<stdio.h>

union Address
{
	uint16_t shortAddr;
	uint32_t longAddr;
};

int main(void)
{

	union Address addr;

	addr.shortAddr = 0xABCD;
	addr.longAddr = 0xEEEECCCC;

	printf("short addr = %#X\n",addr.shortAddr);
	printf("long addr = %#X\n",addr.longAddr);

	/*
	 * Giải thích hoạt động:
	 * Sau khi Union được khai báo, thì trình biên dịch sẽ lựa chọn kiểu data có số byte lớn nhất được
	 * khai báo trong Union trở thành số ô nhớ của Union. Vd ở trên biến longAddr được khai báo 4byte
	 * là số byte lớn nhất đc khai báo nên sẽ thành số byte mà Union đó chiếm trong bộ nhớ.
	 * Khi gán "addr.shortAddr = 0xABCD;" thì tại thời điểm này giá trị của biến shortAddr sẽ đc lưu
	 * trên 4byte bộ nhớ của Union.
	 * Sau đó, "addr.longAddr = 0xEEEECCCC;" đc gán thì giá trị biến longAddr sẽ đc ghi đề lên 4byte
	 * mà trước đó đã ghi giá trị của biến shortAddr, có nghĩa là tại thời điểm này đã ko còn lưu
	 * giá trị của biến shortAddr nữa mà đang là giá trị của biến longAddr.
	 * ->Đây chính là ý nghĩa ở mỗi thời điểm chỉ đc phép lưu trữ 1 value của các biến thành phần
	 * ->Lưu ý lúc này giá trị của biến shortAddr cũng là 0xEEEECCCC -> đây chính là ý nghĩa các thành
	 * phần của Union có chung 1 vùng nhớ.
	 * */

	getchar();

	return 0;

}
