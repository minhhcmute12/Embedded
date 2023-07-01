/*
 * Bài tập: Con trỏ và struct(vd2)
 * Viết chương trình giải mã thông tin gói 32 bit đã cho và in giá trị của các trường khác nhau.
 * Tạo một cấu trúc với các phần tử thành viên là các trường gói tin như hình bên dưới
 * Tiến hành giảm thiểu kích thước bộ nhớ khi sử dụng struct bằng cách sư dụng 'structure bit fileds'
 *
 * structure bit fileds: trường bit cấu trúc, quy định số bit đc nhận(cần chiếm) của thành phần con
 * trong struct
 * */

#include<stdint.h>
#include<stdio.h>

/*
 * This structure we used to store various fields of the packet in to variables
 * The variable of this structure consumes 4 bytes in the memory
*/
/*
  * Cấu trúc này chúng tôi đã sử dụng để lưu trữ các trường khác nhau của gói vào các biến
  * Biến của cấu trúc này chiếm 4 byte trong bộ nhớ
*/

//Khai báo struct sử dụng structure bit fileds
struct Packet
{
	uint32_t crc			:2;  //thành phần con này sẽ chiếm 2 bit trong 32 bit
	uint32_t status			:1;
	uint32_t payload		:12;
	uint32_t bat			:3;
	uint32_t sensor			:3;
	uint32_t longAddr		:8;
	uint32_t shortAddr		:2;
	uint32_t addrMode		:1;
};
/*
 * Giải thích: thay vì lựa chon từng kiểu biến phù hợp cho các thành phần của biến cấu trúc
 * để có thể lấy về đúng số bit cần khi tách
 * thì ta sẽ lựa chọn kiểu biến đáp ứng số bit thông tin cần tách và gán số bit cần chiếm
 * cho mỗi thành phần
 * vd: ta cần tách 1 thông tin 32bit thì tất cả các thành phần struct đều khai báo kiểu uint32_t(32 bit),
 * sau đó ta thiết lập số bit cần chiếm của mỗi thành phần của truct
 * lúc này ta xem các thành phần sẽ chiếm 1 số bit nào đó trong tổng số 32 bit cần tách
 * như vậy ta chỉ cần 32bit tương ứng với 4byte là ta đạt được mục đích
 * */

int main(void)
{

   uint32_t packetValue ;
   printf("Enter the 32bit packet value:");
   scanf("%X",&packetValue);

   struct Packet packet;

   //tiến hành gán các giá trị cho các thành phần struct(sử dụng pp thao tác bit)
   packet.crc 		=   (uint8_t) (packetValue & 0x3);
   packet.status 	=   (uint8_t) ( (packetValue >> 2) & 0x1 );
   packet.payload   =   (uint16_t) ( (packetValue >> 3) & 0xFFF );
   packet.bat       =   (uint8_t) ( (packetValue >> 15) & 0x7 );
   packet.sensor    =   (uint8_t) ( (packetValue >> 18) & 0x7 );
   packet.longAddr  =   (uint8_t) ( (packetValue >> 21) & 0xFF );
   packet.shortAddr =   (uint8_t) ( (packetValue >> 29) & 0x3 );
   packet.addrMode  =   (uint8_t) ( (packetValue >> 31) & 0x1 );

   printf("crc      :%#x\n",packet.crc);
   printf("status   :%#x\n",packet.status);
   printf("payload  :%#x\n",packet.payload);
   printf("bat      :%#x\n",packet.bat);
   printf("sensor   :%#x\n",packet.sensor);
   printf("longAddr :%#x\n",packet.longAddr);
   printf("shortAddr:%#x\n",packet.shortAddr);
   printf("addrMode :%#x\n",packet.addrMode);

   printf("Size of struct is %I64u\n",sizeof(packet));
   //chiếm 4 byte bộ nhớ so vs 10 byte nếu sử dụng như vd1


   while(getchar() != '\n');
   getchar();

   return 0;

}

/*
 *
 * */









