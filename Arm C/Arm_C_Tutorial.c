/* Arm C Tutorial */

/**-I-- Hello World ---*/
/**-II-- Volatile ---*/
/**-III-- Array ---*/
/**-IV-- String ---*/
/**-V-- Pointer ---*/
/**-VI-- Struct ---*/
/**-VII-- Union ---*/
/**-VIII-- Const ---*/
/**-IX-- Macro ---*/
/**-X-- Bit Wise ---*/
/**-XI-- C to Arm ---*/

/**-I-- Hello World ---*/
int main(void)
{
    printf("Hello World\n");
    printf("Size of char data type is %u\n",sizeof(char));
    for(;;);
}

///Lesson 1: Sizeof()
int main(void)
{
	printf("Hello World\n");
	//Kích thước của các kiểu biến đc quyền chiếm dụng trong bộ nhớ
	printf("Size of char is %u\n",sizeof(char));
	printf("Size of short is %u\n",sizeof(short));
	printf("Size of int is %u\n",sizeof(int));
	printf("Size of long is %u\n",sizeof(long));
	printf("Size of long long is %u\n",sizeof(long long));
	printf("Size of double is %u\n",sizeof(double));
    /* Loop forever */
  	for(;;);
}

///Lesson 2: 
/*
 * Bài này hướng dẫn cơ bản về cách hoạt động của chương trình, sủ dụng ngôn ngữ Assembly
 * Ngoài ra ta có thể viết một số câu lệnh cơ bản để học các từ khóa Assembly sẽ sử dụng
 * Cách xem: vừa debug vừa xem bảng Assembly bên trong công cụ ShowView
*/
#include <stdio.h>
/* global variables */
int g_data1 = -4000;
int g_data2 = 200;
int result = 0;

int main(void)
{
	result = g_data1 + g_data2;

	printf("Result = %d\n",result);

	for(;;);
}

///Lesson 3:  Ascii 
#include<stdio.h>

int main(void)
{

    char c1,c2,c3,c4,c5,c6;
    printf("Enter any 6 characters of your choice \n");

    c1 = getchar();
    getchar(); 		//this getchar is used to clear the special character
    				//getchar này được sử dụng để xóa ký tự đặc biệt
    c2 = getchar();
    getchar();		//this getchar is used to clear the special character
    c3 = getchar();
    getchar();		//this getchar is used to clear the special character
    c4 = getchar();
    getchar();		//this getchar is used to clear the special character
    c5 = getchar();
    getchar();		//this getchar is used to clear the special character
    c6 = getchar();

    printf("\nASCII codes : %u,%u,%u,%u,%u,%u",c1,c2,c3,c4,c5,c6);

	printf("\nPress enter key to exit the application \n");
    while(getchar() != '\n')
    {
    	//just read the input buffer and do nothing - Chỉ cần đọc bộ đệm đầu vào và không làm gì cả
    }
    getchar();
}


///Lesson 4: Giới thiệu cách hoạt động lưu một số float, double trong nhúng
#include <stdio.h>

int main(void)
{
	float number = 45.78976834578;
	printf("Number = %0.2f\n",number);			//0.2f: lấy 2 số sau dấu thập phân
	printf("Number = %0.2e\n",number);			//e: hiển thị số theo cơ số 10, số e mũ n

	double chargeE = -1.60217662e-19;				//dien tich 1 eclectron
	printf("chargeE = %0.28lf\n",chargeE);
	printf("chargeE = %0.8le\n",chargeE);

	return 0;
}

///Lesson 5: Bài tập sử dụng vòng For để tạo ra khối tam giác
#include<stdio.h>
#include<stdint.h>

void wait_for_user_input(void);
int main(void)
{

	uint32_t height;
	printf("Enter height of pyramid:");
	scanf("%d",&height);


	for(uint32_t i = 1 ; i <= height  ; i++  )
	{
		for(uint32_t j = i; j > 0 ; j--)
		{
			printf("%d  ",j);
		}
		printf("\n");
	}

   wait_for_user_input();
}

///Lesson 6: Bài tập sử dụng Lệnh If tính thuế
#include<stdio.h>
#include<stdint.h>

void wait_for_user_input(void);

int main(void)
{

	uint64_t income;
	uint64_t tax;

	double temp_income;

	printf("Enter your total income:");
	scanf("%lf",&temp_income);

	if(temp_income < 0){
		printf("Income cannot be -ve\n");
		wait_for_user_input();
		return 0;
	}


	income = (uint64_t) temp_income;

	if(income <= 9525){
		tax = 0;
	}else if( (income > 9525) && (income <= 38700) ){
		tax = income * 0.12;
	}else if((income > 38700) && (income <= 82500)){
		tax = income * 0.22;
	}else if(income > 82500){
		tax = income * 0.32;
		tax = tax + 1000; //tax += 1000;
	}else{
		;//NOP
	}

	printf("Tax payable : $%I64u\n",tax );

    wait_for_user_input();
}

/**-II-- Volatile ---*/
/*
 * Sử dụng 'volatile' và 'const' trong khai báo biến
 * Ở trường hợp này ta sẽ cộng tất cả các thông báo chức của cả 'const' và 'volatile'
 *
 * vd1: uint8_t volatile *const pReg = (uint8_t*)0x40000000; ->cho phép thay đổi giá trị của địa
 * chỉ mà con trỏ đang giữ đồng thời thực hiện các lệnh có chứa biến này nếu sử dụng tối ưu hóa,
 * có nghĩa là thực hiện lệnh này trong khi chạy tối ưu hóa
 *
 * vd2: uint8_t const volatile *const pReg = (uint8_t*)0x40000000; -> Không cho phép thay đổi giá trị của
 * địa chỉ cũng như địa chỉ mà con trỏ đang chứa đồng thời thực hiện các lệnh có chứa biến này nếu sử dụng
 * tối ưu hóa
 *
 * Được sử dụng trong các trường hợp chỉ đc phép đọc(read-only) đối với các buffer hoặc địa chỉ dễ bị thay
 * đổi data một cách bất ngờ
 * */

//Trong vd này, thì tùy thuộc vào chức năng cấu hình địa chỉ mà ta sử dụng 'volatile' hay 'const'
#include<stdint.h>
int main(void)
{
	//Cấu hình địa chỉ cho vd1
	uint32_t volatile *const pClkCtrlReg =   (uint32_t*)0x40023830;
	uint32_t volatile *const pPortDModeReg = (uint32_t*)0x40020C00;
	uint32_t volatile *const pPortDOutReg =  (uint32_t*)0x40020C14;

	uint32_t volatile *const pPortAModeReg = (uint32_t*)0x40020000;
	uint32_t const volatile *const pPortAInReg =   (uint32_t*)0x40020010; //vd2


	//enable the clock for GPOID , GPIOA peripherals in the AHB1ENR
	*pClkCtrlReg |= ( 1 << 3);
	*pClkCtrlReg |= ( 1 << 0);

    //configuring PD12 as output
	*pPortDModeReg &= ~( 3 << 24);
	//make 24th bit position as 1 (SET)
	*pPortDModeReg |= ( 1 << 24);

	//Configure PA0 as input mode (GPIOA MODE REGISTER)
	*pPortAModeReg &= ~(3 << 0);

	while(1)
	{
		//read the pin status of the pin PA0 (GPIOA INPUT DATA REGISTER)
		uint8_t  pinStatus = (uint8_t)(*pPortAInReg & 0x1); //zero out all other bits except bit 0

		if(pinStatus){
			//turn on the LED
			*pPortDOutReg |= ( 1 << 12);
		}else{
			//turn off the LED
			*pPortDOutReg &= ~( 1 << 12);
		}
	}
}

/*
 * Định nghĩa: Volatile
 * Là một kiểu từ khóa hạn định, giới hạn(qualifier) để đặc tả các biến trong ngôn ngữ C.
 * Vai trò sử dụng giống với từ khóa 'const' nhưng cách sử dụng khác nhau.
 * Được dùng để hướng dẫn trình biên dịch không được gọi bất kỳ tối ưu nào trên hoạt động của biến
 * (Hoạt động của biến là hoạt động đọc và ghi)
 * Thông báo cho trình biên dịch rằng giá trị biến có thể thay đổi bất cứ lúc nào mà không phải từ
 * lập trình viên thay đổi. Vì vậy, trình biên dịch sẽ tắt tối ưu hóa các hoạt động read-write trên
 * các biến được khai báo bằng cách khai báo sử dụng từ khóa 'volotile'.
 * Từ khóa 'volotile' rất hữu ích trong mã hệ thống nhúng để giảm lỗi.
 *
 * Cách sử dụng:
 * Sử　dụng từ khóa 'volatile' khi biến có khả năng thay đổi bất ngờ về giá trị biến.
 * Vd: giá trị biến đọc chân pin PA0 thường thay đổi bất ngờ, người dùng có thể thay đổi
 * giá trị vào của biến đọc chân pin bất cứ lúc nào
 * Một số trường hợp cụ thể:
 * + Khi bạn làm việc với bộ nhớ được ánh xạ tới các thanh ghi ngoại vi của các bộ vi điều khiển.
 * + Khi bạn phải làm nhiều task vụ như truy cập các biến global trong một ứng dụng đa luồng RTOS.
 * + Khi một biến global được sử dụng để chia sẻ dữ liệu giữa main code và mã ISR.
 * ISR_Interrupt Service Routine(ngắt)
 *
 * Cách khai báo:
 * +Cách 1: volatile data (dùng cho biến)
 *     kiểu_biến volatile tên_biến;    hoặc    volatile kiểu_biến tên_biến
 * vd: uint8_t volatile my_data;       hoặc    volatile uint8_t my_data;
 *
 * +Cách 2: non-volatile pointer to volatile data (dùng cho con trỏ-hay sử dụng trong embedded)
 *     kiểu_biến volatiole *tên_con_trỏ
 * vd: uint8_t volatile *pStatusReg
 * Cách này thường được sử dụng trong trường hợp 1(cần ánh xạ đến các thanh ghi ngoại vi)
 * */
#include<stdint.h>

int main(void)
{
	//Bật tối ưu hóa tuần tự ở các mức để và debug, sau đó vào xem kết quả ở thanh disassembly

	//Vd1: khai báo các biến không sử dụng từ khóa 'volatile'
//	uint8_t data1;
//	uint8_t data2;
	//Ở vd1, khi ta đặt mức tối ưu hóa lên O1 thì trình biên dịch sẽ bỏ qua các và lệnh gán giá trị
	//Nguyên nhân vì trình biên dich nhận thấy đây là các biến chưa được sử dụng trong chương trình
	//Để biến được coi là sử dụng thì biến đó cần được sử dụng trong chương trình, là các thành phần
	//trong một phép toán, một đối số(argumnet) gọi hàm.

	//Vd2: khai báo các biến sử dụng từ khóa 'volotile'
	uint8_t volatile data1;
	uint8_t volatile data2;
	//Tất cả lệnh liên quan đến biến có khai báo 'volotile' sẽ đc thực hiện dù có thực hiện tối ưu hóa
	//mức bao nhiêu

	data1 = 50;

	data2 = data1;

	data2 = data1;

	for(;;);
}
 
/*
 * Sử dụng từ khóa 'volatile' với ISP_Interrupt Service Routine(ngắt)
 * */
#include <stdint.h>
#include <stdio.h>

//global shared variable between main code and ISR
uint8_t volatile g_button_pressed = 0;   //biến toàn cục xử lý ngắt

uint32_t g_button_press_count =0;

void button_init(void);

//Thiết lập các cấu hình địa chỉ các port sử dụng với từ khóa 'volatile'
uint32_t volatile *pEXTTIPendReg			= (uint32_t*) (0x40013C00 + 0x14);
uint32_t volatile *pClkCtrlReg				= (uint32_t*) (0x40023800 + 0x30);
uint32_t volatile *pClkCtrlRegApb2			= (uint32_t*) (0x40023800 + 0x44);
uint32_t volatile *pGPIOAModeReg 			= (uint32_t*) (0x40020000 + 0x00);
uint32_t volatile *pEXTIMaskReg 			= (uint32_t*) (0x40013C00 + 0x00);
uint32_t volatile *pEXTTIEdgeCtrlReg		= (uint32_t*) (0x40013C00 + 0x08);
uint32_t volatile *pNVICIRQEnReg 			= (uint32_t*) 0xE000E100;

int main(void)
{
	button_init();

	while(1)
	{
		//Disable interrupt  //tắt chế độ cho phép sử dụng ngắt thanh ghi pEXTIMaskReg
		*pEXTIMaskReg &= ~( 1 << 0);

		if(g_button_pressed){   //nếu có nút đc cấu hình ngắt được nhấn

			//Some delay until button debouncing gets over
			for(uint32_t volatile i=0;i<500000/2;i++);  //delay chương trình

			g_button_press_count++;  //tăng biến counter

			printf("Button is pressed : %lu\n",g_button_press_count); //in value biến counter

			g_button_pressed = 0;	//reset lại giá trị biến ngắt
		}

		//Enable interrupt //bật chế độ cho phép sử dụng ngắt thanh ghi pEXTIMaskReg
		*pEXTIMaskReg |= ( 1 << 0);
		//Phải bật/tắt thanh ghi pEXTIMaskReg vì lúc này vd muốn cho ta thấy cách hoạt động khi có
		//từ khóa 'volatile'. Nếu bật thanh ghi pEXTIMaskReg thì khi có nút đc nhấn thì trình xử lý
		//sẽ tự động nhảy vào hàm EXTI0_IRQHandler để xử lý rồi sau đó mới quay lại hàm if.
		//Trên thực tế, khi tạo ngắt chỉ cần viết hàm ngắt chứ ko cần viết hàm if, việc viết hàm if
		//ở vd này chỉ minh họa việc sử dụng 'volatile'.
	}

	//Kết quả: Khi chưa thiết lập từ khóa 'volatile' cho các địa chỉ thanh ghi thì khi chạy với mức
	//tối ưu hóa càng cao thì việc nhấn nút ngắt càng xử lý chậm và không chính xác. Xuất hiện tình trạng
	//nhiệm vụ ngắt không đc thực hiện nhưng giá trị biến g_button_press_count vẫn tăng lên. Nguyên nhân là
	//tối ưu hóa trình biên dịch cho rằng biến ngắt là một biến không đc sử dụng trong chương trình, và bỏ
	//qua ko xử lý trong quá trình tối ưu hóa.
	//còn biến g_button_press_count là một biến có sử dụng do nó nằm trong một biểu thức tính toán.

	//Và sau khi thêm từ khóa 'volatile' vào thì dù ở mức tối ưu hóa nào thì trình biên dịch
	//đều sẽ thực hiện lệnh ngắt chính xác do nó phải thực hiện lệnh có biến g_button_pressed(đang đc
	//khai báo dùng 'volatile')
}

