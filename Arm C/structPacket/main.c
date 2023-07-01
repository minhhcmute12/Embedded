/*
 * Bài tập: Con trỏ và struct(vd1)
 * Viết chương trình giải mã thông tin gói 32 bit đã cho và in giá trị của các trường khác nhau.
 * Tạo một cấu trúc với các phần tử thành viên là các trường gói tin như hình bên dưới
 * */

#include<stdint.h>
#include<stdio.h>

//Cấu trúc của một gói thông tin packet 32bit ví dụ được nhận
struct Packet
{
	//bit 32
	uint8_t crc;   		//chiếm 2 bit
	uint8_t status; 	//chiếm 1 bit
	uint16_t payload; 	//chiếm 12 bit
	uint8_t bat;		//chiếm 3 bit
	uint8_t sensor;		//chiếm 3 bit
	uint8_t longAddr;	//chiếm 8 bit
	uint8_t shortAddr;	//chiếm 2 bit
	uint8_t addrMode;   //chiếm 1 bit
	//bit 0
};
//khi nhận thông tin thì việc phân tách sẽ đc tiến hành từ sau(bit 32) tiến về trước(bit 0)
//Điểm yếu: vì mỗi biến thành phần cửa struct là một kiểu khác nhau và sizeof khác nhau nên có thể có hiện
//tượng dư thừa ô nhớ ko sử dụng. vd: uint8_t là kiểu char chiếm 1 byte(8bit), tuy nhiến ta chỉ sử dụng
//1 bit hoặc 2 bit để lưu trữ dữ liệu, các bit còn lại ko đc sử dụng
//=> Để khắc phục thì ta sử dụng pp structure bit filed, trường bit cấu trúc

int main(void)
{

   uint32_t packetValue ;
   //Nhập giá trị thông tin packet ví dụ nhận đc
   printf("Enter the 32bit packet value:");
   scanf("%X",&packetValue);

   struct Packet packet;

   //tiến hành gán các giá trị cho các thành phần struct(sử dụng pp dịch bit)
   packet.crc 		=   (uint8_t) (packetValue & 0x3);
   packet.status 	=   (uint8_t) ( (packetValue >> 2) & 0x1 );
   packet.payload   =   (uint16_t) ( (packetValue >> 3) & 0xFFF );
   packet.bat       =   (uint8_t) ( (packetValue >> 15) & 0x7 );
   packet.sensor    =   (uint8_t) ( (packetValue >> 18) & 0x7 );
   packet.longAddr  =   (uint8_t) ( (packetValue >> 21) & 0xFF );
   packet.shortAddr =   (uint8_t) ( (packetValue >> 29) & 0x3 );
   packet.addrMode  =   (uint8_t) ( (packetValue >> 31) & 0x1 );

   printf("crc      :%#x\n",packet.crc);   //hiển thi value theo thập lục phân
   printf("status   :%#x\n",packet.status);
   printf("payload  :%#x\n",packet.payload);
   printf("bat      :%#x\n",packet.bat);
   printf("sensor   :%#x\n",packet.sensor);
   printf("longAddr :%#x\n",packet.longAddr);
   printf("shortAddr:%#x\n",packet.shortAddr);
   printf("addrMode :%#x\n",packet.addrMode);

   printf("Size of struct is %I64u\n",sizeof(packet)); //chiếm 10 byte bộ nhớ

   while(getchar() != '\n');
   getchar();

   return 0;

}











