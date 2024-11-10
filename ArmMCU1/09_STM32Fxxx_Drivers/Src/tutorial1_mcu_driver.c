/*
 * tutorial1_mcu_driver.c
 *
 *  Created on: Nov 13, 2023
 *      Author: Minh
 */
/**
*===Structuring peripheral register(S21)
+ Trong chương này ta sẽ tìm hiểu chi tiết về các địa chỉ của các thanh ghi cấu hình peripheral
*==Address of peripheral register(V83)
+ Sở đồ: 						PERIPH_BASE
APB1_PERIPH_BASE - APB2_PERIPH_BASE - AHB1_PERIPH_BASE - AHB2_PERIPH_BASE
											|
										  GPIOA
											|
(GPIOA of peripheral Register)MODE_Reg - OUTPUT_TYPE_reg - SPEED_Reg - PUPDR_Reg - IDR_Reg -  ODR_Reg -   BSRR_Reg
+ Địa chỉ cở sở: 	Offset:		0x00 	   		0x04 		  0x08 		  0x0C 		0x10 	   0x14 	  0x18
							0x40020000 		0x40020004 	   0x40020008  0x4002000C  0x40020010 0x40020014  0x40020018
+ Các thanh ghi và địa chỉ cơ sở tương ứng ở trên sẽ dùng để cấu hình điều khiển GPIOA

*==Structuring peripheral register details(chi tiết)(V84)
+ Ta có thể dùng cách "macro" các địa chỉ cơ sở của các register trong header file, tuy nhiên cách này ít đc sử dụng
vì ta có rất nhiều peripheral device và mỗi peripheral lại có nhiều thanh ghi cần định địa chỉ cơ sở bằng "macro"
nên cách này rất dài và lâu
+ Vì vậy ta sẽ dùng phương pháp là sử dụng một "Struct-cấu trúc" để quản lý các register của một peripheral device
 ^Vd: Thiết lập "Struct" quản lý các register cho một peripheral GPIOx
  ~B1: Tiến hành code một Struct chung cho các GPIOx
	typedef struct
	{
		uint32_t MODERR;   	//Address offset: 0x00
		uint32_t OTYPER;	//Address offset: 0x04
		uint32_t OSPEEDR;	//Address offset: 0x08
		uint32_t PUPDR;		//Address offset: 0x0C
		uint32_t IDR;		//Address offset: 0x10
		uint32_t ODR;		//Address offset: 0x14
		uint32_t BSRRL;		//Address offset: 0x18
		uint32_t BSRRH;		//Address offset: 0x1A
		uint32_t LCKR;		//Address offset: 0x1C
		uint32_t AFR[2];	//Address offset: 0x20-0x24
	}GPIO_RegDef_t;

	Lưu ý: trong struct ko cần phải set byte cho từng thành phần thanh ghi vì mỗi biến khai báo sẽ đều đi với
	kiểu biến của nó và trình biên dịch sẽ dựa vào kiểu biến để xác định dung lượng của biến
	vd: ta khai báo uint32_t = int 32bit = 4byte, có nghĩa là địa chỉ biến MODER cách địa chỉ biến OTYPER là 4byte
	vừa đúng với phần bù địa chỉ các thanh ghi của các GPIOx

  ~B2_C1: Định địa chỉ cơ sở cho Struct thuộc GPIOx mong muốn (có thể gọi Struct chung này cho nhiều GPIO khác)
   GPIO_RegDef_t *pGPIOA = (GPIO_RegDef_t *)0x40020000;	//Định địa chỉ là GPIOA base address

   Lưu ý: Lưu một value(cấu hình thanh ghi) vào bên trong thanh ghi mong muốn của một Struct đã xác định GPIOx
   pGPIOA->MODER = 25;  //tại địa chỉ thanh ghi MODER sẽ lưu giá trị là 25 = 0001 1001
   //*(0x400020000 + 0x00) = 25
   pGPIOA->ODR = 44;	//tại địa chỉ thanh ghi ODR sẽ lưu giá trị là 44 = 0100 0100

  ~B2_C2(phối hợp sử dụng địa chỉ GPIO đã đc định danh trong header file):
   #define GPIOA 	((GPIO_RegDef_t*)GPIO_BASE)      //code định nghĩa trong header file
   GPIO_RegDef_t *pGPIOA = GPIOA

+ Tài liệu tham khảo: RM407-> C8:General-purposor I/Os(GPIO) -> 8.4 GPIO Register -> Table39
 ^Ở bảng Table39 ta sẽ quan sát đc các thanh ghi sẽ có trong một GPIOx
+ Ta sẽ sử dụng từ khóa "volatile" để tránh bị tối ưu hóa khi biên dịch vì các biến này có thể sẽ thay đổi value và
chương trình cần đọc chúng để điều khiển chương trình theo code lập trình

*==Periperal definition macros(V85)
+ Định địa chỉ cơ sở cho GPIOx mong muốn cấu hình điều khiển bằng pp macros
+ Sau khi định nghĩa define thì ta có các GPIOx là các con trỏ kiểu GPIO_RegDef_t với địa chỉ GPIOx_BASEADDR
-> Có nghĩa là với các define GPIOx thì ta có thể truy cập cấu hình điều khiển các thanh ghi của GPIOx một cách nhanh chóng
*/