//Cấu hình địa chỉ thanh ghi cho nút nhấn
void button_init(void)
{

  *pClkCtrlReg |= ( 1 << 0);

  *pClkCtrlRegApb2 |= ( 1 << 14);

  *pEXTTIEdgeCtrlReg |= ( 1 << 0);

  *pEXTIMaskReg |= ( 1 << 0);

  *pNVICIRQEnReg |= ( 1 << 6);

}

/* This is button interrupt handler-hàm xử lý nút nhấn ngắt_interrupt*/
void EXTI0_IRQHandler(void)
{
	//Make this flag SET . if button pressed
  g_button_pressed = 1;

  *pEXTTIPendReg |= ( 1 << 0);
}

/**-III-- Array ---*/
/*
 * Mảng(array) trong lập trình nhúng.
 *
 * Lưu ý 1: mảng thực chất là một con trỏ
 *
 * Lưu ý 2: Sử dụng biến trong khai báo mảng
 * vd: int len = 10; uint8_t someData[len];
 * //chỉ đc phép khai báo cách từ C99 trở đi,ko đc phép khi nằm trong C90 hoặc standard
 *
 * Cách chỉnh sửa vesion C(nếu sử dụng C90 hoặc C89)
 * B1: Properties -> Settings -> Tool Settings -> GCC C Compiler -> Dialect -> Language standard -> Select version
 * B2: ...-> GCC C Compiler -> Warnings -> Pedantic (check)
 *
 * -> Mặc định dùng bản C do STM32Cube IDE cung cấp(ko chọn version)
 * */
#include<stdio.h>
#include<stdint.h>

int main(void)
{
	//Vd0:
	uint32_t studentAge[100];	//khai báo 1 mảng
	printf("Size of an array = %u\n",sizeof(studentAge));   //in số byte mà mảng đang chiếm
	printf("Base a address of the array = %p\n",studentAge);//địa chỉ của mảng, cũng là vị trí địa chỉ
	//phần tử đầu tiên của mảng   -> mảng là 1 con trỏ
	//studentAge+1 :  địa chỉ phần tử thứ 2 trong mảng

	//Vd1: Set value ban đầu và đọc giá trị phần tử nằm trong mảng
	//Khai báo một mảng và set value cho các index của mảng
	uint8_t someData[10] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

	printf("contents of this array\n");

	for(uint32_t i = 0 ; i < 10 ; i++)
	{
		printf("%x\t",someData[i]);  //printf("%x\t",*(someData+i));
	}

	printf("\n");

	//Vd2: Set value trong quá trình duyệt array
	for(uint32_t i = 0 ; i < 10 ; i++)
	{
		someData[i] = 0x33;       //*(someData+i) = 0x33
		printf("%x\t",someData[i]);
	}
	return 0;
}

///Ví dụ 1: Viết hàm xử lý array
#include<stdio.h>
#include<stdint.h>
void array_display(uint8_t const *const pArray, uint32_t nItmes);

int main(void)
{
	//Khai báo và đồng thời set value ban đầu cho các phân tử mảng
	uint8_t someData[10] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};//0xff=0

	//Vd2: Set value cho phần tử mảng với value tằng dần từ 0->9
	for(uint32_t i = 0 ; i < 10 ; i++)
	{
		someData[i] = i;
	}

	//Độ dài của mảng và độ dài của 1 phần tử của mảng
	printf("%I64u \t %I64u \n",sizeof(someData),sizeof(uint8_t)); //10 và 1

	uint32_t nItems = sizeof(someData) / sizeof(uint8_t);

	array_display(someData,nItems);    //vd1: Hiển thị toàn bộ
	array_display(&someData[2],nItems-2);  //vd3: hiển thị từ vị trí và số lần lặp mong muốn

	return 0;
}

/**
 * Hàm xử lý việc hiển thị mảng
 * const *const: ko cho phép thay đổi giá trị và địa chỉ của biến mảng đưa vào
 */
void array_display(uint8_t const *const pArray, uint32_t nItmes)
{
	//lets display the contents of the received array
	for(uint32_t i = 0 ; i < nItmes ; i++)
	{
		printf("%x\t",pArray[i]);
	}

}

///Ví dụ 2: 
/*
 * Bài tập mảng: Viết chương trình nhập vào một 2 mảng(cho phép người dùng nhập số phần tử).
 * Sau đó tiến hành hoán vị, đổi giá trị các phần tử 2 mảng cho nhau.
 * */
#include<stdio.h>
#include<stdint.h>

//Khai báo các hàm sẽ đc viết phía sau hàm main
void wait_for_user_input(void);
void display_array(int32_t *pArray1,uint32_t nitem);
void swap_arrays(int32_t *array1, int32_t *array2, uint32_t nitem1, uint32_t nitem2);

int main(void)
{
	//Nhập số phần tử cho 2 mảng
    int32_t nItem1, nItem2;
    printf("Array swapping program\n");
    printf("Enter no of elements of Array-1 and Array-2:");
    scanf("%d %d",&nItem1,&nItem2);

    //Kiểm tra số phần tử đc nhập vào ko phải là số âm
    if((nItem1 < 0) || (nItem2 < 0))
    {
    	printf("Number of elements cannot be negative\n");
    	wait_for_user_input();
    	return 0;
    }

    //Khởi tạo mảng, lưu ý việc khởi tạo cách này phải từ bản C99 trở lên
    //Ở IDE Stm32 thì mặc định sẽ dùng bản C99
	//Nếu không có C99 thì cần sử dụng bộ nhớ động để tạo mảng
    int32_t array1[nItem1];
    int32_t array2[nItem2];

    //Nhập value cho các thành phần của mảng
    for(uint32_t i = 0; i < nItem1 ; i++ )
    {
    	printf("Enter %d element of array1:",i);
    	scanf("%d",&array1[i]);  //hoặc *(array+1)
    }

    for(uint32_t i = 0; i < nItem2 ; i++ )
    {
    	printf("Enter %d element of array2:",i);
    	scanf("%d",&array2[i]);
    }

	printf("Contents of arrays before swap \n");

	display_array(array1,nItem1);

	printf("\n");

	display_array(array2,nItem2);

	printf("\n");

	printf("Contents of arrays after swap\n");

	swap_arrays(array1,array2,nItem1,nItem2);

	display_array(array1,nItem1);

	printf("\n");

	display_array(array2,nItem2);

	printf("\n");

	wait_for_user_input();

	return 0;
}
//Lưu ý: nếu sử dụng param là : int32_t pArray[] thì chương trình C sẽ tạo ra một biến ảo có value
//đc copy từ mảng đưa vào, việc hoán đổi giá trị sẽ ko các tác dụng với mảng chính đc đưa vào.
//Nên ở đây ta viết int32_t *pArray là muốn thay đổi luôn mảng đc đưa vào, đây chính là lý do vì
//sao con trỏ lại quan trọng.

/**
 * Hàm đổi vị trí các giá trị của 2 mảng
 * array1,array2: mảng hoán đổi
 * nitem1,nitem2 : số phần tử của mảng 1 và mảng 2
 */
void swap_arrays(int32_t *array1, int32_t *array2, uint32_t nitem1, uint32_t nitem2)
{
	//Sử dụng toán tử 3 ngôi để tìm kiếm mảng nào có độ dài lớn hơn làm số mốc chạy vòng lặp
   uint32_t len = nitem1 < nitem2 ? nitem1 : nitem2;

   for(uint32_t i = 0 ; i < len ; i++)
   {
	   //Sử dụng pp sử dụng biến tạm
	   int32_t temp = array1[i];
	   array1[i] = array2[i];
	   array2[i] = temp;
   }
}

/**
 * Hàm hiển thị các value của mảng
 * pArray1: mảng
 * nitem : số phần tử muốn hiển thị
 */
void display_array(int32_t *pArray1,uint32_t nitem)
{
	for(uint32_t i = 0 ; i < nitem ; i++)
	{
		printf("%4d  ",pArray1[i]);
	}
	//Vì hàm chỉ có chức năng hiển thị nên có thể khai báo param là : int32_t pArray1[]
}

/**
 * Hàm thiết lập chờ nhấn 1 nút để thoát chương trình
 */
void wait_for_user_input(void)
{

	printf("Press enter key to exit this application");

    while(getchar() != '\n')
    {
    	//just read the input buffer and do nothing
    }
    getchar();

}

/**
 * Một số kiến thức liên quan đến String_Chuỗi (S30_V186)
 *
 * Phân biệt giữa con trỏ Chuỗi(literal) và Chuỗi thông thường
 * + Chuỗi : char msg1[] = "hello"  -> Value này sẽ đc lưu trong Stack of RAM
 * Khi thay đổi giá trị: msg1[0] = 's';   -> cho phép
 *
 * + Con trỏ Chuỗi: char *msg2[] = "hello"  -> Value này sẽ đc lưu trong ROM(cụ thể là bộ nhớ Flash)
 * Khi thay đổi giá trị: msg2[0] = 's';   -> KHÔNG cho phép thay đổi value trong bộ nhớ ROM, chuỗi hằng
 */

#include <stdint.h>		//thư viện định dạng khai báo biến

int main(void)
{
	char msg1[] = "Hello how are you";
	char *pmsg2[] = "fastbitlab.com";  //tương đương một chuỗi hằng

	printf("Message is : %s\n", msg1);
	printf("Message is : %s\n", pmsg2);

	printf("Address of 'msg1' variable : %p\n", &msg1);   //Value1_msg1
	printf("Value of 'msg1' is : %p\n", msg1);				//Value2_msg1

	printf("Address of 'msg1' variable : %p\n", &pmsg2);
	printf("Value of 'msg1' is : %p\n", pmsg2);

	msg1[0] = 'b';
	//pmsg2[0] = 'b';   //không cho phép thay đổi chuỗi hằng

	//->Tùy vào vi xử lý mà value1==value2==địa_chỉ_x

	for(;;);
}

/**-IV-- String ---*/
/**
 * Trong ngôn ngữ lập trình C, fflush() là một hàm dùng để xóa bộ đệm của một luồng dữ liệu (stream),
 * giúp đảm bảo rằng dữ liệu đã được ghi vào luồng sẽ được đưa ra ngoài ngay lập tức.
 * Trong trường hợp đặc biệt khi ta muốn đảm bảo rằng dữ liệu được hiển thị trên màn hình ngay lập tức,
 * thì hàm fflush(stdout) có thể được sử dụng để đẩy bộ đệm của stdout (luồng đầu ra tiêu chuẩn) ra ngoài.
 * Ví dụ, khi bạn ghi dữ liệu vào stdout bằng hàm printf(), các ký tự trong chuỗi định dạng printf()
 * sẽ được lưu trong bộ đệm và chỉ được hiển thị trên màn hình khi bộ đệm đầy hoặc khi một dòng mới
 * được in ra. Tuy nhiên, nếu bạn muốn hiển thị ký tự ngay lập tức thay vì chờ bộ đệm đầy, bạn có
 * thể sử dụng fflush(stdout) để đẩy bộ đệm của stdout ra ngoài.
 * Lưu ý rằng việc sử dụng fflush() không được khuyến khích trong các ứng dụng thời gian thực hoặc
 * trong các ứng dụng đòi hỏi hiệu suất cao, vì việc xóa bộ đệm sẽ làm giảm hiệu suất của ứng dụng.
 */

/**
 * Điều kiện nhập chuỗi
 * vd: char name[30]; scanf("%[^s]s",name);
 * Yêu cầu nhập chuỗi đến khi nào có ký tự 's' xuất hiện
 * vd2: char name[30]; scanf("%[^\n]s",name); -> Nhập chuỗi đến khi có ký tự xuống dòng
 */

/**-V-- Pointer ---*/
///Lesson 1: Giới thiệu
#include <stdio.h>
/*
	1. Create a char type variable and initialize it to value 100
	2. Print the address of the above variable.
	3. Create a pointer variable and store the address of the above variable
	4. Perform read operation on the pointer variable to fetch 1 byte of data from the pointer
	5. Print the data obtained from the read operation on the pointer.
	6. Perform write operation on the pointer to store the value 65
	7. Print the value of the variable defined in step 1

	1. Tạo một biến kiểu char và khởi tạo nó với giá trị 100
	2. In ra địa chỉ của biến trên.
	3. Tạo biến con trỏ và lưu địa chỉ của biến trên
	4. Thực hiện thao tác đọc trên biến con trỏ để lấy 1 byte dữ liệu từ con trỏ
	5. In dữ liệu thu được từ thao tác đọc trên con trỏ.
	6. Thực hiện thao tác ghi trên con trỏ để lưu giá trị 65
	7. In ra giá trị của biến đã xác định ở bước 1
*/
int main(void)
{
	char data = 100;
	printf("Value of data is: %d\n",data);
	printf("Address of the variable data is: %p\n",&data);			//%p: địa chỉ mà con trỏ đang chứa

	//Create a pointer variable and store the address of the above variable
	char* pAddress = &data;

	char value = *pAddress;
	printf("Value of value is: %d\n",value);

	*pAddress = 65;
	printf("Value of data is: %d\n",data);

	return 0;
}

///Lesson 2: Bài tập ép data về các kiểu dữ liệu khác nhau dùng Pointer
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

///Ví dụ 2: 
#include <stdio.h>

long long int g_data = 0xFFFEABCD11112345; //FFFE byte6-7 - ABCD byte4-5 - 1111 byte2-3 - 2345 byte0-1

int main(void)
{
/*
	char *pAddress = (char*)&g_data;
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);  //gia tri 0x45 hex

	pAddress = pAddress + 5;
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);
*/

	/*short *pAddress = (short*)&g_data;
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);  //gia tri 0x45 hex

	pAddress = pAddress + 2;
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);
*/

	int *pAddress = (int*)&g_data;
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);  //gia tri 0x45 hex

	pAddress = pAddress + 1;   //trong con tro kiểu int +1 di chuyen 4byte(4 địa chỉ)
	printf("Value at address %p \n",pAddress);
	printf("Value at address %p is: %x\n",pAddress,*pAddress);

	//Giải thích việc + 1 mà di chuyển 4 byte
	//Vì con trỏ đc khai báo là kiểu int nên nó cần 4 byte để lưu dữ liệu
	//mà 1 địa chỉ sẽ lưu 1 byte, nên 4 địa chỉ sẽ chiếm 4 byte
	//khi +1 con trỏ nghĩa là ta cần đến con trỏ tiếp theo nên ta cần
	//di chuyển qua 4 byte mà con trỏ hiện tại đang chiếm để lưu biến int.
	//Vì vậy tùy thuộc vào kiểu dữ liệu con trỏ mà số địa chỉ chạy sẽ khác nhau.

	//Kết quả việc +1 con trỏ:
