/*
 * tutorial_mcu_driver.c
 *
 *  Created on: Nov 10, 2023
 *      Author: Minh
 */
/**
*===GPIO driver development overview and Project creation(S19)
*==GPIO driver development overview(V72)
+ Sơ đồ:
 SAMPLE APPLICATION --> Driver Layer((gpio_driver.c,.h); (i2c_driver.c,.h);(uart_driver.c,.h),...) --> STM32407_MCU
										((spi_driver.c,.h); (stm32fx_driver.h)

+ STM32407_MCU: đây là nơi chứa các phần cứng ngoại vi mà ta sẽ lập trình điều khiển như GPIO,I2C,...
+ Driver Layer: là phần tập hợp các tệp .c,.h khác nhau. Chúng ta sẽ lập trình cho các file lập trình điều khiển các
ngoại vi như là gpio,i2c,uart,....Theo sơ đồ ta sẽ có mỗi ngoại vi sẽ có các file driver .c,.h riêng. Có nghĩa là thay
vì code chung thì ta sẽ code riêng từng ngoại vi vđk sau đó khi nào sử dụng thì ta sẽ tiến hành gọi các file riêng đó.
Ngoài ra, với mỗi MCU mà sẽ làm việc thì cũng cần viết riêng cho nó một driver gọi là "Device Headers file".
+ SAMPLE APPLICATION: đây chính là nơi lập trình chung để gọi các driver đã viết để xử lý chúng.

+ Drive layer như là các APIs dùng để kết nối phần cứng(backend_STM32407_MCU) với phần mềm điều khiển(fontend_APPLICATION)
+ Thứ tự của việc lập trình một ngoại vi vđk:
 1. Viết file stm32fx_driver.h, cấu hình các chức năng cần thiết và chi tiết cho vi điều khiển hoạt động
 2. Viết các file driver cho từng ngoại vi muốn sử dụng trong chương trình
 3. Viết file ứng dụng, kết hợp và điều khiển các ngoại vi bằng cách gọi kiểm tra hoạt động các file driver ngoại vi đã viết

*==MCU Device Headers File(V73)
+ Đây là một Header file (file .h) trong Lập trình C. Nơi sẽ chứa các chi tiết cụ thể của vđk muốn sử dụng
+ Ví dụ: chứa các địa chỉ cơ sở của các Memory đang có trong vđk như là SRAM,FLASH,ROM,...
+ Chứa các địa chỉ cơ sở của các Bus kết nối như APB Bus,AHB Bus
+ Chứa các địa chỉ cơ sở của các ngoại vi mà đang kết nối với các Bus khác nhau
+ Cấu hình xung Clock
+ Chứa các định nghĩa liên quan đến Interrupt(IRQx)
+ Chứa các cấu trúc(Struct) mô tả thanh ghi Ngoại vi và đồng thời định dạng "số bit" cho các phần ngoại vi đó
+ Sau khi đã có các file header thì lúc sử dụng chúng trong các file khác thì ta có từ khóa: #include"tên_file_header.h"

*==New Project Creation and MCU Header File(V74)
+ Tạo một file Project
+ Tạo một file folder "drivers". Trong folder "drivers" tạo 2 thư mục khác là "Inc" và "Src"
 ^"Inc" là thư mục dành riêng dùng để chứa các "header" của các file cấu hình MCU_Stm32F4xx
 ^"Src" là thư mục dành riêng dùng để chứa các "source" của các file cấu hình MCU_Stm32F4xx
 ->Lưu ý: đây là 2 thư mục chứa các "header" và "source" viết riêng để cấu hình cho chip MCU mà ta đang sử dụng trong dự án
 chứ đây ko phải là "header" và "source" viết cho Project chính đang làm.

 ^Tiến hành thực hiện việc "Include-thêm vào" folder "drivers" này khi chạy chương trình, có nghĩa là thông báo cho trình biên
 dịch biết đây là một trong các "Source" để biên dịch chương trình. (Nó là file hỗ trợ chứa các thư viện hỗ trợ cho việc viết
 code chương trình chính, project chính nên cần include khi build)
  ~Click phải vào folder "drivers" -> properties -> C/C++ Build -> Bỏ tích "Exclude(loại bỏ) resource from build"

+ Thao tác với folder "driver->Inc"
 ^Tạo một file "stm32f407xx.h" (Nên đặt theo tên theo chip hoặc board đang sử dụng để code project)
  ->Ta đã có một file để chứa các thông tin chi tiết về vđk đang dùng cho dự án như địa chỉ cơ sở của các thành phần quan trọng
  như Memory, Bus Domain, Thanh ghi ngoại vi(Periperal Register) đc hỗ trợ trong chip,...
  ->Các cấu hình này trong file này có thể gọi là "C Macros"

*==Include pass Setting(V75)
+ Bài này hướng dẫn cách thiết lập các file ".h" đã đc code vào trong file code project theo path-đường dẫn đúng và chuẩn nhất
+ vd: nếu ta thêm dòng [include "stm32f407xx.h"] vào trực tiếp file main.c, thì khi build thì trình biên dịch sẽ báo lỗi vì ta
chưa include đường dẫn file "stm32f407xx.h" đúng cách
+ Nếu dùng path : include "driver/Inc/stm32f407xx.h" thì quá rắc rối và ko chuyên
+ Cách đúng: Ta sẽ setting path để project có thể tự động biết tất cả các file .h thêm vào đều có đúng path hay không
 ~Click chuột phải vào tên project chính(vd: 09_STM32Fxx_Driver) -> Properties -> C/C++ Build -> Settings -> MCU GCC Compiler
 -> Include paths -> Click add(+ màu xanh) -> Click Workpace -> Chọn path mong muốn(Lưu ý chỉ chọn tới thư mục chưa các file
 header mong muốn add vào project) -> OK (2 lần) -> Apply and Close -> Kiểm tra lại
*/