/**
*===Writing Periperal Clock enable and disable macros(S22)
Trong chương này ta sẽ tiến hành định nghĩa cho thanh ghi quản lý ngoại vi Xung Clock RCC

*==Writing Periperal Clock Enable and Disable C Masros(V86)
+ Tài liệu: RM407 -> C7 -> 7.3 RCC register -> 7.3.24 RCC Register Map
 ^Ta sẽ dựa vào bảng này(lấy các tên viết tắt và địa chỉ offset) để thiết lập các thành phần của Struct đc code
 trong file header file
 ^Các thành phần này là các "Thanh ghi" để cấu hình và điều khiển Xung Clock and Reset RCC
 ^Việc khai báo các thành phần "Reserved" trong struct giúp cho ta sắp xếp đc chính xác các địa chỉ phần bù của các
 thanh ghi khác của RCC(Các địa chỉ thanh ghi thường cách nhau 4 byte, và việc khai báo kiểu int(4byte) giúp cho struct
 tự động sắp xếp đc địa chỉ các thanh ghi từ đó việc gọi thực hiện điều khiển sẽ dễ dàng hơn )
 ^Xác định địa chỉ cơ sở của RCC Register(Table1.STM32F4xx : đc treo với AHB1 Bus)

+ Tiến hành thiết lập việc "CLock Enable" dành riêng cho các GPIOx Periperals dựa trên struct general RCC đã đc viết
 ^Tài liệu: RM407 -> C7 -> 7.3 RCC register ->7.3.10 RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)
 ^Tài liệu giúp việc xác định GPIOx nào ở bit số mấy cần kích hoạt enable(lên 1)

+ Tiến hành thiết lập việc "CLock Enable" dành riêng cho các I2Cx Periperals dựa trên struct general RCC đã đc viết
 ^Tài liệu: RM407 -> C7 -> 7.3 RCC register ->7.3.13 RCC APB1 peripheral clock enable register (RCC_APB1ENR)
 ^Tài liệu giúp việc xác định I2Cx nào ở bit số mấy cần kích hoạt enable(lên 1)

+ Tiến hành thiết lập việc "CLock Enable" dành riêng cho các SPIx Periperals dựa trên struct general RCC đã đc viết
 ^Tài liệu: RM407 -> C7 -> 7.3 RCC register ->7.3.13  (RCC_APB1ENR-> SPI2,3) + 7.3.14  (RCC_APB2ENR-> SPI1)
 ^Tài liệu giúp việc xác định SPIx nào ở bit số mấy cần kích hoạt enable(lên 1)
+ ....Làm tương tự khi muốn kích hoạt "Clock Enable" cho bất kỳ ngoại vi nào

+ Tiến hành thiết lập việc "CLock Disable" dành riêng cho các GPIOx Periperals dựa trên struct general RCC đã đc viết
  ...

*==Project include settings and Build(V87)
+ Thiết lập một file main.c
+ Lưu ý: khi thiết lập một file thư viện bên ngoại cần xác định đường dẫn(path) cho nó bằng settings properties
*/