//	Value at address 0000000000403010
//	Value at address 0000000000403010 is: 11112345
//	Value at address 0000000000403014
//	Value at address 0000000000403014 is: fffeabcd
	//Ta thấy rằng con trỏ x sẽ nằm ở địa chỉ 0403010
	//và con trỏ x+1 sẽ nằm ở vị trí 0403014, cách 4byte(địa chỉ) so vs con trỏ chuẩn x

	//Nếu con trỏ x là 1 biến kiểu char thì vị trí con trỏ x+1 chỉ cách 1 byte(địa chỉ)
	//vì biến kiểu char chỉ có 1 byte lưu dữ liệu

	//=> vì vậy tùy vào biến con trỏ thuộc kiểu dữ liệu nào thì vị trí x+1 sẽ khác nhau
}

/**-VII-- Struct ---*/
///Lesson 1: Giới thiệu
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

///Lesson 2: một số ví dụ
//Ví dụ 1: 
#include<stdio.h>
struct DataSet
{
	char  data1 ;
	int   data2 ;
	char  data3 ;
	short data4 ;
}__attribute__((packed)); //Un-Aligned Storage: ko dùng bộ nhớ đệm khi sử dụng struct
//=> Việc dùng Un-Aligned Storage có thể giảm kích thước file nhưng có thể làm ảnh hưởng đến
//hiệu suất của ứng dùng do trình biên dịch sẽ cần nhiều lệnh hơn để đọc ghi các giá trị
//khi ko dùng bộ đệm.


struct DataSet data ; //this consumes 12 bytes in memory(RAM)
//C1: khai báo typedef trực tiếp trong struct
//typedef struct
//{
//	unsigned int carNumber;
//	uint32_t carPrice;
//	uint16_t carMaxSpeed;
//	float carWeight;
//}CarModel_t;    //thêm _t: để phân biệt struct ko sử dụng typedef
//--> CarModel_t carBWM,carFord;


//C2: khai báo typedef sau khi đã tạo struct
//struct CarModel
//{
//	unsigned int carNumber;
//	uint32_t carPrice;
//	uint16_t carMaxSpeed;
//	float carWeight;
//};
//typedef struct CarModel CarModel;
//--> CarModel carBWM,carFord;

//Lưu ý 1: không được phép khai báo thành phần con là struct trùng với struct mẹ
//vd:
//struct CarModel
//{
//	unsigned int carNumber;
//	uint32_t carPrice;
//	uint16_t carMaxSpeed;
//	float carWeight;
//  struct CarModel carBWM;   //không được phép
//};

//Lưu ý 2: được phép khai báo thành phần con là con trỏ struct với struct mẹ
//vd:
//struct CarModel
//{
//	unsigned int carNumber;
//	uint32_t carPrice;
//	uint16_t carMaxSpeed;
//	float carWeight;
//  struct CarModel *carBWM;   //được phép
//};

//Lưu ý 3: được phép khai báo thành phần con là một struct khác với struct mẹ.
//thường thì ta nên tạo 1 typedef của struct con đó sau đó mới khai báo ở struct mẹ
//vd:
//struct CarModel
//{
//	unsigned int carNumber;
//	uint32_t carPrice;
//	uint16_t carMaxSpeed;
//	float carWeight;
//  struct
//	{
//		char data5;
//		int data6;
//	}moreData;
//};

int main(void)
{

	data.data1 = 0xAA;
	data.data2 = 0xFFFFEEEE;
	data.data3 = 0x55;
	data.data4 = 0xA5A5;

	printf("data.data1 = %d\n",data.data1);
	printf("data.data2 = %d\n",data.data2);
	printf("data.data3 = %d\n",data.data3);
	printf("data.data4 = %d\n",data.data4);

	for(;;);
}

//Ví dụ 2:
#include<stdint.h>
#include<stdio.h>

struct Packet
{
	uint8_t crc;
	uint8_t status;
	uint16_t payload;
	uint8_t bat;
	uint8_t sensor;
	uint8_t longAddr;
	uint8_t shortAddr;
	uint8_t addrMode;
};

int main(void)
{
   uint32_t packetValue ;
   printf("Enter the 32bit packet value:");
   scanf("%X",&packetValue);

   struct Packet packet;

   packet.crc 		=   (uint8_t) (packetValue & 0x3); // bit dau tien 0, 0x03=11 day 2 bit len 1
   packet.status 	=   (uint8_t) ( (packetValue >> 2) & 0x1 ); //dich toi  bit thu2 và Ox1=1 day bit do len 1
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

   while(getchar() != '\n');
   getchar();

   return 0;
}

//Ví dụ 3:
#include<stdint.h>
#include<stdio.h>

struct carModel
{
	uint8_t carSpeed;
	uint32_t carPrice;
};

int main(void)
{
	struct carModel  carBMW = {0};
	printf("Sizeof of struct carModel is %I64u byte\n",sizeof(carBMW));
	printf("Sizeof of struct carModel is %I64u byte\n",sizeof(struct carModel));

    getchar();

	return 0;
}

///Lesson 3: Bài tập quản lý sinh viên
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Định nghĩa bản ghi sinh viên
typedef struct
{
    int 	rollNumber;		//ma so ca nhan
    char    name[100];		//ten
    char    branch[50];		//chi nhanh
    int 	dob;			//ngay thang nam sinh
    int 	semister;		//vi tri

}STUDENT_INFO_t;

// Bản ghi tối đa mà chương trình này có thể lưu trữ
int max_record=10;

// Mảng bản ghi toàn cục
STUDENT_INFO_t students[10];

//function prototypes of this program
void display_menu(void);
int read_menu_code(void);
void decode_menu_code(int8_t menu_code);
void display_all_records(STUDENT_INFO_t *record, int8_t max_record);
int check_roll_number(int roll_number, STUDENT_INFO_t *record, int8_t max_record);
int add_new_record(STUDENT_INFO_t *record, int8_t max_record);
int delete_record(STUDENT_INFO_t *record, int8_t max_record);

int main()
{
	int8_t menu_code;
	int8_t app_continue = 1;

	printf("Chuong trinh quan ly du lieu sinh vien\n");
	while(app_continue)
	{
		//1. Danh sach lua chon tinh nang
		display_menu();

		//2.Doc ma menu da chon
		menu_code = read_menu_code();

		if(menu_code)
		{
			//Decode the menu code
			decode_menu_code(menu_code);
		}else{
			//end the application if menu_code = 0;
			app_continue = 0;
			printf("Exiting application\n");
		}
	}

	return 0;
}

void display_menu()
{
	printf("Display all records  -->1\n");
	printf("Add new record       -->2\n");
	printf("Delete a record      -->3\n");
	printf("Exit application     -->0\n");
	printf("Enter your option here:");
}

/*
 * This function reads the menu code entered by the user and return the code
 */
int read_menu_code(void)
{
	int input;
	scanf("%d",&input);
	return input;
}

//This function decodes the menu
void decode_menu_code(int8_t menu_code)
{
    int8_t ret;

    switch(menu_code)
    {
        case 1:
        	printf("Displaying all students record\n");
        	display_all_records(students,max_record);
        	break;
        case 2:
			printf("Add a new record\n");
			ret = add_new_record(students,max_record);
			!ret ? printf("Record add unsuccessful\n") : printf("Record added successfully\n");
			break;
        case 3:
			printf("Delete a record\n");
			ret = delete_record(students,max_record);
			ret ? printf("Record deleted successfully\n") : printf("Record not found\n");
			break;
        default:
			printf("Invalid input\n");
    }
}

/*
 * This function displays all the records so far entered by the user
 * in_param[1] : base address of the records
 * in_param[2] : maximum record number maintained by this program
 * Note: if no records found, then this fucntion should print "No records found'
 */
void display_all_records(STUDENT_INFO_t *record, int8_t max_record)
{
	int record_found = 0; //tim co trong danh sach khong neu khong tra ve 0
	for(int i = 0 ; i < max_record ; i++ )
	{
		if(record[i].rollNumber)
		{
			record_found = 1;
			//display the record
			printf("***********\n");
			printf("rollNumber : %u\n",record[i].rollNumber);
			printf("studentSemister   : %u\n",record[i].semister);
			printf("studentDOB        : %u\n",record[i].dob);
			printf("studentBranch     : %s\n",record[i].branch);
			printf("studentName       : %s\n",record[i].name);
			printf("***********\n");
		}
	}

	if(!record_found)
	{
		printf("No records found\n");
	}
}
/*
 * This function matches the given roll_numer with the roll numbers of the records by traversing through record node one by one
 * in_param[1]: given roll number
 * in_param[2]: base address of the records
 * in_param[3]: max record number of this program
 * return : 0 for no_match , 1 for match
 */
int check_roll_number(int roll_number, STUDENT_INFO_t *record, int8_t max_record)
{
	int is_exist = 0;
	for(int i = 0 ; i < max_record ; i++)
	{
	    if ( record[i].rollNumber == roll_number)
	    {
	         is_exist = 1;
	         break;
	    }
	}
	return is_exist;
}

/*
 * This function adds a new record by asking various information from the user.
 * before adding a record , function has to identify an empty node
 * Assume a node is empty , if roll number field of node is zero
 * in_param[1]: base address of the records
 * in_param[2]: max record number of this program
 * return values : 0 if add is unsuccessful, 1 if add is successful
 * Note1 : This function should also print "roll number already exist" if roll number entered is already exist in the record
 * Note2 : This function should also print "No space available", if all nodes of the array are occupied
 */
int add_new_record(STUDENT_INFO_t *record, int8_t max_record)
{
	int add_status=0;
	int is_exist=0;
	int roll_number;
	int i;

	for( i = 0 ; i < max_record ; i++)
	{
		if( ! record[i].rollNumber)
	    {
			//add the record
	        printf("Enter the rollNumber : ");
	        scanf("%d",&roll_number);
	        is_exist = check_roll_number(roll_number,students,max_record);
	        if(! is_exist)
	        {
	        	add_status = 1; //adding new record
	            record[i].rollNumber = roll_number;
	            printf("Enter the studentSemister   : ");
	            scanf("%d",&record[i].semister);
	            printf("Enter the studentDOB        : ");
	            scanf("%d",&record[i].dob);
	            getchar();
	            printf("Enter the studentBranch     : ");
	            scanf("%50[^\n]s",record[i].branch);
	            getchar();
	            printf("Enter the studentName       : ");
	            scanf("%50[^\n]s",record[i].name);
	        }
	        else{
	            printf("roll number already exist\n");
	        }
	        break;
	     }
	}
	if(i == max_record)
		printf("No space available\n");

	return add_status;
}

/*
 * Delete a record.
 * first ask the user to enter the roll number of the student
 * delete a node against that roll number
 * in_param[1]: base address of the records
 * in_param[2]: max record number of this program
 * return values : 0 if delete unsuccessful, 1 if delete successful
 *
 */
int delete_record(STUDENT_INFO_t *record, int8_t max_record)
{
	int8_t delete_flag = 0;
	int roll_number;

	printf("Enter the roll number of the student:");
	scanf("%d",&roll_number);

	for(int i = 0 ; i < max_record ; i++)
	{
		if( roll_number == record[i].rollNumber)
	    {
			delete_flag = 1;
	        memset(&record[i],0,sizeof(STUDENT_INFO_t));
	        break;
	    }
	}
	return delete_flag;
}

///Lesson 4:  Cấu trúc trường bit
/* Vd1:
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

/*
 * Vd2:
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
 * để có thể lấy về đúng số bit cần khi tách thì ta sẽ lựa chọn kiểu biến đáp ứng số bit thông
 * tin cần tách và gán số bit cần chiếm cho mỗi thành phần
 *
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

///Lesson 5: Structure and pointer
#include<stdint.h>
#include<stdio.h>

/**
 * Struct
 */
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

	struct DataSet data;   //khai báo biến sử dụng Struct

	//Lưu data vào biến khai báo sử dụng struct
	data.data1 = 0x11;
	data.data2 = 0xFFFFEEEE;
	data.data3 = 0x22;
	data.data4 = 0xABCD;

	displayMemberElements(data);  //Vd1
	//Giải thích: khi ta đưa giá trị của biến data vào thì trình biên dịch sẽ tạo ra 1 biến data
	//khác nhưng có cùng giá trị vs biến data đc đưa vào và sử dụng nó trong quá trình xử lý ở hàm,
	//vì vậy việc xử lý tình toán thay đổi dữ liệu của biến data trong hàm ko có tác dụng vì ko làm
	//ảnh hưởng đến value của biến data thật sự.
	//Vì vậy, nếu ta cần xử lý tác vụ liên quan đến biến nhưng ko cần làm thay đổi value của biến thì
	//ta dùng phương pháp như trên. Tuy nhiến nếu cần thay đổi value của biến thì cần sử dụng đến
	//con trỏ

	//Vd2: sử dụng con trỏ và struct(ko sử dụng hàm)
	struct DataSet *pData;   //khai báo con trỏ biến Struct DataSet
	pData = &data;			//con trỏ pData sẽ lưu địa chỉ của biến mà sử dụng Struct ở trên
	pData->data1 = 0x55;    //thay đổi trực tiếp giá trị của thành phần con của truct

	printf("After: data.data1 = %X\n",data.data1);  //kiểm tra giá trị biến data đã bị thay đổi data1 chưa
	displayMemberElements_Pointer(pData);  //sử dụng con trỏ để hiển thị giá trị biến

	getchar();
	return 0;
}

/*
 * Hàm hiển thị giá trị của 1 struct Vd1
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
 * Hàm hiển thị giá trị của 1 struct Vd2 sử dụng con trỏ
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

/**-VII-- Union ---*/
///Lesson 1: Giới thiệu
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