/**
*===Updating MCU specific header file with bus domain and peripheral details(S20)
Trong chương này sẽ tiến hành file stm32f407x.h, thiết lập các chức năng của chip cần thiết cho chương trình main
Lưu ý: vì thiết kế khác nhau nên tùy từng loại chip, loại board mà ta sẽ có nội dung file stm32f407x.h khác nhau
==>Vì vậy việc nắm và hiểu đc file datasheet là rất quan trọng

*==Writing base address C macros for MCU embedded memory part1+part2(V77+V78)
+ Định nghĩa các địa chỉ cơ sở của các Memory
+ Lưu ý: khi định địa chỉ lấy từ datasheet thì ta cần chuyển đổi các giá trị địa chỉ đó về các giá trị số nguyên
mà trình biên dịch có thể hiểu đc.
 ^Vd: 0x0800_0000 --sau khi đổi---> 0x0800_0000U hoặc (uint32_t)0x0800_0000

+ Cách tính địa chỉ SRAM2: ta có SRAM(SRAM1+SRAM2) và dung lượng của SRAM1 là 112KB, suy ta vị trí từ
0x20000000U + 111kb là vị trí cuối của SRAM1(Lưu ý: 1kB = 1000B)
->Vị trí của SRAM2 là 112*1024 = 114,688(Dec) = 1C000(Hex) -> Địa chỉ: 0x20000000U + 1C000 = 0x2001C000
 ^Mẹo: sử dụng phần mềm máy tính để có thể đổi đc các dạng số Nhị-Bát-Thập-ThậpLục
 ^Xem chi tiết bảng Table 3. Memory mapping

*==Defining base addresses of different bus domains - Xác định địa chỉ cơ sở của các Bus khác nhau(V79)
+ PERIPH_Base gồm: APB1_PERIPH_BASE, APB2_PERIPH_BASE, AHB1_PERIPH_BASE,AHB2_PERIPH_BASE
 ^Các ngoại vi khác nhau sẽ đc kết nối(hanging-treo) với các Bus khác nhau
 ^AHB Bus đc sử dụng cho những peripheral device cần giao tiếp dữ liệu cao như Camera interface, GPIOs
 ^APB Bus đc sử dụng cho những peripheral device cần giao tiếp dữ liệu low speed
 ^Theo Memory map thì: PERIPH_Base có địa chỉ cơ sở là 0x4000_0000(tương đương APB1_PERIPH_BASE)

 ^Tài liệu: Figure 5. STM32F40xxx block diagram (datasheet_stm32f407.pdf)

+ Theo datasheet RM407(Table1.STM32F4xx) ta sẽ có đc địa chỉ cơ sở của các Bus khác nhau
+ Cách Phân tích một địa chỉ ngoại vi
 ^Table1.STM32F4xx: Tại địa chỉ 0x4000_0000 là ngoại vi TIM2
 ^Tiến hành tìm đến thanh ghi ngoại vi TIM2(Chương 18)
 ^Di chuyển đến TIM register map, tại đây ta có thể quan sát đc địa chỉ các chức năng cần thiết để kích hoạt các TIMx
  ~Vd1: với offset = 0x00 ta sẽ cấu hình đc thanh ghi TIMx_CR1
  ~Vd2: với offset = 0x04 ta sẽ cấu hình đc thanh ghi TIMx_CR2
  => Tại địa chỉ 0x4000_0000 ta sẽ đc cấu hình đc thanh ghi TIM2_CR1 của ngoại vi TIM2 đc kết nối với APB1_PERIPH_BASE

+ Địa chỉ cơ sở của các Bus trong STM32F407xx
 ^PERIPH_BASE và APB1_PERIPH_BASE(Address: 0x4000_0000)
 ^APB2_PERIPH_BASE(Offset: 0x00010000 -> Address: 0x40010000)
 ^AHB1_PERIPH_BASE(Offset: 0x00020000 -> Address: 0x40020000)
 ^AHB2_PERIPH_BASE(Offset: 0x10000000 -> Address: 0x50000000)
+ Tiến hành macros(define) base address của các Bus vào file stm32f407x.h

*==Defining base address of AHB1 Periperals(V80)
+ Mỗi Periperal Device sẽ đc kết nối(treo-hanging) với mỗi Bus khác nhau, vì vậy tùy vào ngoại vi muốn sử dụng thì ta cũng
phải xác định ngoại vi đó đang đc kết nối với Bus nào
+ Trong STM32407_MCU thì AHB1 Bus sẽ là Bus Domain của các GPIOx(Từ A->I)
+ Trong video này ta sẽ xác định địa chỉ cơ sở của các peripheral đang kết nối với AHB1 bus
+ Sử dụng datasheet phần Memory Map

*==Defining base address of APB1 and APB2 Periperals(V81)
+ Thiết lập các địa chỉ cơ sở của các peripheral mà treo với các APB1 and APB2 Bus
+ Trên APB1 Bus thì các ngoại vi đc treo là: I2Cx(1,2,3), SPIx(2,3), USARTx(2,3,4,5)
+ Trên APB2 Bus thì các ngoại vi đc treo là: SPIx(1), USARTx(1,6), EXTI, SYSCFG

*==Defining base address conclusion(Kết luận)(V82
 Mẹo: Cách viết để Phân biệt các biến macro để tránh nhầm lẫn trong quá trình viết code
 Ta có thể viết là
  + 	 #define HAL_FLASH_BASEADDR		0x08000000U
  + hoặc #define DRV_FLASH_BASEADDR		0x08000000U
 HAL hoặc DRV : có nghĩa là biến macro này đang thuộc "HAL layer header file" hoặc "Driver layer header file",
 thường đc dùng để ký hiệu phân biết biến sử dụng đang thuộc file lập trình nào,thuộc lớp nào, nằm ở đâu
 -> Tùy vào lập trình viên hay đội nhóm mà sẽ có các ký hiệu chung để phân biệt các biến macro này
*/
