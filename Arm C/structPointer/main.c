/*
 * Structure and pointer
 * */

#include<stdint.h>
#include<stdio.h>

struct DataSet
{
	char data1;
	int  data2;
	char data3;
	short data4;
};

//Khai báo các hàm sẽ đc sử dụng trong chương trình
void displayMemberElements(struct DataSet data);
void displayMemberElements_Pointer(struct DataSet *data);

int main(void)
{

	struct DataSet data;

	data.data1 = 0x11;
	data.data2 = 0xFFFFEEEE;
	data.data3 = 0x22;
	data.data4 = 0xABCD;

	displayMemberElements(data);
	//Giải thích: khi ta đưa giá trị của biến data vào thì trình biên dịch sẽ tạo ra 1 biến data
	//khác nhưng có cùng giá trị vs biến data đc đưa vào và sử dụng nó trong quá trình xử lý ở hàm,
	//vì vậy việc xử lý tình toán thay đổi dữ liệu của biến data trong hàm ko có tác dụng vì ko làm
	//ảnh hưởng đến value của biến data thật sự.
	//Vì vậy, nếu ta cần xử lý tác vụ liên quan đến biến nhưng ko cần làm thay đổi value của biến thì
	//ta dùng phương pháp như trên. Tuy nhiến nếu cần thay đổi value của biến thì cần sử dụng đến
	//con trỏ

	//Vd1: sử dụng con trỏ và struct(ko sử dụng hàm)
	struct DataSet *pData;   //khai báo con trỏ biến Struct DataSet
	pData = &data;
	pData->data1 = 0x55;    //thay đổi giá trị trục tiếp của thành phần con của truct

	printf("After: data.data1 = %X\n",data.data1);  //kiểm tra giá trị biến data đã bị thay đổi data1 chưa
	displayMemberElements_Pointer(pData);  //sử dụng con trỏ để hiển thị giá trị biến

	getchar();

	return 0;

}

/*
 * Hàm hiển thị giá trị của 1 struct
 * Param: giá trị của 1 struct được đưa vào
 */
void displayMemberElements(struct DataSet data)
{
	printf("data1 = %X\n",data.data1);
	printf("data2 = %X\n",data.data2);
	printf("data3 = %X\n",data.data3);
	printf("data4 = %X\n",data.data4);

	//data: đây là biến ảo, có giá trị giống với biến data đc đưa vào khi gọi hàm
	//value biến data này thay đổi cũng ko làm thay đổi biến data đc đưa vào ở gọi hàm
}

/*
 * Hàm hiển thị giá trị của 1 struct
 * Param: giá trị của 1 địa chỉ struct được đưa vào
 */
void displayMemberElements_Pointer(struct DataSet *data)
{
	printf("data1 = %X\n",data->data1);
	printf("data2 = %X\n",data->data2);
	printf("data3 = %X\n",data->data3);
	printf("data4 = %X\n",data->data4);

	//data: đây là địa chỉ cửa biến data được đưa vào khi gọi hàm
	//value biến data này thay đổi cũng sẽ làm thay đổi biến data đc đưa vào ở gọi hàm
}