///Lesson 2: Ứng dụng Union trong hệ thống nhúng
/*
 * Applicability of unions in embedded system code
 1.Bit extraction
 2.Storing mutually exclusive data thus saving memory
 *
 * Khả năng ứng dụng của union trong mã hệ thống nhúng
 1. Khai thác(phân loại, chiết xuất) bit
 2. Lưu trữ dữ liệu loại trừ(ghi đè) lẫn nhau do đó tiết kiệm bộ nhớ
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

   union Packet packet;  //khai báo tham chiếu một Union
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
  //pp thao tác bit mà ta vẫn sẽ chia tách đc các thông tin dữ liệu theo yêu cầu

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

/**-VIII-- Const ---*/
/*
 * Giá trị "const"
 *
 * Lưu ý: 'const' không có nghĩa là giá trị không bao giờ đổi, mà nó là trình biên dịch cảnh báo
 * cho bạn rằng ai đó đang cố gắng thay đổi giá trị của biến đó bằng tên của nó.
 *
 * Các biến 'const' ở dạng biến 'local' sẽ được lưu trữ trong bộ nhớ RAM, và chức năng duy nhất của
 * chúng là chỉ đọc 'read-only'.
 * Các biến 'const' ở dạng biến 'global' sẽ được lưu trữ trong bộ nhớ ROM hoặc FLASH.
 *
 * Trong STM32 thì, tất cả các biến global 'const' đều sẽ được lưu trong bộ nhớ FLASH. Vì vậy khi bạn
 * cố gắng thay đổi biến const bằng cách thay đổi địa chỉ của nó sẽ KHÔNG có tác dụng . Bởi vì bộ nhớ
 * FLASH là bộ nhớ chống ghi(read-only)
 *
 * Các cách khai báo biến 'const'
 * C1: Cách bình thường
 * 	   kiểu_biến const tên_biến = giá trị; -> vd: uint8_t const data = 10;
 *
 * C2: Sử dụng con trỏ
 * 	   kiểu_biến const *tên_con_trỏ = (kiểu_biến *) địa_chỉ_cần lưu_giá_trị_const
 * 	   -> vd: uint8_t const *pData = (uint8_t*)0x40000000;
 * 	   Các cách gán cho phép: pData = (uint8_t*)0x50000000 hoặc pData = (uint8_t*)0x60000000
 * 	   Các cách gán KHÔNG cho phép : *pData = 50;
 * 	   => Có nghĩa là cho phép thay đổi địa chỉ mà con trỏ lưu thành địa chỉ khác nhưng KHÔNG cho phép
 * 	   thay đổi giá trị của địa chỉ mà con trỏ đang lưu.
 *
 * C3: Dạng con trỏ đặc biệt 1
 * 	   kiểu_biến *const tên_biến = (kiểu_biến *) địa_chỉ_cần lưu_giá_trị_const
 * 	   -> vd: uint8_t *const pData = (uint8_t*)0x40000000;
 * 	   Đây là trường hợp con trỏ dạng 'const' nhưng cho phép thay đổi dữ liệu
 * 	   Các cách gán cho phép: *pData = 50; hoặc *pData = 10;
 * 	   Các cách gán KHÔNG cho phép : pData = (uint8_t*)0x50000000 hoặc pData = (uint8_t*)0x60000000
 * 	   => Có nghĩa là cho phép thay đổi giá trị của địa chỉ mà con trỏ lưu nhưng KHÔNG cho phép
 * 	   thay đổi địa chỉ mà con trỏ đang lưu thành địa chỉ khác.
 * 	   =>Ta có thể gọi đây là con trỏ chỉ cho phép đọc(read-only) địa chỉ mà nó đang lưu mà
 * 	   KHÔNG cho phép thay đổi địa chỉ mà nó đang lưu.
 *
 * C4 Dạng con trỏ đặc biệt 2 (tổng hợp từ C2+C3)
 * 	   kiểu_biến const *const tên_biến = (kiểu_biến *) địa_chỉ_cần lưu_giá_trị_const
 * 	   -> vd: uint8_t const *const pData = (uint8_t*)0x40000000;
 * 	   Chỉ được phép đọc(read-only) địa chỉ mà con trỏ đang lưu và giá trị của địa chỉ mà
 * 	   con trỏ đang lưu. KHÔNG cho phép thay đổi bất cứ cái gì liên quan đến giá trị hoặc địa chỉ
 * 	   mà con trỏ đang lưu.
 *
 * *Một số lưu ý khác: (điều 4 là quan trọng nhất)
 * 1.Sử dụng 'const' là thêm một số an toàn trong quá trình code, có nghĩa là trình biên dịch sẽ thông
 * báo đến bạn khi bạn cố gắng thay đổi giá trị của biến const.
 * 2.Biến 'const' vì là không thay đổi nên nó chỉ có một trạng thái trong suốt chương trình. Bạn cần theo
 * dõi các trạng thái khác nhau của nó.
 * 3.Việc sử dụng 'const' sẽ giúp cải thiện khả năng đọc dữ liệu
 * 4(**).Nên sử dụng 'const' khi bạn sử dụng con trỏ trong các hàm hoặc khai báo prototype. Lý do là để
 * hạn chế việc data nguồn vì một lý do nào đó bị thay đổi trong quá trình xử lý.
 * 5.Sử dụng 'const' cũng giúp cho trình biên dịch tạo mã được tối ưu hóa hơn.
 */

//Ví dụ: 
#include <stdio.h>
#include <stdint.h>

//uint8_t const data = 10;   //Vd2: khai báo biến const,ở dạng global so với hàm main
//Kết quả sau khi chạy run thì chương trình mặc dù ko lỗi những sẽ bị đứng.

int main(void)
{
	uint8_t const data = 10;   //VD1: khai báo biến const, ở dạng local so với hàm main

	printf("Value = %u\n",data);

	//ta sẽ tiến hành thay đổi giá trị của biến const 'data'
	uint8_t *ptr = (uint8_t*)&data; //ép kiểu: (uint8_t*) == uint8_t const *
	//Tạo con trỏ lưu địa chỉ của biến const 'data'
	//Tuy nhiên nếu chỉ viết thông thường là '&data' thì chương trình đưa ra cảnh báo lỗi
	//do địa chỉ này đang là 1 const ko được phép thay đổi giá trị của nó(trạng thái read-only)
	//Vì vậy ta cần ép về kiểu '(uint8_t*)' để thông báo cho trình biên dịch cho phép đc
	//thay đổi giá trị biến lưu tại địa chỉ const

	*ptr = 50;  //thay đổi value của địa chỉ đang đc lưu bởi con trỏ ptr

	printf("Value = %u\n",data);  //data=50

	/*
	 * Ta thấy rằng mặc dù ban đầu biến data đc khai báo là const, nhưng nếu ta thiết lập về kiểu con trỏ
	 * hằng thì sẽ đc phép yêu cầu trình biên dịch cho phép thay đổi value của 1 biến const
	 */

	getchar();
}

/*
 * Giải thích 1: Trường hợp nên sử dụng con trỏ dạng 'const' (cho cách viết 2)
 *
 * Ta có 1 chương trình sau: Viết một chương trình copy từ file nguồn(src) sang file đích(dst)
 * void copy_src_to_dst(uint8_t *src, uint8_t *dst, uint32_t len)
 * {
 * 		for(uint32_t i=0;i<len;i++)
 * 		{
 * 			*dst = *src; 	//dòng1: lấy giá trị tại địa chỉ src gán cho giá trị địa chỉ dst
 * 			dst++;			//tăng giá trị địa chỉ lên 1 đơn vị
 * 			src++;			//tăng giá trị địa chỉ lên 1 đơn vị
 * 		}
 * }
 * Thoạt nhìn ta thấy rằng các dòng code ko có vấn đề gì, và cũng ko có cảnh báo nào
 * việc chạy code cũng sẽ bình thường. Tuy nhiên, vấn đề ở đây liệu có chắc rằng trước hoặc sau khi
 * dòng code 1 được viết thì người lập trình có tác động gì vào giá trị của *src hay không.
 * Vd: viết thêm 1 dòng code: *src = 10;
 * Nếu điều này xảy ra thì giá trị *src sẽ bị thay đổi trong quá trình gán và sẽ tác động đến các hàm
 * khác có sử dụng *src mà ko có cảnh báo nào từ trình biên dịch.
 * -> Giải pháp: khai báo sử dụng con trỏ dạng 'const'
 *
 * void copy_src_to_dst(uint8_t const *src, uint8_t *dst, uint32_t len)
 * {
 * 		for(uint32_t i=0;i<len;i++)
 * 		{
 * 			*dst = *src; 	//dòng1: lấy giá trị tại địa chỉ src gán cho giá trị địa chỉ dst
 * 			dst++;			//tăng giá trị địa chỉ lên 1 đơn vị
 * 			src++;			//tăng giá trị địa chỉ lên 1 đơn vị
 * 		}
 * }
 * Khai báo *src là một 'const', chỉ cho phép đọc dữ liệu mà KHÔNG cho phép thay đổi
 * dữ liêu con trỏ nguồn. Cho dù lập trình viên có vô ý thêm 1 dòng code để làm thay đổi giá trị
 * con trỏ nguồn thì trình biên dịch cũng sẽ dễ dàng phát hiện và báo lỗi cho lập trình viên
 * Cách sử dụng con trỏ 'const' thường được áp dụng cho các chương trình cần bảo vệ dữ liệu nguồn.
 * Tránh các trường hợp dữ liệu nguồn bị thay thế trong quá trình xử lý
 */

/*
 * Giải thích 2: Trường hợp nên sử dụng con trỏ dạng 'const' (cho cách viết 3)
 *
 * Ta có 1 chương trình sau: Viết một chương trình cập nhật tuổi và tiền lương
 * void update_user_data(uint8_t *const pUserAge, uint8_t *const pUserSalary)
 * {
 * 		if(pUserAge != NULL)  //địa chỉ con trỏ đang lưu khác NULL
 * 		{
 * 			*pUserAge getUserAge();
 * 		}
 *
 * 		if(pUserSalary != NULL)
 * 		{
 * 			*pUserAge getUserSalary();
 * 		}
 * }
 * Việc sử dụng khai báo con trỏ ở dạng này sẽ giúp cho lập trình viên thay đổi giá trị tuổi và lương
 * của 1 địa chỉ đại diện cho 1 object và tránh được việc vô tình làm thay đổi địa chỉ mà con trỏ
 * đang lưu trong quá trình xử lý code.
 */

/*
 * Giải thích 3: Trường hợp nên sử dụng con trỏ dạng 'const' (cho cách viết 4)
 *
 * Ta có 1 chương trình sau: Viết một chương trình đọc giá trị của thanh ghi register
 * uint32_t read_status_register(uint32_t const*const pStatusReg)
 * {
 * 		return (*pStatusReg);   //đọc và trả về giá trị của địa chỉ mà con trỏ đang lưu
 * }
 * Ở các hành động liên quan đến đọc trạng thái hiện tại ở các thanh ghi thì thường chỉ đc phép đọc
 * và không cho phép thay đổi địa chỉ hay giá trị địa chỉ đang có.
 * Vì vậy, dù lập trình viên có vô ý tác động thay đổi địa chỉ hoặc giá trị của địa chỉ mà con trỏ
 * đang lưu thì trình biên dịch cũng sẽ báo lỗi. Nếu không có 'const' thì lập trình viên có thể
 * tác động vào kết quả đọc, dẫn đến các hành động không mong muốn tiếp theo, chưa kể đến việc
 * có thể làm lỗ hổng để hacker tấn công hệ thống.
 */

/**-IX-- Macro ---*/
///Lesson 1: Từ khóa "define"
/**
 * Chỉ thị tiền xử lý trong lập trình C (S31)
 *
 * + Macros trong C(#define): (V190)
 * Được sử dụng để thay thế văn bản trong code, dùng để xác định môt hằng số
 * Cú pháp: #define <TÊN_TỪ_THAY_THẾ> <VALUE>
 * Example: #define MAX_RECORD 10   | 	#define PIN_8 8
 * Khác với CONST(hằng số thường là các luật, định luật,ko thay đổi đc) thì define cho phép
 * thay đổi hằng số theo ý lập trình viên tùy vào ngữ cảnh thực hiện công việc.
 * KO có dấu chấm phẩy khi kết thúc 1 define
 * <TÊN_TỪ_THAY_THẾ> : nên viết dạng chữ hoa, và nó là một nhãn dán, ko phải biến, nên sẽ ko tiêu tốn
 * bộ nhớ trong quá trình biên dịch.
 *
 * Thêm: Tạo công thức khi dùng với #define
 * VD: #define PI_VALUE 3.1415
 * #define AREA_OF_CIRCLE(r) PI_VALUE * r * r     //r: giá trị param sẽ đc đưa vào
 *
 * Code chính: areaCircle = AREA_OF_CIRCLE(radius);   //đưa giá trị radius vào công thức
 * ==> areaCircle = PI_VALUE * radius * radius;   <=> areaCircle = 3.1415 * radius * radius;
 * => Tuy nhiên, ko nên hoặc hạn chế sử dụng cách này vì mục đích chính của define là tạo ra các giá trị
 * hằng nhằm thay thế văn bản. Nếu có thêm 1 dòng như "areaCircle = AREA_OF_CIRCLE(radius+1);" thì chắc
 * chắn giá trị của define sẽ thay đổi hoặc ko chính xác.
 * Vd:  areaCircle = AREA_OF_CIRCLE(1+1);  //ý lập trình viên mong muốn đưa 2 vào
 * ==> areaCircle = PI_VALUE * 1+1 * 1+1;  //tạo ra 1 công thức toán sai hoặc khác -> kết quả define sẽ SAI
 *
 * ->Vì vậy khi triển khai define với một công thức toán thì nên thêm dấu '()' để đảm bảo tính toán vẹn
 * vd: #define AREA_OF_CIRCLE(r) ((PI_VALUE) * (r) * (r))
 */

