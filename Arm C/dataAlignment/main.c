/*
 * Struct cơ bản và cách thực thi con trỏ với struct
 * Struct padding(bộ đệm): giải thích cách lưu các kiểu biến thành phần của 1 struct.
 *
 * Padding(bộ đệm): là những ô nhớ nằm giữa các kiểu biến thành phần mà ko đc sử dụng,
 * chúng sẽ đc lưu giá trị 0. (xem video 158 để hiểu chi tiết)
 * Padding sẽ xuất hiện nếu ta khai báo struct theo cách thông thường, lúc này
 * thì Struct sẽ sử dụng bộ nhớ dạng Aligned Storage(yêu cầu bộ nhớ đệm khi lưu
 * kiểu dữ liệu giữa các biến). Làm tăng kích thước sizeof của struct.
 * Hình dung: dạng Aligned Storage-các kiểu dữ liệu sẽ chiếm các ô nhớ chẵn(0,2,4), hoặc 1 ô nhớ(kiểu char).
 * Các ô nhớ lẻ sẽ bị bỏ qua, đây chính là các ô nhớ Padding.
 *
 * Ngoài ra còn 1 kiểu ngược lại là  Un-Aligned Storage(ko cần bộ nhớ đệm)
 * Các kiểu biến sẽ đc lưu ở các ô nhớ sát nhau. Cách này sẽ giúp tiết kiệm
 * ô nhớ, đồng thời giúp giảm kích thước sizeof của struct
 *
 * */
#include<stdint.h>
#include<stdio.h>

//Aligned Storage
struct DataSet
{
	char  data1;
	int   data2;
	char  data3;
	short data4;
};

//Un-Aligned Storage
//struct DataSet
//{
//	char  data1;
//	int   data2;
//	char  data3;
//	short data4;
//}__attribute__((packed));

int main(void)
{
	struct DataSet data;

	data.data1  = 0x11;
	data.data2  = 0XFFFFEEEE;
	data.data3  = 0x22;
	data.data4  = 0xABCD;

	uint8_t *ptr;

	ptr = (uint8_t*)&data;  //ép kiểu: đưa địa chỉ từ dạng kiểu struct DataSet về dạng ký tự char
	//ptr = &data;  //vd nếu ta ko thực hiện việc ép kiểu

	uint32_t totalSize = sizeof(struct DataSet);

	printf("Memory address       Content \n");
	printf("==============================\n");

	for(uint32_t i = 0 ; i < totalSize ; i++)
	{
		printf("%p ,   %X\n",ptr,*ptr);
		ptr++;
	}

	printf("Total memory consumed by this struct variable = %I64u\n",sizeof(struct DataSet));


   getchar();

}
