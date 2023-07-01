/*
 * Applicability of unions in embedded system code
 1.Bit extraction
 2.Storing mutually exclusive data thus saving memory
 *
 * Khả năng ứng dụng của union trong mã hệ thống nhúng
 1. Khai thác(phân lạo, chiết xuất) bit
 2. Lưu trữ dữ liệu loại trừ(ghi đè) lẫn nhau do đó tiết kiệm bộ nhớ
 *
 */
#include<stdint.h>
#include<stdio.h>

/*
 * This structure we used to store various fields of the packet in to variables
 * The variable of this structure consumes 4 bytes in the memory
*/

//Sử dụng Union và struct(pp trường bit cấu trúc) để giải mã 1 đoạn thông tin 32bit thành các thành phần
union Packet
{
	uint32_t packetValue;

	struct
	{
		uint32_t crc			:2;
		uint32_t status			:1;
		uint32_t payload		:12;
		uint32_t bat			:3;
		uint32_t sensor			:3;
		uint32_t longAddr		:8;
		uint32_t shortAddr		:2;
		uint32_t addrMode		:1;
	}packetFields;
};

int main(void)
{

   union Packet packet;
   printf("Enter the 32bit packet value:");
   scanf("%X",&packet.packetValue);

   printf("crc      :%#x\n",packet.packetFields.crc);
   printf("status   :%#x\n",packet.packetFields.status);
   printf("payload  :%#x\n",packet.packetFields.payload);
   printf("bat      :%#x\n",packet.packetFields.bat);
   printf("sensor   :%#x\n",packet.packetFields.sensor);
   printf("longAddr :%#x\n",packet.packetFields.longAddr);
   printf("shortAddr:%#x\n",packet.packetFields.shortAddr);
   printf("addrMode :%#x\n",packet.packetFields.addrMode);
   //Ta thấy điểm khác biệt của việc dùng Union và Struct làm thành phần là ko cần dùng
  //pp thao tác bit mà ta vẫn sẽ chia tách đc các thông tin dữ liệu theo yếu cầu

  /*
   * Giải thích hoạt động.
   * Sau khi khai báo Union thì bộ nhớ đc cấp của Union này là 4byte, có nghĩa là biến packetValue
   * và biến struct packetFields sẽ sử dụng 1 vung nhớ 4byte để lưu dữ liệu.
   * Lưu ý: các thành phần của struct sẽ chia nhau 4byte ô nhớ theo pp trường cấu trúc bit
   * Nói cách khác, 1 thời điểm chỉ có 1 thành phần của Union đc phép lưu giá trị. Có nghĩa là tại 1 thời
   * điểm chỉ đc lưu,gán giá trị của packetValue hay packetFields.
   *
   * Sau khi packetValue đc gán giá trị thì có nghĩa cùng nhớ của Union sẽ là giá trị của packetValue.
   * Nói cách khác giá trị của packetFields cũng là giá trị của packetValue.
   * Khi đó vì biến struct ta đã làm khai báo trường bit cấu trúc nên trình biên dịch sẽ tự động phân
   * bổ các giá trị vào các biến thành phần của struct mà ko cần sử dụng pp thao tác bit.
   * */
   printf("Size of union is %I64u\n",sizeof(packet));

   while(getchar() != '\n');
   getchar();

	return 0;
}