///Lesson 2: Các chỉ thị tiền xử lý nâng cao
/**
 * + Chỉ thị tiền xử lý có điều kiện(#if,#ifdef,#endif,#else,#undef,#ifndef)
 * 1. #if and #endif directive
 * Syntax:
 * #if <constant expression_ Biểu thức điều kiện>
 *
 * #endif
 *
 * Example:
 * #if 0
 * //code block_ nội dung code
 * #endif
 * Giải thích:
 * Lệnh này kiểm tra xem biểu thức hằng có giá trị bằng 0 hay khác không.
 * Nếu hằng số là 0, thì khối mã sẽ không được đưa vào để biên dịch mã.
 * Nếu các hằng số khác 0, khối mã sẽ được đưa vào để biên dịch mã.
 * Các lập trình viên thường dùng cách này để loại trừ một đoạn code khỏi bản build.
 * Có nghĩa là dù bạn viết các dòng code sai hoặc các dong code vô nghĩa giữa #if 0 .... #endif thì khi build code
 * thì trình biên dịch cũng sẽ ko báo lỗi đối với chương trình.
 * Ngược lại, ta có #if 1 .... #endif. Mục đích đoạn code này là ta cho nó kết hợp với #if 0 .... #endif trong quá trình code.
 * Lúc này trong đoạn code sẽ cho phép ta xác định được dòng code nào được build và không được build.
 *
 * 2. #if ...#else ...#endif
 * Syntax:
 * #if <constant expression_ Biểu thức điều kiện>
 *
 * #else
 *
 * #endif
 *
 * Example:
 * #if 1
 * //code_block-1
 * #else
 * //code_block-2
 * #endif
 *
 * * Giải thích: Là cấu trúc nâng cao của lệnh #if.... #endif
 * Lệnh này kiểm tra xem biểu thức hằng có giá trị bằng 1 hay khác không.
 * Nếu hằng số là 1, thì khối mã sẽ được đưa vào để biên dịch mã.
 * Và ngược lại, khối mã sẽ không được đưa vào để biên dịch mã.
 * Ta có thể thay thế số 1 bằng số 0 và ngược lại.
 *
 * 3.#ifdef
 * Systax:
 * #ifdef <identifier_định danh #define có đc khai báo trước đó hay ko>
 *
 * #endif
 *
 * Example:
 * #ifdef New_Feature
 * //code_block
 * #endif
 *
 * Giải thích:
 * Chỉ thị #ifdef kiểm tra xem mã định danh(#define) có được xác định trước hoặc trong chương trình hay không.
 * Nếu định danh được xác định, khối mã sẽ được đưa vào để biên dịch.
 *
 * Thủ thuật: #undef AREA_TRI -> vô hiệu hóa đoạn code đã được thiết lập định danh AREA_TRI của #ifdef...#endif
 *
 * NÂNG CAO: #ifdef and #else
 * Example:
 * #ifdef New_Feature
 * //code_block of New_Feature
 * #else
 * //code_block of Old_Feature
 * #endif
 *
 * 4.#ifndef
 * Syntax
 * #ifndef <identifier>
 *
 * #endif
 *
 * Example:
 * #ifndef New_Feature
 * //Code block for old feature
 * #endif
 *
 * Giải thích:
 * Chỉ thị #ifndef kiểm tra xem mã định danh(#define) có được xác định trong chương trình hay không.
 * Nếu định danh không được xác định(không đc khai báo #define), khối mã sẽ được đưa vào để biên dịch.
 * Ví dụ: nếu ta ko #define AREA_TRI thì khối mã có code block #ifndef AREA_TRI ...#endif sẽ đc thực thi
 * và đồng thời code block #ifdef AREA_CIR ...#endif sẽ không được thực thi
 */
 
///Bài tập minh họa 1: 
#include <stdio.h>

//#define AREA_CIR  //B2_Cách1: Định nghĩa(define) 1 trong 2 định danh mà bạn muốn nó hoạt động với #ifdef...#endif bên dưới
#define AREA_TRI
//#undef AREA_TRI  //vô hiệu hóa định danh

//B2_Cách2: nếu không sử dụng #define thì ta có thể dùng properties->settings->Preprocessor-> bấm vào ký hiệu '+'
//-> thêm định danh (vd: thêm AREA_TRI vào ô đó) -> OK

//Ví dụ về kết hợp các chỉ thị tiền xử lý và từ khóa 'define'
//Vd1: Nếu cả hai từ khóa đều được định danh define trước
//#if defined(AREA_CIR) && defined(AREA_TRI)
//	printf("This is circle area calculation program\n");
//#endif

//Vd2: Nếu cả hai từ khóa đều không được định danh define trước, kết hợp với các từ khóa thông báo
#if !defined(AREA_CIR) && !defined(AREA_TRI)
	//#error "No macros defined."       //thông báo lỗi khi biên dịch
	#warning "No macros defined."		//thông báo cảnh báo lỗi khi biên dịch
#endif

int main()
{
//Ví dụ về cách sử dụng #ifdef..#endif
//B1: tạo các #ifdef...#endif định danh đối với các code_block mà bạn muốn định nghĩa
#ifdef AREA_CIR
   printf("This is circle area calculation program\n");
   float radius = 0;
   fflush(stdout);
   printf("Enter the radius :");
   fflush(stdout);
   scanf("%f", &radius);
   printf("Area of circle  = %f\n",(3.1415 * radius * radius));
   fflush(stdout);
#endif

//B1: tạo các #ifdef...#endif đối với các code_block mà bạn muốn định nghĩa
#ifdef AREA_TRI
   printf("This is Triangle area calculation program\n");
   float base, height;
   fflush(stdout);
   printf("Enter base and height: ");
   fflush(stdout);
   scanf("%f%f",&base, &height);
   printf("Area of triangle = %f\n", (0.5 * base * height));
#endif

/**
 * Sau khi bấm build thì trình biên dịch chỉ lựa chọn chạy đoạn có có mã định danh trùng với mã đc #define.
 * Cụ thể ở đây là trình biên dịch sẽ cho chạy đoạn code có mã định danh #ifdef AREA_TRI..#endif
 */
   return 0;
}

///Bài tập minh họa 2: Tạo calculation sử dụng các chỉ tiền xử lý
/*
 * File 1: math.h 
 */
#ifndef MATH_H_   //Giải thích: ban đầu khi biên dịch sẽ kiểm tra chương trình có define nào ko
#define MATH_H_   //nếu ko sẽ nhảy xuống dòng này, ta sẽ thực hiện lệnh define và tiếp tục thực
				//hiện các lệnh tiền khai báo các hàm. (Lưu ý sau khi thực hiện các lệnh tiền khai báo
				//các hàm thì chúng sẽ đc lưu sẵn trong ram và chờ đc gọi.). việc #ifndef MATH_H_ giúp cho
				//sau này có một file khác gọi thư viện này thì sẽ ko thực hiện đc. Ví dụ nếu ko có lệnh #ifndef MATH_H_
				//thì các lệnh tiền khai báo hàm vẫn sẽ đc thực hiện.Lúc đó trong RAM vô tình có nhiều hàm trùng tên.
				//Điều đó dẫn đến việc xung đột xử lý giữa các hàm khi đc gọi thực hiện nhiệm vụ.

//Khai báo các hàm sẽ đc viết trong file math.c
int math_add(int n1,int n2);
int math_sub(int n1,int n2);
long long int math_mul(int n1,int n2);
float math_div(int n1,int n2);
#endif /* MATH_H_ */

/*
 * File 2: math.c
 */
#include "math.h"				//thư viện định nghĩa các hàm sử dụng
int math_add(int n1,int n2)
{
	return n1+n2;
}
int math_sub(int n1,int n2)
{
	return n1-n2;
}
long long int math_mul(int n1,int n2)
{
	return (long long int)n1*n2;
}
float math_div(int n1,int n2)
{
	return (float)n1/n2;
}

/*
 * File 3: main.c
 */
#include <stdio.h>
#include "math.h"

int main(void)
{
	printf("Add : %X\n",math_add(0x0FFF1111, 0x0FFF1111));

	printf("Sub : %d\n",math_sub(0x0FFF1111, 0x0FFF1111));

	printf("Mul : %I64x\n",math_mul(0x0FFF1111, 0x0FFF1111));

	printf("Div : %f\n",math_div(100,8));

	return 0;
}

/**-X-- Bit Wise ---*/
///Lesson 1: Giới thiệu
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
	int32_t num1 ,num2;
	printf("Enter 2 numbers(give space between 2 nos):");
	scanf("%d %d",&num1,&num2);

	printf("Bitwise AND(&) : %d\n",(num1 & num2));
	printf("Bitwise OR (|) : %d\n",(num1 | num2));
	printf("Bitwise XOR(^) : %d\n",(num1 ^ num2));
	printf("Bitwise NOT(~) : %d\n",(~num1));

    wait_for_user_input();
}

/**
 * Hàm chờ nhấn nút bất kỳ để thoát chương trình
 */
void wait_for_user_input(void)
{

	printf("Press enter key to exit this application");

    while(getchar() != '\n')
    {
    	//just read the input buffer and do nothing
    }
    getchar();

}

///Lesson 2: Phương pháp bit_masking(hay gọi là bit mặt nạ):
/*
 * Mục đích của phương pháp này là dùng 1 bit mặt nạ A và bitWise với bit data nhằm thay đổi trạng thái
 * của 1 hay nhiều bit data.
 *
 * VD: thay đổi trạng thái bit từ 0 thành 1 hoặc từ 1 thành 0
 * Cách 1(Testing bits): dùng toán tử bitwise AND(&) với 1 dãy bit mặt mạ do bản thân tính toán trước
 * VD: [data] 00101110    hoặc 	 10101110    hoặc 	001011110
 * 				&                   &					&
 * 	   [mask] 00000010		     10000000			000000011
 * 	   [out]  00000010			 10000000			000000010
 *
 * -> Phương pháp này thường đc sử dụng khi chỉ muốn tạo 1 vị trí bit đầu ra bằng 1 các bit còn lại bằng 0
 * -> Có nghĩa là chỉ vị trí bit nào có hai ngõ vào đều bằng 1 thì vị trí ngõ ra mới bằng 1
 *
 * Cách 3(Clearing bit): dùng toán tử bitwise AND(&) với 1 dãy bit mặt mạ do bản thân tính toán trước
 * VD: [data] 10111110    hoặc 	 10111110
 * 				&                   &
 * 	   [mask] 10001111		   ~(10000000)
 * 	   [out]  10001110			 10000000
 * 	-> hương pháp này thường đc sử dụng khi cần xóa hoặc đưa 1 vùng bit data về trạng thái 0
 * */
#include<stdio.h>
#include<stdint.h>

void wait_for_user_input(void);
int main(void)
{
	int32_t num1;
	printf("Enter a number:");
	scanf("%d",&num1);

	if(num1 & 1){    //biến mask_value : 1 = 0000 0001
		printf("%d is odd number\n",num1);   //số lẻ
	}else{
		printf("%d is even number\n",num1);	//số chẵn
	}

    wait_for_user_input();
}

///Lesson 3: Phương pháp bit_masking(hay gọi là bit mặt nạ):
/*
 * Mục đích của phương pháp này là dùng 1 bit mặt nạ và bitWise với bit data nhằm thay đổi trạng thái
 * của 1 hay nhiều bit data.
 *
 * VD: thay đổi trạng thái bit từ 0 thành 1 hoặc từ 1 thành 0
 * Cách 2(Settings of bit): dùng toán tử bitwise OR(|) với 1 dãy bit mặt ta bản thân tính toán trước
 * VD: [data] 00111110
 * 				|
 * 	   [mask] 10010000
 * 	   [out]  10111110
 *
 * -> Phương pháp này thường đc sử dụng khi ta muốn đảo trạng thái tất cả các bit bên trong data nguồn
 *
 * Cách 2(Toggling of bit): dùng toán tử bitwise XOR(^) với 1 dãy bit mặt ta bản thân tính toán trước
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

/**-XI-- C to Arm ---*/
///Bài 1: Lập trình sáng LED
/*
 * Tính địa chỉ cho các bước bật sáng led:

Address of clock control register(AHB1ENR)
0x40023800+0x30 => 	0x40023830

Address of the GPIO mode register (used to control mode)
0x40020C00 + 0x00 	=>	0x40020C00

Address of the GPIO output data register (used to wroite)
0x40020C00 + 0x14  =>  0x40020C14
*/

/*
 * Tiến trình thực hiện việc bật tắt led
 * B1: Cần xác định cổng GPIO là ngoại vi được sử dụng để kết nối với đèn LED
 * Ở bài này ta sẽ sử dụng port GPIOD
 *
 * B2: Cần xác định pin GPIO muốn sử dụng
 * Ở bài này ta sẽ sử dụng pin 12 của port GPIOD
 *
 * B3: Kích hoạt ngoại vi GPIO(Enable the clock)
 * Có nghĩa là: theo mặc định,đa số các pin port sẽ đều ở trạng thái ngủ đông và cần chờ một xung clock
 * tín hiệu để kích(cho phép) pin đó hoạt động.
 * Lưu ý: bạn sẽ ko sử dụng được điều khiển ngoại vi pin cho đến khi bạn kích hoạt xung clock
 *
 * B4: Định cấu hình đầu ra cho chân pin(viết code thiết lập chân đó điều khiển tác dụng gì )
 * vd: muốn chân đó là chân output, hoặc input hoặc chân nhận tín hiệu analog
 * Vì vậy cần dựa vào datasheet để cấu hình cho pin port
 *
 * B5: Write to the GPIO pin (thiết lập điều khiển pin đã chọn điều khiển led)
 * 1(high) tạo chân pin GPIO ở mức HIGH(3.3V) - sáng
 * 0(high) tạo chân pin GPIO ở mức HIGH(3.3V) - tắt
 * */

#include <stdint.h>		//thư viện định dạng khai báo biến

int main(void)
{
	//Xác định địa chỉ thanh ghi ban đầu các thành phần cần điều khiển vào các biến đại diện
    uint32_t *pClkCtrlReg = (uint32_t*)0x40023830;    //bước 3: đăng ký clock
    //Giải thích cách lấy địa chỉ thanh ghi clock
    //Địa chỉ thanh ghi Rigister and Clock(lấy xung clock) trong Memory map là 0x40023800
    //Và vì port D kết nối với memory map thông qua thanh ghi AHB1 có địa chỉ là 0x30
    //Địa chỉ của xung clock cần lấy cho port D là 0x40023830+0x30 = 0x40023830
    //Tại sao lại phép cộng: Memory map có địa chỉ là từ  0x40023800 đến 0x40023BFF
    //Mà thanh ghi AHB1 thuộc Memory map nên địa chỉ thanh ghi AHB1 sẽ nằm từ 0x40023800 đến 0x40023BFF
    //Và theo datasheet thì ta có Address offset(địa chỉ phần bù) của AHB1 là 0x30
    //Có nghĩa là cần lấy địa chỉ gốc thanh ghi RCC(0x40023800) + với phần bù 0x30
    //sẽ cho ra địa chỉ thanh ghi AHB1 nằm trên Memory map

    //Việc ép kiểu con trỏ ở đây để chương trình xác định đây là dãy số địa chỉ cần sử dụng
    //để gián tiếp mở khóa xung clock sử dụng cho port D

    uint32_t *pPortDmodeReg = (uint32_t*)0x40020C00;	//bước 4: cấu hình port sử dụng (chức năng của chân pin)
    //0x40020C00 : địa chỉ cở sở của port D (tìm trong datasheet)
    //vì trong port D sẽ chứa nhiều thanh ghi chức năng khác nhau nên cần xác định địa chỉ thanh ghi chức
    //năng mà ta muốn điều khiển
    //Ở đây ta muốn tìm địa chỉ thanh ghi để đăng ký kích hoạt chế độ điều khiển port-thiết lập chức năng chân pin
    //Và địa chỉ phần bù của thanh ghi này là 0x00
    //=> Địa chỉ thanh ghi cần sử dụng là: 0x40020C00 + 0x00 = 0x40020C00

    //Thêm: Mở datashet tìm phần đăng ký cấu hình cho port GPIO(phần 8.4.1)
    //Có thể thiết lập 4 chức năng cho 1 pin(mỗi port GPIO có 16 pin, 1pin chiếm 2 bit_có 32 bit từ 0->31)
    //00: input and reset ;  01: output  ;  10: chức năng thay thế   ;   11: tín hiệu analog

    uint32_t *pPortDoutReg = (uint32_t*)0x40020C14;			//bước 5: cấu hình địa chỉ port làm output
    //Ta sẽ thiết lập port ở chế độ output nên ta sẽ tìm và đăng ký thanh ghi có chức năng này
    //Địa chỉ phần bù: 0x14 -> địa chỉ cần đăng ký 0x40020C00 + 0x14 = 0x40020C14
    //Vì ta sẽ điều khiển đèn led LD4 và theo sở đồ khối thì LD4 thuộc chân pin số 12
    //trong GPIO thì pin 12 sẽ lấy bit 24 và bit 25

    //I. Su dung PP mat na bit
    //1.Bat dong ho cho thiet bi ngoai vi AHB1ENR
    //Cách viết 1:
    uint32_t temp = *pClkCtrlReg;
    temp = temp | 0x08;          //dung mat na bit  //modify  //su dung  OR để nâng bit4 lên 1 của dãy 32 bit
    *pClkCtrlReg = temp;		//write back

    //Cách viết 2:
    //*pClkCtrlReg = *pClkCtrlReg | 0x08;

    //cách viết 3
    //*pClkCtrlReg |= 0x08

    //Giải thích: Ở đây ta muốn yêu cầu cấp 1 xung clock(bit lên 1) cho port D
    //Theo datasheet thì địa chỉ cấp xung clock port D trong pClkCtrlReg nằm ở bit thứ 4 trong 32 bit
    //Ta sẽ đưa bit thứ 4 đó lên 1 và cho rằng các bit còn lại đều 0
    //Để làm được điều này thì ta cần quy đổi tính toán bit một chút:
    //Tất cả bit đều tắt:     0000 0000 0000 0000 0000 0000 0000 0000  (bit 4 là 0)
    //Dùng pp mặt nạ bit, ở dây ta cho nó OR với 1 dãy số bit mà ở ta muốn có vị trí nhảy lên 1
    //Và dễ nhất là cho nó OR với  0000 1000 = 0x08 để vị trí bit 4 nhảy lên 1

    //						  0000 0000 0000 0000 0000 0000 0000 0000  (bit 4 là 0)
    //											|(OR)
    //														0000 1000  (0x08)
    // -> Ta có dãy nhị phân: 0000 0000 0000 0000 0000 0000 0000 1000  (bit 4 đã lên 1)
    // -> Thập lục phân:       0     0    0    0    0    0    0    8

    //2.Cau hinh che do chan IO lam dau ra
    //Mặc định ban đầu tất cả 32 bit đều ở trạng thái 0
    //a.clear(reset) the bit 24 and 25, với cấu hình chân 00, xóa các chức năng cũ mà có thể đã thiết lập trước đó
    *pPortDmodeReg &= 0xFCFFFFFF;   //dung phep AND nang tat ca bit len 1 trừ 2 bit bit 24 va 25
    //1111 1100 1111 1111 1111 1111 1111 1111
    // F    C    F    F    F    F    F    F

    //b. set bit 24 len 1, đặt chế độ output cho chân pin 12 của port D(yêu cầu cho phép sử dụng pin 12)
    *pPortDmodeReg |= 0x01000000;		//dung phep OR nang 1 bit
    //0000 0001 0000 0000 0000 0000 0000 0000
    // 0    1    0    0    0    0    0    0
    //Giải thích: pin 12 port D thuộc 2 bit là 25 và 24, để thiết lập output cho pin này thì theo datasheet
    //thì cần thiết lập bit 25,24 lần lượt là 01=output.

    //3. Set output chan 12 GPIO len 1 cho LED sáng
    *pPortDoutReg |= 0x1000;
    //0001 0000 0000 0000 		(pin 12 lên 1, pin thì chỉ có 16 pin)
    // 1    0    0    0

    //II. Su dung phuong phap dich bit(tham khảo C19_Bitwise shift operator: phương pháp dịch bit)
    //1.Bat dong ho cho thiet bi ngoai vi AHB1ENR set bit 3 lên 1 (địa chỉ bit sẽ chạy từ 0->31)
//    *pClkCtrlReg |= (1<<3); //= *pClkCtrlReg = *pClkCtrlReg| (1<<3);
//	  //Giải thích: đầu tiên tưởng tượng 1 dãy ảo với bit 0 là 1, các bit khác là 0(số lượng bit 0 sẽ
//    //phụ thuộc vào só lượng bit thanh ghi muốn cấu hình)
//    //(1<<3): dịch dãy bit tưởng tượng 3 lần: ...1000(lần 3)<-...0100(lần 2)<-...0010(lần 1)<-...0001(lần 0)
//	  //Sau khi dịch dãy bit ảo sẽ có bit 3 được set lên 1
//	  //Ta tiến hành OR dãy bit ảo này với dãy bit thanh ghi muốn cấu hình
//    //<< : dịch từ phải sang trái
//
//    //2.Cau hinh che do chan IO lam dau ra
//    //a.clear the bit 24 and 25
//    *pPortDmodeReg &= ~(3<<24) ;    //3 = 11(tác động vào 2bit) : set 2bit 24,25 về thành 00(về chế độ reset)
//
//    //b. set bit 24 len 1, cho phép cấu hình port D là output
//    *pPortDmodeReg |= (1<<24);		//dich bit cho chan 24 len 1
//
//    //3. Set chan 12 GPIO len 1 cho LED sang
//    *pPortDoutReg |= (1<<12);		//dich bit cho chan 12 len 1

	while(1);
}

///Bài 2: Lập trình LED sáng tắt
#include <stdint.h>
int main(void)
{
	//Phần thiết lập các cấu hình ban đầu cho hệ thống nhúng
    uint32_t *pClkCtrlReg = (uint32_t*)0x40023830;
    uint32_t *pPortDmodeReg = (uint32_t*)0x40020C00;
    uint32_t *pPortDoutReg = (uint32_t*)0x40020C14;


    //II. Su dung phuong phap dich bit
    //1.Bat dong ho cho thiet bi ngoai vi AHB1ENR set bit 3
    *pClkCtrlReg |= ( 1 << 3 );

    //2.Cau hinh che do chan IO lam dau ra
    //a.clear the bit 24 and 25
    *pPortDmodeReg &= ~ (3 << 24 ) ;    //3 = 11 : set bit 24,25 ve 0

    //b. set bit 24 len 1
    *pPortDmodeReg |= ( 1 << 24 );		//dich bit cho chan 24 len 1
    //Kết thúc cấu hình ban đầu cho hệ thống

    //Xử lý các hành động sẽ lặp đi lặp lại trong hệ thống
    while(1)
    {
    //3. Set chan 12 GPIO len 1 cho LED sang
    *pPortDoutReg |= ( 1 << 12 );		//dich bit cho chan 12 len 1

    //thiet lap ham delay led
    //tao ra n vong lap, để để làm chậm nhịp xử lý
    for(uint32_t i = 0 ; i < 300000 ; i++);

    //Turn off LED
    *pPortDoutReg &= ~( 1 << 12 );

    for(uint32_t i = 0 ; i < 300000 ; i++);
    }
}

//Bài 2.1: Lập trình LED sáng tắt (Phương pháp Bit Field)
/*
 * Cách sử dụng pp bit-fields trong hệ thống nhúng:
 * Đặt vấn đề: Ở bài tập LedOn, để thiết lập vị trí bit cần được sử dụng thì chúng ta đã dùng
 * phương pháp mặt nạ bit. Tuy nhiên pp này có nhược điểm là việc xác định vị trí bit của thanh ghi,
 * số bit cần dịch và toán tử sử dụng(OR hoặc AND) khá là phức tạp.
 *
 * Phương Pháp bit-fields cung cấp cho ta một cách làm đơn giản hơn để xác định các công việc
 * xử lý bit trên thanh ghi.
 * Điều kiện tiên quyết: dựa vào datasheet, phải viết lại cấu hình từng bit của thanh ghi trong file thư viện.
 *
 * Cách làm:
 * B1: Tạo thư viện cấu hình chân_bit của các thanh ghi đc sử dụng để lập trình Header File (sử dụng cấu trúc typedef struct)
 * Vd: ở bài LedOn ta sử dụng 3 thanh ghi nên sẽ viết thư viện cấu hình lại cho 3 thanh ghi đó
 * Lưu ý: ko cần thiết viết thư viện cho các thanh ghi không được sử dụng.
 *
 * B2: Thiết lập địa chỉ cho các struct(đại diện cho các thanh ghi)
 * Sử dụng các typedef struct đã tạo làm kiểu_biến đại diện cho thanh ghi đã đc cấu hình trong file thư viện
 *
 * B3: Cấu hình(điều khiển) bit các chân thanh ghi theo cấu hình chân đã cài đặt theo thư viện
 * Ta chỉ cần biết số chân và sử dụng con trỏ struct(tên_biến_struct -> thành_phần) để xử lý cấu hình cho chân bit
 *
 * Đánh giá: Phương pháp bit-field, mặc dù pp này cần phải viết cấu hình chân các thanh ghi trong một file thư viện
 * nhưng việc này giúp cho lập trình viên về sau sẽ viết code đc dễ dàng hơn. Tránh đc tình trạng điều khiển bit
 * sai do việc tính toán dịch bit của pp mặt nạ bit.
 * ->Đây là pp nên sử dụng cho các dự án hệ thống nhúng
 * */
 
/*
 * File 1: main.h
 * Đây là (B1) file thư viện cấu hình bit của các chân thanh ghi đc sử dụng trong chương trình
 */
//Thiết lập tiền xử lý
#ifndef MAIN_H_
#define MAIN_H_
//Thiết lập tiền xử lý tránh cho việc thư viện bị gọi khởi động nhiều lần, khi gọi lần đầu nó sẽ đc lưu lại trong
//Ram để khi cần có thể gọi tiếp sử dụng mà ko cần phải gọi khởi động nhiều lần

#include <stdint.h>

//Thiết lập chân_bit trên thanh ghi RCC_AHB1ENR dựa vào hình 7.3.10 của datasheet
typedef struct
{
	uint32_t gpioa_en   	: 1;			//:1	pin chiếm 1bit
	uint32_t gpiob_en   	: 1;
	uint32_t gpioc_en   	: 1;
	uint32_t gpiod_en   	: 1;
	uint32_t gpioe_en   	: 1;
	uint32_t gpiof_en   	: 1;
	uint32_t gpiog_en   	: 1;
	uint32_t gpioh_en   	: 1;
	uint32_t gpioi_en   	: 1;
	uint32_t reserved1   	: 3;			//:3	pin chiếm 3bit
	uint32_t crc_en     	: 1;
	uint32_t reserved2   	: 3;
	uint32_t reserved3   	: 2;
	uint32_t bkpsram_en 	: 1;
	uint32_t reserved4     	: 1;
	uint32_t ccmdataram_en 	: 1;
	uint32_t dma1_en		: 1;
	uint32_t dma2_en 		: 1;
	uint32_t reserved5 		: 2;
	uint32_t ethmac_en 		: 1;
	uint32_t ethmactxe_en	: 1;
	uint32_t ethmacrxe_en	: 1;
	uint32_t ethmacptp_en	: 1;
	uint32_t otghs_en 		: 1;
	uint32_t otghsulpi_en 	: 1;
	uint32_t reserved6 		: 1;

}RCC_AHB1ENR_t;

//Thiết lập chân_bit trên thanh ghi GPIOx_MODE_t dựa vào phần 8.4.1 của datasheet
typedef struct
{
	uint32_t pin_0			:2;
	uint32_t pin_1			:2;
	uint32_t pin_2			:2;
	uint32_t pin_3			:2;
	uint32_t pin_4			:2;
	uint32_t pin_5			:2;
	uint32_t pin_6			:2;
	uint32_t pin_7			:2;
	uint32_t pin_8			:2;
	uint32_t pin_9			:2;
	uint32_t pin_10			:2;
	uint32_t pin_11			:2;
	uint32_t pin_12			:2;
	uint32_t pin_13			:2;
	uint32_t pin_14			:2;
	uint32_t pin_15			:2;

}GPIOx_MODE_t;

//Thiết lập chân_bit trên thanh ghi GPIOx_ODR_t dựa vào hình 8.4.6 của datasheet
typedef struct
{
	uint32_t pin_0			:1;
	uint32_t pin_1			:1;
	uint32_t pin_2			:1;
	uint32_t pin_3			:1;
	uint32_t pin_4			:1;
	uint32_t pin_5			:1;
	uint32_t pin_6			:1;
	uint32_t pin_7			:1;
	uint32_t pin_8			:1;
	uint32_t pin_9			:1;
	uint32_t pin_10			:1;
	uint32_t pin_11			:1;
	uint32_t pin_12			:1;
	uint32_t pin_13			:1;
	uint32_t pin_14			:1;
	uint32_t pin_15			:1;
	uint32_t reserved 		:16;

}GPIOx_ODR_t;
#endif /* MAIN_H_ */

/*
 * File 2: main.c
 */
#include "main.h"   //B1: thư viện cấu hình chân pin (dùng để xác định chân khi sử dụng pp bit-fields)

/*uint32_t *pClkCtrlReg 	= (uint32_t*)0x40023830;
uint32_t *pPortDmodeReg = (uint32_t*)0x40020C00;
uint32_t *pPortDoutReg  = (uint32_t*)0x40020C14;*/

int main(void)
{
	//B2: Thiết lập địa chỉ thanh ghi cho các struct(đại diện cho các thanh ghi)
	RCC_AHB1ENR_t volatile *const pClkCtrlReg   = (RCC_AHB1ENR_t *)0x40023830;
	GPIOx_MODE_t volatile *const  pPortDmodeReg = (GPIOx_MODE_t *)0x40020C00;
	GPIOx_ODR_t volatile *const pPortDoutReg    = (GPIOx_ODR_t *)0x40020C14;

	//B3: Cấu hình(điều khiển) bit các chân thanh ghi theo cấu hình chân đã cài đặt theo thư viện
	//1.Thiết lập xung clock thanh ghi AHB1ENR set bit 3
	pClkCtrlReg->gpiod_en = 1;      //thiết lập chân gpiod_en của thanh ghi pClkCtrlReg lên trạng thái 1

	//2.Cấu hình port làm ngõ ra IO
	pPortDmodeReg->pin_12 = 1; //chân pin 12(gồm bit25,24) của thanh ghi pPortDmodeReg lên 1:
	//1: có nghĩa là bit25=0,bit24=1,
	while(1)
	{
		//3. Set chan 12 GPIO của thanh ghi pPortDoutReg lên 1 cho LED sáng
		pPortDoutReg->pin_12 = 1;

	    //thiet lap ham delay led
	    //tao ra 10000 vong lap
	    for(uint32_t i = 0 ; i < 300000 ; i++);

	    //Turn of LED
	    pPortDoutReg->pin_12 = 0;

	    for(uint32_t i = 0 ; i < 300000 ; i++);
	}
}

//Bài 2.2: Lập trình LED sáng tắt (Phương pháp Bit Field và Macro)
/*
 * File 1: main.h
 * Đây là (B1) file thư viện cấu hình bit của các chân thanh ghi đc sử dụng trong chương trình
 */

//Thiết lập tiền xử lý
#ifndef MAIN_H_
#define MAIN_H_
//Thiết lập tiền xử lý tránh cho việc thư viện bị gọi khởi động nhiều lần, khi gọi lần đầu nó sẽ đc lưu lại trong
//Ram để khi cần có thể gọi tiếp sử dụng mà ko cần phải gọi khởi động nhiều lần

#include <stdint.h>

//B1.2 some macro: thay thế các địa chỉ thanh ghi bằng các từ khóa
#define ADDR_REG_AHB1ENR ((RCC_AHB1ENR_t *)0x40023830)
#define ADDR_REG_GPIO_MODE ((GPIOx_MODE_t *)0x40020C00)
#define ADDR_REG_GPIO_OD ((GPIOx_ODR_t *)0x40020C14)

//B1.3: Thay thế việc điều khiển 1 hoặc 0 bằng các biến(kiểu câu lệnh để dễ lập trình hơn)
#define CLOCK_ENABLE 		(1)
#define MODE_CONF_OUTPUT 	(1)
#define PIN_STATE_HIGH 		(1)
#define PIN_STATE_LOW 		(0)

#define DELAY_COUNT 		(300000UL)  //UL: cho biết giá trị ở dạng 32 bit

//B1.1:Thiết lập chân_bit trên thanh ghi RCC_AHB1ENR dựa vào hình 7.3.10 của datasheet
typedef struct
{
	uint32_t gpioa_en   	: 1;			//:1	pin chiếm 1bit
	uint32_t gpiob_en   	: 1;
	uint32_t gpioc_en   	: 1;
	uint32_t gpiod_en   	: 1;
	uint32_t gpioe_en   	: 1;
	uint32_t gpiof_en   	: 1;
	uint32_t gpiog_en   	: 1;
	uint32_t gpioh_en   	: 1;
	uint32_t gpioi_en   	: 1;
	uint32_t reserved1   	: 3;			//:3	pin chiếm 3bit
	uint32_t crc_en     	: 1;
	uint32_t reserved2   	: 3;
	uint32_t reserved3   	: 2;
	uint32_t bkpsram_en 	: 1;
	uint32_t reserved4     	: 1;
	uint32_t ccmdataram_en 	: 1;
	uint32_t dma1_en		: 1;
	uint32_t dma2_en 		: 1;
	uint32_t reserved5 		: 2;
	uint32_t ethmac_en 		: 1;
	uint32_t ethmactxe_en	: 1;
	uint32_t ethmacrxe_en	: 1;
	uint32_t ethmacptp_en	: 1;
	uint32_t otghs_en 		: 1;
	uint32_t otghsulpi_en 	: 1;
	uint32_t reserved6 		: 1;

}RCC_AHB1ENR_t;

//Thiết lập chân_bit trên thanh ghi GPIOx_MODE_t dựa vào phần 8.4.1 của datasheet
typedef struct
{
	uint32_t pin_0			:2;
	uint32_t pin_1			:2;
	uint32_t pin_2			:2;
	uint32_t pin_3			:2;
	uint32_t pin_4			:2;
	uint32_t pin_5			:2;
	uint32_t pin_6			:2;
	uint32_t pin_7			:2;
	uint32_t pin_8			:2;
	uint32_t pin_9			:2;
	uint32_t pin_10			:2;
	uint32_t pin_11			:2;
	uint32_t pin_12			:2;
	uint32_t pin_13			:2;
	uint32_t pin_14			:2;
	uint32_t pin_15			:2;

}GPIOx_MODE_t;

//Thiết lập chân_bit trên thanh ghi GPIOx_ODR_t dựa vào hình 8.4.6 của datasheet
typedef struct
{
	uint32_t pin_0			:1;
	uint32_t pin_1			:1;
	uint32_t pin_2			:1;
	uint32_t pin_3			:1;
	uint32_t pin_4			:1;
	uint32_t pin_5			:1;
	uint32_t pin_6			:1;
	uint32_t pin_7			:1;
	uint32_t pin_8			:1;
	uint32_t pin_9			:1;
	uint32_t pin_10			:1;
	uint32_t pin_11			:1;
	uint32_t pin_12			:1;
	uint32_t pin_13			:1;
	uint32_t pin_14			:1;
	uint32_t pin_15			:1;
	uint32_t reserved 		:16;

}GPIOx_ODR_t;

#endif /* MAIN_H_ */

/*
 * File 2: main.c
 * Thiết lập các chỉ thị define _ macro cho các địa chỉ sử dụng (nằm trong file .h)
 * */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include "main.h"   //B1: thư viện cấu hình chân pin (dùng để xác định chân khi sử dụng pp bit-fields)

/*uint32_t *pClkCtrlReg 	= (uint32_t*)0x40023830;
uint32_t *pPortDmodeReg = (uint32_t*)0x40020C00;
uint32_t *pPortDoutReg  = (uint32_t*)0x40020C14;*/

int main(void)
{
	//B2: Thiết lập địa chỉ cho các struct(đại diện cho các thanh ghi)
	RCC_AHB1ENR_t volatile *const pClkCtrlReg   = ADDR_REG_AHB1ENR;
	GPIOx_MODE_t volatile *const  pPortDmodeReg = ADDR_REG_GPIO_MODE;
	GPIOx_ODR_t volatile *const pPortDoutReg    = ADDR_REG_GPIO_OD;

	//B3: Cấu hình(điều khiển) bit các chân thanh ghi theo cấu hình chân đã cài đặt theo thư viện
	//1.Thiết lập xung clock thanh ghi AHB1ENR set bit 3
	pClkCtrlReg->gpiod_en = CLOCK_ENABLE;      //thiết lập chân gpiod_en của thanh ghi pClkCtrlReg lên trạng thái 1

	//2.Cấu hình port làm ngõ ra IO
	pPortDmodeReg->pin_12 = MODE_CONF_OUTPUT; //chân pin 12(bit25,24) của thanh ghi pPortDmodeReg lên 1
	//1: có nghĩa là bit25=0,bit24=1,

	while(1)
	{
		//3. Set chan 12 GPIO của thanh ghi pPortDoutReg lên 1 cho LED sáng
		pPortDoutReg->pin_12 = PIN_STATE_HIGH;

	    //thiet lap ham delay led
	    //tao ra 10000 vong lap
	    for(uint32_t i = 0 ; i < DELAY_COUNT ; i++);

	    //Turn of LED
	    pPortDoutReg->pin_12 = PIN_STATE_LOW;

	    for(uint32_t i = 0 ; i < DELAY_COUNT ; i++);
	}
}

//Bài 3: Đọc trạng thái nút nhấn
/*
 * Thiết lập phần cứng:
 * Nối chân PA0 với VDD: input = 1;
 * Nối chân PA0 với VDD: input = 0;
 * */
#include <stdint.h>

int main(void)
{
	//Khai báo cấu hình địa chỉ xung clock
    uint32_t *pClkCtrlReg = (uint32_t*)0x40023830;

    //Khai báo cấu hình các địa chỉ cần thiết thiết lập port D
    uint32_t *pPortDmodeReg = (uint32_t*)0x40020C00;
    uint32_t *pPortDoutReg = (uint32_t*)0x40020C14;

    ///Khai báo cấu hình các địa chỉ cần thiết thiết lập port A
    uint32_t *pPortAmodeReg = (uint32_t*)0x40020000 ;  //GPIOA
    uint32_t *pPortAInReg =   (uint32_t*)0x40020010 ;  //GPIOA 0x40020000 + 0x01(address offset)
    //Đây là địa chỉ cấu hình để thiết lập đọc trạng thái của các pin port A

    //II. Su dung phuong phap dich bit
    //1.Bat dong ho cho thiet bi ngoai vi GPIOD, GPOIA o AHB1ENR
    *pClkCtrlReg |= ( 1 << 3 );			//GPIOD
    *pClkCtrlReg |= ( 1 << 0 );			//GPIOA

    //2.Cau hinh che do chan IO lam dau ra
    //a.clear the bit 24 and 25  PD12 là output
    *pPortDmodeReg &= ~ ( 3 << 24 ) ;    //3 = 11 : set bit 24,25 ve 0
    //b. set bit 24 len 1
    *pPortDmodeReg |= ( 1 << 24 );		//dich bit cho chan 24 len 1

    //thiet lap PA0 input (GPIO MODE REGISTER)
    *pPortAmodeReg &= ~( 3 << 0);		//3 = 11 : set bit 0,11 về 00
    // vừa là trạng thái reset vừa là trạng thái input (xem datasheet phần GPIO port mode register)

    //Gợi ý: &(AND) : thường sẽ thay đổi tất cả trạng thái của tất cả bit
    //		 |(NOT) : chỉ thay đổi bit mà ta muốn thay đổi

    while(1)
    {
    	//doc trang thai chan PA0
    	uint8_t pinStatus = (uint8_t)(*pPortAInReg & 0x1);  //set tat ca bit khac la 0 tru bit 0 là 1
    	//lúc này việc set bit 0 lên 1 có nghĩa là yêu cầu chương trình đọc giá trị đang có tại chân PA01
    	//pPortAInReg : chứa địa chỉ mà ta đã cấu hình ban đầu là cho phép đọc chân pin của port A

		if(pinStatus)
		{
			*pPortDoutReg |= ( 1 << 12 );
		}
		else{
			*pPortDoutReg &= ~ ( 1 << 12 );
		}

    //3. Set chan 12 GPIO len 1 cho LED sang
    //*pPortDoutReg |= ( 1 << 12 );		//dich bit cho chan 12 len 1
    }
}

//Bài 3.1: Đọc trạng thái nút nhấn (Sử dụng tối ưu hóa)
/*
 * Optimazation: Tối ưu hóa trong mã code
 * Tối ưu hóa là một loạt các hành động được thực hiện bởi trình biên dịch trên mã chương trình của bạn
 * Mục đích:
 * + Giảm một số hướng dẫn(instruction) trên code của bạn
 * + Giảm thời gian truy cập bộ nhớ
 * + Giảm mức tiêu thụ điện
 * Nếu thời gian truy cập bộ nhớ và một số hướng dẫn giảm thì bạn có thể nhận được mức tiêu thụ năng lượng
 * tốt hơn.
 *
 * Theo mặc định, trình biên dịch không gọi bất kỳ tối ưu hóa nào trong chương trình của bạn.
 * Bạn có thể bật tối ưu hóa bằng cờ biên dịch
 *
 * Cờ biên dịch(compiler flags) gồm các mức tối ưu hóa: O0,O1,O2,O3
 * +O0: đây là mức tối ưu hóa bằng 0.
 * Khi bạn có giới hạn về code và giới hạn bộ nhớ RAM thì hầu như không có tối ưu hóa nào được thực hiện
 * trên mã của bạn. Trình biên dịch cũng sẽ không gọi bất kỳ thuật toán tối ưu hóa nào nên tốc độ biên
 * dịch của nó cũng rất nhanh.
 * Đây là mức gỡ lỗi thân thiện, có nghĩa là khi bạn đang cố gắng gỡ lỗi mã của mình,sẽ có một mapping
 * (bản đồ) giữa hướng dẫn(instruction) và source nguồn đc viết. Các mapping sẽ giúp lập trình viên kiểm
 * tra được các lệnh,các lỗi xảy ra,.. trong quá trình biên dịch.Nó được sử dụng trong quá trình phát triển.
 * Lưu ý: Các mã đã hoạt động với tối ưu hóa O0 thì có thể KHÔNG hoạt động với các tối ưu O1,O2,O3. Vì vậy,
 * code cần được xác minh lại.
 *
 * +O1: Đây là mức tối ưu hóa vừa phải
 * Dùng để giảm thời gian truy cập bộ nhớ và không gian code.
 *
 * +O2: Đây là mức tối ưu hóa hoàn toàn
 * Dẫn đến thời gian biên dịch chương trình chậm. Vì lúc này trình biên dịch sẽ phải gọi các thuật toán tối
 * ưu hóa khác nhau để xử lý code của bạn
 * Đây không phải là mức dùng để gỡ lỗi(cả mức tối ưu hóa O3) vì nó sẽ không tạo ra mapping giữa instruction
 * và source code
 *
 * +O3: Đây là mức tối ưu hóa hoàn toàn O2+ (thêm một số mức tối ưu hóa cao hơn O2)
 * Dẫn đến nó cũng gây ra tốc độ biên dịch chậm nhất
 * Có thể gây ra lỗi chương trình nếu không được tối ưu hóa đúng cách.
 * Vì có thể gây ra lỗi nên nếu bạn bật mức tối ưu hóa này thì ứng dụng của bạn có thể không hoạt động
 * đúng. vì vậy cso thể bạn sẽ phải viết lại code chương trình. (Cần chú ý khi sử dụng mức O3)
 *
 * =>Tùy vào chương trình mà bạn nên chọn mức tối ưu hóa phù hợp
 * =>Sau khi chọn mức tối ưu hóa bắt buộc phải run lại chương trình để kiểm tra xem chương trình
 * ở mức tối ưu hóa đó có hoạt động hay không hoặc hoạt động có ổn định hay không.
 *
 * Cách chọn tối ưu hóa: Chuột phải -> properties -> C/C++ Build -> Settings -> Tool Settings
 * -> MCU GCC Compiler -> Optimization -> Chọn mức tối ưu hóa (Optimization level)
 * */

/*
 * Sử dụng từ khóa 'volatile' kết hợp với việc tối ưu hóa:
 * Trường hợp 1: Ban đầu khi biến *pPortAInReg khi chưa đc thêm từ khóa 'volatile' thì khi run thì trình
 * biên dịch sẽ bỏ qua lệnh đọc giá trị chân biến này(do nhận thấy đây ko phải là biến đc sử dụng)
 * và chỉ tiếp tục các lệnh if_else, nhưng biến *pPortAInReg sẽ đọc giá trị thay đổi của người dùng
 * để thực hiện nhiệm vụ sáng tắt theo yêu cầu, vì vậy tối ưu hóa lúc này sẽ ko chạy chính xác nhiệm vụ.
 *
 * Trường hợp 2: Thêm từ khóa 'volatile' vào trong khai báo biến
 * Kết quả ta thấy dù có thiết lập mức tối ưu hóa nào thì trình biên dịch sẽ thực hiện các lệnh
 * của các biến có thiết lập từ khóa 'volatile'.
 * Dòng text từ file .elf được tạo giảm từ 820->764
 * */

#include <stdint.h>

int main(void)
{
	//Trường hợp 1: Khai báo biến không sử dụng volatile
//    uint32_t *pClkCtrlReg = (uint32_t*)0x40023830; 	//Khai báo cấu hình địa chỉ xung clock
//
//    uint32_t *pPortDmodeReg = (uint32_t*)0x40020C00; //Khai báo cấu hình các địa chỉ cần thiết thiết lập port D
//    uint32_t *pPortDoutReg = (uint32_t*)0x40020C14;  //thiết lập địa chỉ làm output port D
//
//    uint32_t *pPortAmodeReg = (uint32_t*)0x40020000 ;  //GPIOA ///Khai báo cấu hình các địa chỉ cần thiết thiết lập port A

//    uint32_t *pPortAInReg =   (uint32_t*)0x40020010 ;  //GPIOA 0x40020000 + 0x01(address offset)
    //Đây là địa chỉ cấu hình để thiết lập đọc trạng thái của các pin port A

    //Trường hợp 2: tối ưu hóa kết hợp từ khóa 'volatile'
    uint32_t volatile *pClkCtrlReg = (uint32_t*)0x40023830; 	//Khai báo cấu hình địa chỉ xung clock

    uint32_t volatile *pPortDmodeReg = (uint32_t*)0x40020C00; //Khai báo cấu hình các địa chỉ cần thiết thiết lập port D
    uint32_t volatile *pPortDoutReg = (uint32_t*)0x40020C14;  //thiết lập địa chỉ làm output port D

    uint32_t volatile *pPortAmodeReg = (uint32_t*)0x40020000 ;  //GPIOA ///Khai báo cấu hình các địa chỉ cần thiết thiết lập port A

    uint32_t volatile *pPortAInReg =   (uint32_t*)0x40020010 ;	//thiết lập địa chỉ làm input port D

    //II. Su dung phuong phap dich bit
    //1.Bat dong ho cho thiet bi ngoai vi GPIOD, GPOIA o AHB1ENR
    *pClkCtrlReg |= ( 1 << 3 );			//GPIOD
    *pClkCtrlReg |= ( 1 << 0 );			//GPIOA

    //2.Cau hinh che do chan IO lam dau ra
    //a.clear the bit 24 and 25  PD12 là output
    *pPortDmodeReg &= ~ ( 3 << 24 ) ;    //3 = 11 : set bit 24,25 ve 0
    //b. set bit 24 len 1
    *pPortDmodeReg |= ( 1 << 24 );		//dich bit cho chan 24 len 1

    //thiet lap PA0 input (GPIO MODE REGISTER)
    *pPortAmodeReg &= ~( 3 << 0);		//3 = 11 : set bit 0,11 về 00
    // vừa là trạng thái reset vừa là trạng thái input (xem datasheet phần GPIO port mode register)

    //Gợi ý: &(AND) : thường sẽ thay đổi tất cả trạng thái của tất cả bit
    //		 |(NOT) : chỉ thay đổi bit mà ta muốn thay đổi

    while(1)
    {
    	//doc trang thai chan PA0
    	uint8_t pinStatus = (uint8_t)(*pPortAInReg & 0x1);  //set tat ca bit khac la 0 tru bit 0 là 1
    	//lúc này việc set bit 0 lên 1 có nghĩa là yêu cầu chương trình đọc giá trị đang có tại chân PA01
    	//pPortAInReg : chứa địa chỉ mà ta đã cấu hình ban đầu là cho phép đọc chân pin của port A

		if(pinStatus)
		{
			*pPortDoutReg |= ( 1 << 12 );
		}
		else{
			*pPortDoutReg &= ~ ( 1 << 12 );
		}

    //3. Set chan 12 GPIO len 1 cho LED sang
    //*pPortDoutReg |= ( 1 << 12 );		//dich bit cho chan 12 len 1
    }
}

//Bài 4: Đọc trạng thái của Keypad
/*
 * S28: Keypad (Lưu ý hãy đọc từ bài LedOn trước khi đọc bài này)
 * Cách hoạt động: dựa trên giá trị 8bit (4bit hàng cột và 4 bit hàng ngang)
 * Khi phím nào đc nhấn thì giá trị hàng và cột liên quan là LOW=0,
 * Các vị trí khác vẫn giữ mức HIGH
 *
 * Tại sao sử dụng pull-up resistors(1==ko_nhấn==high -> 0==nhấn==low)(S28:Keypad -> V174)
 * Pull-up resistors (tạm dịch là tụt áp kéo lên) là các trở kháng được sử dụng trong các hệ thống nhúng để
 * đảm bảo tín hiệu đầu vào luôn có giá trị xác định, đặc biệt là trong trường hợp không có thiết bị nào
 * kết nối đến đầu vào đó hoặc các hiện tượng như lờn nút nhấn gây nhiễu(mấp mô) tín hiệu đầu vào.

 * Trong hệ thống nhúng, các chân đầu vào trên vi điều khiển thường được kết nối với các bộ cảm biến,
 * các công tắc hoặc các tín hiệu từ bên ngoài. Khi không có thiết bị nào kết nối với đầu vào đó,
 * tín hiệu trên chân đầu vào có thể trở thành giá trị không xác định hoặc dao động, gây ra các vấn đề
 * về độ tin cậy của hệ thống.

 * Để giải quyết vấn đề này, pull-up resistors được sử dụng để đảm bảo rằng đầu vào luôn có giá trị xác định khi
 * không có thiết bị nào kết nối đến. Pull-up resistors được kết nối từ đầu vào tới nguồn điện, đảm bảo rằng
 * tín hiệu đầu vào sẽ luôn có giá trị cao (hoặc gần giá trị cao) khi không có thiết bị nào kết nối đến.

 * Khi có thiết bị kết nối đến đầu vào, tín hiệu từ thiết bị đó sẽ có ưu tiên và đè lên tín hiệu từ pull-up resistor,
 * làm thay đổi giá trị đầu vào(có tín hiệu điều khiển xác định) của vi điều khiển.

 * Vì vậy, pull-up resistors là một phương pháp đơn giản và hiệu quả để đảm bảo tính ổn định và tin cậy của hệ thống nhúng
 * trong các trường hợp không có thiết bị nào kết nối đến đầu vào.
 *
 * Phân biệt:
 * Pull-up resistors(điện trở kéo lên): dây_tín_hiệu -> điện trở 22k -> VDD
 * Pull-down resistors(điện trở kéo xuống): dây_tín_hiệu -> điện trở 22k -> GND
 * */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include<stdint.h>
#include<stdio.h>
//Chống dội phím
void delay()
{
	for(uint32_t i = 0; i < 250000; i++);

	/*
	 * Tính toán thời gian delay
	 * Vi điều khiển STM32 sử dụng thạch anh có tần số 16Mhz
	 * 1 lệnh xử lý (instruction) = 1 xung clock
	 * 1 ins = 0.0625 micro seconds
	 * 1 chu trình xử lý vòng lặp for cần 7 lệnh: 7 ins = 0.5 micro seconds (7*0.625 ms)
	 *
	 * -> 0.5 micro seconds -> 1 vòng lặp
	 * -> 1000 micro seconds -> 2000 vòng lặp
	 *
	 * Để chống dội cần 150ms -> 150 * 2000 = 300000+- lần vòng lặp
	 * */
}

int main(void)
{
	//Định nghĩa, cấu hình địa chỉ của các thanh ghi ngoại vi lấy từ datasheet cho các biến đại diện
	uint32_t volatile *const pGPIODModeReg = (uint32_t*)(0x40020C00);	//8.4.1(GPIOx_MODER)
	uint32_t volatile *const pInPutDataReg = (uint32_t*)(0x40020C00 + 0x10);
	uint32_t volatile *const pOutPutDataReg = (uint32_t*)(0x40020C00 + 0x14);
	uint32_t volatile *const pClockCtrlReg = (uint32_t*)(0x40023800 + 0x30); //RCC_AHB1ENR
	uint32_t volatile *const pPullupDownReg = (uint32_t*)(0x40020C00 + 0x0C);  //8.4.4_GPIOx_PUPDR

	//1. Cấu hình xung clock cho thanh ghi ngoại vi PortD
	*pClockCtrlReg |= (1 << 3);

	//2. Cấu hình PD0,PD1,PD2,PD3 làm đầu ra output (rows)
	*pGPIODModeReg &= ~(0xFF);  //clear tất cả pin_cặp_bit về 00, phòng trường hợp thanh ghi đã đc sử dụng trước đó
	//ở đây ta cần set pin_cặp_bit từ Moder0->Moder3 = 01(set pull-up)
	*pGPIODModeReg |= 0x55; 	// 0101 0101 = 0x55, set các pin_cặp_bit len 01

	//3. Cấu hình PD8,PD9,PD10,PD11 làm đầu vào input (columns)
	////ở đây ta cần set pin_cặp_bit từ Moder8->Moder11 = 00
	*pGPIODModeReg &= ~(0xFF << 16);		//set value 00 bit từ vị trí bit 16

	//4. Set pull-up cho PD8,PD9,PD10,PD11
	//ở đây ta cần set pin_cặp_bit từ PUPDR8->PUPDR8 = 01(set pull-up)
	*pPullupDownReg &= ~(0xFF << 16);    //clear bit tù vị trí bit 16
	*pPullupDownReg |= (0x55 << 16);	// set các pin_cặp_bit len 01, cấu hình pull-up

	//Hỏi: tại sao sử dụng thêm thanh ghi GPIOx_PUPDR cho tín hiệu input
	//trong khi tín hiệu output không cần
	//Giải thích: khi GPIOx_MODER(thanh ghi của vi điều khiển) là output thì tín hiệu đầu ra
	//luôn luôn sẽ là 0 hoặc 1. Tuy nhiên, vì tín hiệu input có thể là một value không xác định,
	//bị nhiễu, hoặc dao động điện áp nên sẽ ko xác định đc value input là 0 hoặc 1. Chính vì thế
	//ta phải sử dụng thanh ghi có tích hợp pull-up resistor để lọc, khử nhiễu giá trị input.

	//Lưu ý: thanh ghi gốc đang xử lý là thanh ghi 32 bit GPIOx_MODER = 0x40020C00.
	//Các thanh ghi pPullupDownReg,pClockCtrlReg,.. là các thanh ghi bổ sung tính năng cho thanh ghi gốc
	//Vd: khi ta cấu hình input cho PD8,PD9,PD10,PD11 trên thanh ghi GPIOx_MODER, tuy nhiên vì muốn
	//lọc, khử nhiễu input nên ta sử dụng thêm thanh ghi pPullupDownReg để các chân PD8,PD9,PD10,PD11
	//có thêm khả năng pull-up.

	while(1)
	{
		//5.1 Thiet lap Rows len muc cao high
		*pOutPutDataReg |= 0x0F;

		//6.1 Thiet lap R1 LOW(PD0)
		*pOutPutDataReg &= ~(1 << 0);

		//7.1 Quet cac cot
		//Check C1(PD8) low or high
		if(!(*pInPutDataReg & (1 << 8)))
		{
			//key dc nhan
			delay();
			printf("1\n");
		}

		//Check C2(PD9) low or high
		if(!(*pInPutDataReg & (1 << 9)))
		{
			//key dc nhan
			delay();
			printf("2\n");
		}

		//Check C3(PD10) low or high
		if(!(*pInPutDataReg & (1 << 10)))
		{
			//key dc nhan
			delay();
			printf("3\n");
		}

		//Check C4(PD11) low or high
		if(!(*pInPutDataReg & (1 << 11)))
		{
			//key dc nhan
			delay();
			printf("A\n");
		}


		//5.2 Thiet lap Rows len muc cao high
		*pOutPutDataReg |= 0x0F;
		//6.2 Thiet lap R2 LOW(PD1)
		*pOutPutDataReg &= ~(1 << 1);

		//7.2 Quet cac cot
		//Check C1(PD8) low or high
		if(!(*pInPutDataReg & (1 << 8)))
		{
			//key dc nhan
			delay();
			printf("4\n");
		}

		//Check C2(PD9) low or high
		if(!(*pInPutDataReg & (1 << 9)))
		{
			//key dc nhan
			delay();
			printf("5\n");
		}

		//Check C3(PD10) low or high
		if(!(*pInPutDataReg & (1 << 10)))
		{
			//key dc nhan
			delay();
			printf("6\n");
		}

		//Check C4(PD11) low or high
		if(!(*pInPutDataReg & (1 << 11)))
		{
			//key dc nhan
			delay();
			printf("B\n");
		}



		//5.3 Thiet lap Rows len muc cao high
		*pOutPutDataReg |= 0x0F;
		//6.3 Thiet lap R3 LOW(PD2)
		*pOutPutDataReg &= ~(1 << 2);

		//7.3 Quet cac cot
		//Check C1(PD8) low or high
		if(!(*pInPutDataReg & (1 << 8)))
		{
			//key dc nhan
			delay();
			printf("7\n");
		}

		//Check C2(PD9) low or high
		if(!(*pInPutDataReg & (1 << 9)))
		{
			//key dc nhan
			delay();
			printf("8\n");
		}

		//Check C3(PD10) low or high
		if(!(*pInPutDataReg & (1 << 10)))
		{
			//key dc nhan
			delay();
			printf("9\n");
		}

		//Check C4(PD11) low or high
		if(!(*pInPutDataReg & (1 << 11)))
		{
			//key dc nhan
			delay();
			printf("C\n");
		}



		//5.4 Thiet lap Rows len muc cao high
		*pOutPutDataReg |= 0x0F;
		//6.4 Thiet lap R4 LOW(PD3)
		*pOutPutDataReg &= ~(1 << 3);

		//7.4 Quet cac cot
		//Check C1(PD8) low or high
		if(!(*pInPutDataReg & (1 << 8)))
		{
			//key dc nhan
			delay();
			printf("*\n");
		}

		//Check C2(PD9) low or high
		if(!(*pInPutDataReg & (1 << 9)))
		{
			//key dc nhan
			delay();
			printf("0\n");
		}

		//Check C3(PD10) low or high
		if(!(*pInPutDataReg & (1 << 10)))
		{
			//key dc nhan
			delay();
			printf("#\n");
		}

		//Check C4(PD11) low or high
		if(!(*pInPutDataReg & (1 << 11)))
		{
			//key dc nhan
			delay();
			printf("D\n");
		}
	}
}