/********--- MCU2 ---*********/
/**-I-- STM32CubeMx ---*/
/**-II-- STM32 HAL and Project Architecture(S6) ---*/
/**-III-- STM32 HAL Program Flow with UART Exercise(S7) ---*/
/**-IV-- Clock and PLL Programing(S8) ---*/


/**-I-- STM32CubeMx ---*/
///ST Link Driver:
//Nếu bạn có một máy tính mới thì cần cài đặt ST Link Driver để nạp code và Gỡ lỗi

//Software: ST-Link/V2-1 firmware upgrade

//Khi tạo file STM32F4446RE Project thì khi được hỏi
//Initialize all peripheral with their default Mode? -> Select "No"
//Device Configuration Tool Editor? -> Select "Yes"

///V13: Học cách cấu hình USART2 sử dụng STM32Cube

///Pass ST: Lumia@851891

//File: stm32f4xx_hal_msp.c: 
//Thiết lập GPIO Alternate để cấu hình chân x của Peripheral Device
//File CubeMx sẽ sử dụng cơ chế "__weak" để ghi đè hàm

/* Trong lập trình vi điều khiển STM32F4, từ khóa assert_param đóng vai trò như một cơ 
chế kiểm tra và báo lỗi quan trọng. Nó được sử dụng để kiểm tra tính hợp lệ của các tham 
số truyền vào các hàm thư viện của STM32. Nếu một tham số nào đó không nằm trong phạm vi 
hợp lệ, hàm assert_param sẽ được gọi, dẫn đến việc chương trình dừng lại hoặc thực hiện 
một hành động xử lý lỗi khác.

^Cách hoạt động của assert_param:
 +Kiểm tra tham số: Hàm assert_param sẽ kiểm tra một điều kiện (thường là một biểu thức điều kiện) và nếu điều kiện 
 đó sai (false), nó sẽ thực hiện một hành động cụ thể, thường là dừng chương trình.
 +Thông báo lỗi: Trong một số trường hợp, hàm có thể cung cấp thông báo lỗi để giúp lập trình viên xác định nguyên 
 nhân của sự cố.
 +Chế độ phát triển: Hàm này thường được sử dụng trong chế độ phát triển và có thể được tắt trong chế độ sản xuất 
 để giảm kích thước mã và tăng hiệu suất.

^ Ví dụ đơn giản về cách sử dụng: 
#include <stdio.h>
#define assert_param(expr) ((expr) ? (void)0 : assert_failed(__FILE__, __LINE__))
void assert_failed(const char* file, int line) {
    // Xử lý lỗi, có thể in ra thông báo hoặc dừng chương trình
    printf("Assertion failed in file %s on line %d\n", file, line);
    while (1); // Dừng chương trình
}
void example_function(int param) {
    // Kiểm tra tham số
    assert_param(param > 0); // Kiểm tra xem param có lớn hơn 0 không

    // Tiến hành thực hiện các thao tác khác
    printf("Parameter is valid: %d\n", param);
}

int main() {
    example_function(5);  // Tham số hợp lệ
    example_function(-1); // Tham số không hợp lệ, sẽ kích hoạt assert_failed
    return 0;
}
 +Giải thích mã:
  -Macro assert_param: Đây là một macro kiểm tra điều kiện. Nếu điều kiện không đúng, nó sẽ gọi hàm assert_failed 
  với tên tệp và số dòng nơi lỗi xảy ra.
  -Hàm assert_failed: Hàm này sẽ xử lý lỗi, trong ví dụ này, nó chỉ in ra thông báo và dừng chương trình.
  -Hàm example_function: Hàm này kiểm tra tham số đầu vào bằng cách sử dụng assert_param. Nếu tham số không hợp lệ, 
  chương trình sẽ dừng lại.
*/

/**-II-- STM32 HAL and Project Architecture ---*/
/*Các Layer trong một Project sử dụng thư viện HAL(Hardware Abstraction Layer)
Sơ đồ hoạt động cơ bản chạy từ trên xuống dưới: 
	STM32Cube Layer(STM32Fx_HAL_Layer)
			|			|
	CMSIS Core Layer	|
			|			|
		MicroController Layer

1.STM32Cube Layer(STM32Fx_HAL_Layer): cung cấp các API Peripheral để giao tiếp, điều khiển các
thiết bị ngoại của Microcontroller STM32F4. Một số thư mục quan trọng:
 ^Include:
  -stm32f4xx_hal.h: File header chính cho HAL của STM32F4xx. Nó bao gồm các định nghĩa cho cấu trúc dữ liệu,
 macro, và hàm API cho tất cả các thiết bị ngoại vi.
  -stm32f4xx_hal_conf.h: File cấu hình cho HAL, cho phép bạn tùy chỉnh các cài đặt mặc định.
  -stm32f4xx_hal_exti.h: File header cho lớp HAL cho ngắt ngoại vi (EXTI).
  -stm32f4xx_hal_rcc.h: File header cho lớp HAL cho đồng hồ (RCC).
  -stm32f4xx_hal_rtc.h: File header cho lớp HAL cho đồng hồ thời gian thực (RTC).
  -stm32f4xx_hal_gpio.h: File header cho lớp HAL cho chân GPIO.
  ...và nhiều file header khác cho các thiết bị ngoại vi khác.
 
 ^Src:
 stm32f4xx_hal.c: File nguồn chứa các hàm thực thi cho HAL của STM32F4xx.
 stm32f4xx_hal_exti.c: File nguồn chứa các hàm thực thi cho lớp HAL cho ngắt ngoại vi (EXTI).
 stm32f4xx_hal_rcc.c: File nguồn chứa các hàm thực thi cho lớp HAL cho đồng hồ (RCC).
 stm32f4xx_hal_rtc.c: File nguồn chứa các hàm thực thi cho lớp HAL cho đồng hồ thời gian thực (RTC).
 stm32f4xx_hal_gpio.c: File nguồn chứa các hàm thực thi cho lớp HAL cho chân GPIO.
 ...và nhiều file nguồn khác cho các thiết bị ngoại vi khác.

 ^Chức năng:
  -Trừu tượng hóa phần cứng: HAL cung cấp các hàm API dễ sử dụng để điều khiển các thiết bị ngoại vi, 
  cho phép bạn sử dụng các chức năng của vi điều khiển STM32F4xx mà không cần phải hiểu rõ kiến trúc phần cứng.
 -Cấu hình thiết bị ngoại vi: HAL cung cấp các hàm để cấu hình các thiết bị ngoại vi, bao gồm đồng hồ, ngắt, 
 chân GPIO, và nhiều thiết bị ngoại vi khác.
 -Xử lý ngắt: HAL xử lý các ngắt từ các thiết bị ngoại vi và cho phép bạn định nghĩa hàm xử lý ngắt.
 -Kiểm soát quyền truy cập: HAL kiểm soát quyền truy cập vào các tài nguyên phần cứng, giúp đảm bảo tính ổn định 
 và an toàn của hệ thống.
 
2.CMSIS(Cortex Microcontroller Software Interface Standard) Core Layer: 
là một tập hợp các tiêu chuẩn và thư viện phần mềm được thiết kế để đơn giản hóa việc phát triển phần mềm nhúng 
cho các vi điều khiển ARM Cortex.
Nội dung chi tiết của các thư mục và file trong Drivers -> CMSIS
 ^Device: ARM - Chứa các file cấu hình cho vi điều khiển ARM Cortex-M, bao gồm:
 system_stm32f4xx.c: File nguồn chứa hàm khởi tạo hệ thống cho STM32F4xx.
 startup_stm32f4xx.s: File assembly chứa vector ngắt và mã khởi động cho STM32F4xx.
 Include: Chứa các file header cho các thành phần CMSIS, bao gồm:
 core_cm4.h: File header cho kiến trúc ARM Cortex-M4.
 core_cmInstr.h: File header cho các lệnh của ARM Cortex-M4.
 core_cmFunc.h: File header cho các hàm intrinsics của ARM Cortex-M4.
 cmsis_gcc.h: File header cho trình biên dịch GCC.
 cmsis_compiler.h: File header cho các macro và hàm liên quan đến trình biên dịch.
 cmsis_version.h: File header chứa thông tin phiên bản CMSIS.
 Ngoài ra: một số file xử lý liên quan đến NVIC, SysTick, Memory Protection Unit,...

 ^RTOS:
 RTX: Chứa các file liên quan đến hệ điều hành thời gian thực (RTOS) RTX.
 RTX_Config.c: File cấu hình cho RTX.
 RTX_Conf_CM.c: File cấu hình cho RTX trên ARM Cortex-M.
 RTX_CM.c: File nguồn của RTX cho ARM Cortex-M.
 Include: Chứa các file header cho RTX, bao gồm:
 cmsis_os.h: File header cho RTOS CMSIS.
 cmsis_os2.h: File header cho RTOS CMSIS phiên bản 2.
 
 ^Include:
 cmsis_armcc.h: File header cho trình biên dịch ARM Compiler.
 cmsis_compiler.h: File header cho các macro và hàm liên quan đến trình biên dịch.
 
 ^Source: system_stm32f4xx.c: File nguồn chứa hàm khởi tạo hệ thống cho STM32F4xx.
 ^startup: startup_stm32f4xx.s: File assembly chứa vector ngắt và mã khởi động cho STM32F4xx.
 ^Chức năng:
  -Khởi tạo hệ thống: File system_stm32f4xx.c chứa các hàm khởi tạo hệ thống cho STM32F4xx, bao gồm đồng hồ, 
  bộ nhớ, ngắt, và các thiết bị ngoại vi.
  -Vector ngắt: File startup_stm32f4xx.s chứa vector ngắt, xác định địa chỉ của các hàm xử lý ngắt cho STM32F4xx.
  -Cấu hình ARM Cortex-M: Các file header trong thư mục Include cung cấp các định nghĩa, macro, và hàm liên quan 
  đến kiến trúc ARM Cortex-M4, giúp đơn giản hóa việc phát triển phần mềm nhúng.
  -RTX: Thư mục RTOS -> RTX cung cấp các file nguồn và header cho RTX, giúp bạn sử dụng hệ điều hành thời gian 
  thực RTX trên STM32F4xx
 
3.MicroController Layer: chứa các "peripheral register" của Microcontroller. Sử dụng dụng các API ở Layer 1 và 2
để cấu hình thanh ghi lớp này, hướng dẫn cho Microcontroller hoạt động.
Nói cách khác đây chính là lớp phần cứng và do Microcontroller quản lý
Lưu ý: Lớp "STM32Cube Layer" có thể giao tiếp trực tiếp Microcontroller Layer mà ko cần qua lớp CMSIS Core Layer
nếu như không sử dụng các hoạt động đặc biêt như các API liên quan đến ngắt hoặc systick.
*/

/* Hướng dẫn sử dụng STM32CubeMx tạo Project
Quy trình: Mở STM32CubeMx -> chọn board tạo project -> Hỏi "Initialize all peripheral with default mode?"
-> Chọn "No" vì trong dự án cần custom lại các peripheral sử dụng(nếu ko cần custom thì chọn "Yes")
-> Hiện ra giao diện STM32CubeMxIDE -> Tiến hành Save Project vào thư mục Workbench mong muốn
-> Vào mục "Project Manager" -> Viết tên Project -> Chọn vị trí lưu project 
-> Mục "Toolchain/IDE" chọn STM32CubeIDE hoặc chọn "SW4STM32" nếu phần mềm IDE là OpenSTM32 
-> Kiểm tra version firmware đang sử dụng -> Chọn các peripheral muốn sử dụng trong dự án
-> Chọn "Generate Code"
*/

/* Project Hierarchy - Hệ thống phân cấp dự án
Quan sát các thư mục đc tạo ra sau khi sử dụng STM32CubeMx để tạo Project
+ Drivers:
  Drivers-> STM32F4xx_HAL_Driver == STM32Cube Layer(STM32Fx_HAL_Layer)
  Drivers -> CMSIS == CMSIS Core Layer
+ Inc: File Thư viện(vd: main.h) của Project - Thường do user lập trình
+ Src: File mã nguồn hoạt động Project (vd: main.c)
+ Startup: chứa mã nguồn liên quan đến điểm khởi đầu của chương trình khi vi điều khiển được bật nguồn.
  ^Chức năng chính của file startup_stm32f4xx.s:  File này được viết bằng ngôn ngữ assembly
   -Khởi tạo vector ngắt (Interrupt Vector): Vector ngắt là một bảng chứa địa chỉ của các hàm xử lý ngắt. 
  File khởi động sẽ thiết lập vector ngắt phù hợp cho từng loại ngắt của vi điều khiển.
   -Khởi tạo bộ nhớ: Gán vùng nhớ cho các thành phần khác nhau như stack, heap và data.
   -Khởi tạo các thiết bị ngoại vi: Thực hiện các thao tác khởi tạo cơ bản cho các thiết bị ngoại vi như 
   đồng hồ (RCC), GPIO, v.v.
   -Gọi hàm main(): Sau khi hoàn tất quá trình khởi động, file khởi động sẽ chuyển giao quyền điều khiển 
   cho hàm main() trong file main.c của bạn.
  
  ^Nội dung của file startup_stm32f4xx.s: File này thường chứa các đoạn code assembly sau:
  Kiểm tra lỗi khởi động: Kiểm tra xem có lỗi xảy ra trong quá trình khởi động hay không.
  Thiết lập stack pointer: Thiết lập con trỏ stack (stack pointer) đến vùng nhớ stack.
  Thiết lập vector ngắt: Thiết lập vector ngắt cho từng ngắt của vi điều khiển.
  Khởi tạo các biến toàn cục: Khởi tạo giá trị ban đầu cho các biến toàn cục (global) trong code C của bạn.
  Gọi hàm main(): Chuyển giao quyền điều khiển cho hàm main().

  ^Lưu ý: ~Bạn thường không cần chỉnh sửa file startup_stm32f4xx.s trừ khi bạn cần thay đổi cấu hình vector ngắt 
  hoặc thực hiện các thao tác đặc biệt trong quá trình khởi động.
  ~STM32CubeMX sẽ tự động tạo file startup_stm32f4xx.s phù hợp với cấu hình bạn lựa chọn trong phần "System Core"
  của phần mềm.
  ~Hiểu rõ chức năng của file khởi động giúp bạn nắm vững quá trình khởi động của vi điều khiển và phát triển 
  phần mềm nhúng hiệu quả hơn.
*/
/* Project Layers Interation - Tương tác với các lớp dự án
"Project Layers Interaction" trong hệ thống nhúng STM32 đề cập đến cách thức các lớp (layer) khác nhau trong một 
dự án phần mềm nhúng tương tác với nhau. Các lớp này được thiết kế để tách biệt các chức năng và trách nhiệm khác 
nhau trong dự án, giúp cho việc phát triển, bảo trì và gỡ lỗi trở nên dễ dàng hơn.
Dưới đây là một số lớp phổ biến trong hệ thống nhúng STM32 và cách chúng tương tác với nhau:
1. Lớp Vi điều khiển (Microcontroller Layer):
 Chức năng: Xử lý các hoạt động cơ bản của vi điều khiển, bao gồm:
 Khởi tạo đồng hồ (RCC)
 Cấu hình bộ nhớ
 Xử lý ngắt (NVIC)
 Quản lý năng lượng
 Tương tác: Lớp này là nền tảng cơ bản cho các lớp khác, nó cung cấp các hàm API để cấu hình và kiểm soát các chức
năng cốt lõi của vi điều khiển.

2. Lớp Trừu tượng hóa Phần cứng (Hardware Abstraction Layer - HAL):
 Chức năng: Cung cấp các hàm API dễ sử dụng để điều khiển các thiết bị ngoại vi của STM32.
 Các hàm API được thiết kế để trừu tượng hóa phần cứng, giúp cho việc viết code độc lập với kiến trúc cụ thể của chip.
 Tương tác: HAL tương tác với Lớp Vi điều khiển để truy cập và cấu hình các thiết bị ngoại vi. Nó cũng cung cấp các 
hàm API cho các lớp ứng dụng để tương tác với phần cứng.

3. Lớp Trình điều khiển (Driver Layer):
 Chức năng: Xử lý các hoạt động cụ thể của thiết bị ngoại vi.
 Mỗi driver quản lý một thiết bị cụ thể, chẳng hạn như sensor, màn hình LCD, bộ nhớ flash, v.v.
 Tương tác: Các driver tương tác với HAL để cấu hình và điều khiển thiết bị ngoại vi.
 
4. Lớp Ứng dụng (Application Layer):
 Chức năng: Thực hiện các chức năng chính của dự án.
 Ví dụ: thu thập dữ liệu từ sensor, điều khiển động cơ, xử lý thông tin, giao tiếp với người dùng, v.v.
 Tương tác: Lớp ứng dụng tương tác với HAL và các driver để truy cập và điều khiển phần cứng, đồng thời sử dụng 
 các hàm API được cung cấp bởi HAL.
 
5. Cách thức tương tác:
 Sự trừu tượng hóa: Các lớp cao hơn (như lớp ứng dụng) không cần biết cách thức hoạt động chi tiết của phần cứng. 
Thay vào đó, chúng tương tác với các lớp thấp hơn (như HAL và driver) thông qua các hàm API được cung cấp.
 Kết nối: Các lớp được kết nối với nhau thông qua các hàm API và dữ liệu được truyền qua các tham số hàm hoặc các 
cấu trúc dữ liệu được chia sẻ.
 Sự phụ thuộc: Các lớp cao hơn phụ thuộc vào các lớp thấp hơn, nhưng các lớp thấp hơn không phụ thuộc vào các lớp 
cao hơn. Điều này cho phép bạn thay đổi hoặc nâng cấp các lớp thấp hơn mà không ảnh hưởng đến các lớp cao hơn.
Ví dụ:
 Lớp ứng dụng muốn đọc dữ liệu từ sensor.
 Lớp ứng dụng sẽ gọi hàm API từ HAL để cấu hình sensor.
 HAL sẽ tương tác với driver của sensor để kích hoạt đọc dữ liệu.
 Driver của sensor sẽ đọc dữ liệu từ sensor và truyền dữ liệu đó về HAL.
 HAL sẽ trả về dữ liệu đã đọc cho lớp ứng dụng.
 Lợi ích của Project Layers Interaction:

6. Ưu điểm: 
 Tách biệt trách nhiệm: Giúp cho việc phát triển, bảo trì và gỡ lỗi trở nên dễ dàng hơn.
 Tái sử dụng code: Các driver và HAL có thể được tái sử dụng trong các dự án khác.
 Cải thiện khả năng bảo trì: Thay đổi một lớp không ảnh hưởng đến các lớp khác.
 Nâng cấp dễ dàng: Có thể nâng cấp các lớp thấp hơn mà không cần thay đổi code của lớp ứng dụng.
*/
/* STM32 Cube framework program flow - Luồng hoạt động của chương trình
"STM32 Cube framework program flow" mô tả luồng thực thi của chương trình trong một dự án STM32 được tạo ra bằng 
STM32CubeMX. Nó bao gồm các giai đoạn chính mà vi điều khiển STM32 trải qua khi được bật nguồn và chạy mã của bạn.
^Giai đoạn 1: Khởi động (Startup):
 Khởi tạo vector ngắt: File startup_stm32f4xx.s (nằm trong thư mục Startup) sẽ thiết lập bảng vector ngắt cho vi 
điều khiển.
 Khởi tạo bộ nhớ: Gán vùng nhớ cho các thành phần như stack, heap và data.
 Khởi tạo các thiết bị ngoại vi cơ bản: Thực hiện các thao tác khởi tạo cơ bản cho một số thiết bị ngoại vi như 
đồng hồ (RCC), GPIO, v.v.
 Gọi hàm main(): File khởi động sẽ chuyển quyền điều khiển sang hàm main() trong file main.c của bạn.
 
^Giai đoạn 2: Khởi tạo HAL (Hardware Abstraction Layer): Gọi hàm HAL_Init(): Trong hàm main(), bạn cần gọi 
hàm HAL_Init() để khởi tạo HAL. Hàm này sẽ:
 -Khởi tạo HAL (cấu trúc dữ liệu, biến, hàm API)
 -Cấu hình đồng hồ (RCC)
 -Khởi tạo bộ nhớ
 -Khởi tạo ngắt (NVIC)
 -Khởi tạo một số thiết bị ngoại vi cơ bản

^Giai đoạn 3: Khởi tạo các thiết bị ngoại vi cụ thể: Sử dụng các hàm API của HAL: Sau khi HAL được khởi tạo, 
bạn sẽ sử dụng các hàm API của HAL để cấu hình và kiểm soát các thiết bị ngoại vi cụ thể mà bạn cần sử dụng 
trong dự án. Ví dụ:
 HAL_GPIO_Init() để cấu hình chân GPIO
 HAL_UART_Init() để cấu hình UART
 HAL_SPI_Init() để cấu hình SPI
 HAL_TIM_Init() để cấu hình Timer

^Giai đoạn 4: Vòng lặp chính (Main loop): 
 Thực hiện các chức năng chính: Trong vòng lặp chính của hàm main(), bạn sẽ viết code để thực hiện các chức năng 
chính của ứng dụng.
 Xử lý ngắt: Vi điều khiển sẽ tự động xử lý các ngắt (nếu được cấu hình) và gọi các hàm xử lý ngắt tương ứng.

^Giai đoạn 5: Thoát chương trình: Kết thúc vòng lặp chính: Khi chương trình kết thúc, vi điều khiển có thể chuyển 
sang chế độ ngủ đông (sleep) hoặc tắt nguồn (power off) để tiết kiệm năng lượng.

^Lưu ý: Luồng chương trình này là một sơ đồ chung, có thể có một số biến thể phụ thuộc vào cấu hình và chức năng 
của dự án. STM32CubeMX cung cấp các công cụ hỗ trợ để tạo code HAL và cấu hình các thiết bị ngoại vi, giúp bạn 
dễ dàng quản lý và điều khiển hệ thống nhúng STM32.
*/
/* HAL_Init()
^Hàm HAL_Init() là một hàm cực kỳ quan trọng trong lập trình hệ thống nhúng STM32F4. Nó được cung cấp bởi 
HAL (Hardware Abstraction(trừu tượng) Layer) của STM32Cube và là điểm khởi đầu cho việc khởi tạo và cấu hình toàn 
bộ hệ thống nhúng của bạn.

^Chức năng chính của hàm HAL_Init():
 Khởi tạo HAL: Hàm này sẽ khởi tạo HAL, bao gồm việc khởi tạo các cấu trúc dữ liệu, biến và các hàm API cần thiết 
để tương tác với phần cứng của STM32F4.
 Khởi tạo các "Flash Interface Init(Stm32_hal_conf.h)", các SysTick timer,HAL_MspInit(),...
Với hàm "HAL_MspInit()" thì nên tham khảo thêm file "stm32f4xx_hal_msp.c" nằm ở thư mục "Src"
 Khởi tạo đồng hồ (RCC): Hàm này sẽ cấu hình đồng hồ chính (System Clock) của vi điều khiển STM32F4, xác định tốc 
độ hoạt động của CPU và các thiết bị ngoại vi khác.
 Khởi tạo bộ nhớ: Hàm này sẽ gán vùng nhớ cho các thành phần khác nhau của hệ thống, bao gồm stack, heap và dữ liệu 
toàn cục.
 Khởi tạo ngắt (NVIC): Hàm này sẽ cấu hình vector ngắt (Interrupt Vector) của vi điều khiển, đảm bảo rằng các ngắt 
được xử lý chính xác.
 Khởi tạo các thiết bị ngoại vi cơ bản: Hàm này sẽ khởi tạo một số thiết bị ngoại vi cơ bản như GPIO, DMA, v.v. 
nhưng không bao gồm các thiết bị ngoại vi cụ thể mà bạn muốn sử dụng.

^Cách sử dụng:
Hàm HAL_Init() thường được gọi đầu tiên trong hàm main() của bạn. Ví dụ:
int main(void) {
  // Khởi tạo HAL
  HAL_Init();

  // ... (Khởi tạo các thiết bị ngoại vi khác và thực hiện các tác vụ khác)
}

^Lưu ý:
 Hàm HAL_Init() chỉ khởi tạo HAL và các chức năng cơ bản của vi điều khiển. Bạn vẫn cần khởi tạo và cấu hình các 
thiết bị ngoại vi cụ thể mà bạn muốn sử dụng trong chương trình của mình.
 Hàm HAL_Init() chỉ cần được gọi một lần trong suốt quá trình chạy của chương trình.
 Nếu bạn không gọi HAL_Init(), hệ thống có thể hoạt động không chính xác hoặc gặp lỗi.

^Tầm quan trọng của hàm HAL_Init(): Hàm HAL_Init() đóng vai trò rất quan trọng trong quá trình khởi động hệ thống
nhúng STM32F4. Nó đảm bảo rằng các chức năng cơ bản của vi điều khiển được khởi tạo một cách chính xác và hệ thống 
hoạt động ổn định. Nếu bạn bỏ qua hàm này, hệ thống có thể gặp lỗi nghiêm trọng và không hoạt động như mong đợi.

^Lời khuyên: Luôn luôn gọi hàm HAL_Init() đầu tiên trong hàm main() của bạn để đảm bảo hệ thống nhúng STM32F4 hoạt 
động chính xác.
*/
/* Understanding "main.c" and "stm32f4xx_hal_msp.c" and "stm32f4xx_it.c"
^Tham khảo sơ đồ: RM0390 -> Trang 7+8
^File "main.c"(Application): HIGH Level Initialization
Chức năng: Là file mã nguồn chính của dự án, chứa hàm main() và các hàm liên quan đến logic chính của chương trình.
Nội dung: Hàm main():
 Thường gọi hàm HAL_Init() để khởi tạo HAL (Hardware Abstraction Layer).
 Khởi tạo các thiết bị ngoại vi cần thiết cho ứng dụng.
 Thực thi các chức năng chính của chương trình, bao gồm vòng lặp chính (main loop).
 Các hàm khác: Có thể chứa các hàm hỗ trợ cho logic chính của chương trình.

^File stm32f4xx_hal_msp.c: LOW Level Initialization
Chức năng: Chứa các hàm liên quan đến khởi tạo và khử khởi tạo các thiết bị ngoại vi (peripheral) và các tài 
nguyên khác.
Nội dung: Ví dụ Hàm "HAL_UART_MspInit()": Khởi tạo các tài nguyên cho UART, bao gồm:
 Cấu hình chân GPIO cho TX và RX.
 Kích hoạt đồng hồ cho UART.
 Hàm HAL_UART_MspDeInit(): khởi tạo các tài nguyên cho UART.
 Các hàm tương tự cho các thiết bị ngoại vi khác: Ví dụ: HAL_GPIO_MspInit(), HAL_SPI_MspInit(), HAL_TIM_MspInit(), v.v.
 Ngoài ra, khởi tạo các hàm liên quan đến cấu hình NVIC(dùng cho System Exception hoặc Interrupt)

^File "stm32f4xx_it.c":
Chức năng: Chứa các hàm xử lý ngắt (interrupt handler) cho các thiết bị ngoại vi.
Nội dung: Một số ví dụ
 Hàm EXTI0_IRQHandler(): Xử lý ngắt từ chân GPIO0.
 Hàm UART2_IRQHandler(): Xử lý ngắt từ UART2.
 Các hàm tương tự cho các ngắt khác: Ví dụ: TIM2_IRQHandler(), SPI1_IRQHandler(), v.v.
*/
/* Peripheral Handle Structure - Cấu trúc Xử lý Thiết bị Ngoại vi
^Là một cấu trúc dữ liệu được sử dụng để quản lý và truy cập các thiết bị ngoại vi (peripheral) của vi điều khiển. 
Mỗi thiết bị ngoại vi được liên kết với một cấu trúc xử lý riêng biệt, chứa các thông tin và hàm API cần thiết để 
điều khiển thiết bị đó. 
 Cấu trúc xử lý này giúp đơn giản hóa việc tương tác với các thiết bị ngoại vi bằng cách cung cấp một giao diện 
thống nhất. Thay vì thao tác trực tiếp với các thanh ghi phần cứng, bạn chỉ cần sử dụng các hàm API được cung 
cấp bởi cấu trúc xử lý.

^Tham khảo các Peripheral Handle Structure trong Project: CMSIS->Device->ST->file "stm32f446xx.h"
Đây xem như là file "stm32f4xx.h" dùng để cấu hình "Base Address Peripheral" từ Datasheet
Từ đó xác định các Address của các APBx hoặc AHBx -> Address Peripheral muốn sử dụng

^Lợi ích của việc sử dụng cấu trúc xử lý:
 -Tổ chức code rõ ràng: Mỗi thiết bị ngoại vi được quản lý bởi một cấu trúc riêng biệt, giúp cho code dễ đọc, 
 hiểu và bảo trì hơn.
 -Tái sử dụng code: Cấu trúc xử lý có thể được tái sử dụng cho nhiều dự án khác nhau.
 -Cải thiện hiệu suất: HAL sử dụng cấu trúc xử lý để tối ưu hóa việc truy cập và điều khiển các thiết bị ngoại vi,
 giúp cải thiện hiệu suất của chương trình.

^Ví dụ về cấu trúc UART: 
+Cấu trúc xử lý thiết bị ngoại vi UART thường bao gồm các thành phần sau:
 -Instance: Biến chỉ định instance cụ thể của thiết bị ngoại vi. Ví dụ, nếu bạn đang sử dụng UART2, biến Instance 
 sẽ được gán là USART2. Ví dụ: 
	typedef struct
	{
		__vo uint32_t SR;			//USART Status register  			    	- Address offset: 0x00 - __vo: volatile
		__vo uint32_t DR;			//USART Data register 			    		- Address offset: 0x04
		__vo uint32_t BRR;			//USART Baud rate register  			    - Address offset: 0x08
		__vo uint32_t CR1;			//USART Control register 1			   		- Address offset: 0x0C
		__vo uint32_t CR2;			//USART Control register 2 			   		- Address offset: 0x10
		__vo uint32_t CR3;			//USART Control register 3 			   		- Address offset: 0x14
		__vo uint32_t GTPR;			//USART Guard time and prescaler register	- Address offset: 0x18
									//Thời gian bảo vệ và đăng ký bộ đếm trước
	}USART_RegDef_t;  
	
 -Init: Cấu trúc chứa các thông số cấu hình ban đầu của thiết bị ngoại vi. Ví dụ, User có thể đặt tốc độ baud rate, 
 độ dài dữ liệu, parity, stop bit, v.v. trong cấu trúc Init cho UART.
	typedef struct
	{
		uint8_t  USART_Mode;					//Tx or Rx or Tx-Rx
		uint32_t USART_Baud;					//Baud rate
		uint8_t  USART_NoOfStopBits;			//Use Stop bit
		uint8_t  USART_WordLength;				//Data Frame
		uint8_t  USART_ParityControl;			//Odd or Even
		uint8_t  USART_HWFlowControl;			//CTS or RTS
	}USART_Config_t;
	
 -State: Biến thể hiện trạng thái hiện tại của thiết bị ngoại vi, chẳng hạn như đang hoạt động, đang chờ, bị lỗi, v.v.
 -ErrorCode: Biến lưu trữ mã lỗi nếu xảy ra lỗi khi sử dụng thiết bị ngoại vi.
 -Lock: Biến thể hiện trạng thái khóa của thiết bị ngoại vi. Một thiết bị ngoại vi có thể bị khóa bởi một nhiệm vụ 
 cụ thể để tránh xung đột khi nhiều nhiệm vụ cố gắng truy cập cùng lúc.
 -pRxBuffPtr: Con trỏ đến bộ nhớ đệm nhận dữ liệu (buffer).
 -pTxBuffPtr: Con trỏ đến bộ nhớ đệm truyền dữ liệu (buffer).
 -RxXferSize: Kích thước bộ nhớ đệm nhận dữ liệu.
 -TxXferSize: Kích thước bộ nhớ đệm truyền dữ liệu.
 -RxXferCount: Số lượng byte đã được nhận.
 -TxXferCount: Số lượng byte đã được truyền.
 
 -Ví dụ về một UART Handle:
	typedef struct
	{
		USART_RegDef_t *pUSARTx;				//Cấu hình thanh ghi để processor sử dụng xử lý chương trình
		USART_Config_t   USART_Config;			//Cấu hình ban đầu do user đưa vào khi khởi tạo ban đầu

		//Configuration for Interrupt
		uint8_t *pTxBuffer;
		uint8_t *pRxBuffer;
		uint32_t TxLen;
		uint32_t RxLen;
		uint8_t TxBusyState;
		uint8_t RxBusyState;
	}USART_Handle_t;

+Sử dụng cấu trúc xử lý thiết bị ngoại vi:
 -Khởi tạo: Bạn cần khởi tạo cấu trúc xử lý cho mỗi thiết bị ngoại vi bạn muốn sử dụng. Cấu trúc này thường được 
 khai báo là một biến toàn cục trong file main.c hoặc trong các file driver của thiết bị ngoại vi.
 -Cấu hình: Bạn sử dụng các hàm API của HAL để cấu hình các thông số của thiết bị ngoại vi thông qua cấu trúc xử 
 lý. Ví dụ:
	UART_Handle_t huart2;
	// Khởi tạo cấu trúc xử lý cho UART2
	huart2.Instance = USART2;

	// Cấu hình các thông số cho UART2
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	// Khởi tạo UART2
	if (HAL_UART_Init(&huart2) != HAL_OK) {
	Error_Handler();
	}
	
 -Truy cập và điều khiển: Sau khi cấu hình xong, bạn có thể sử dụng các hàm API của HAL để điều khiển thiết bị 
 ngoại vi thông qua cấu trúc xử lý. Ví dụ:
	// Gửi dữ liệu qua UART2
	HAL_UART_Transmit(&huart2, data, len, HAL_MAX_DELAY);
	// Nhận dữ liệu từ UART2
	HAL_UART_Receive(&huart2, data, len, HAL_MAX_DELAY);
	
^Ví dụ về cấu trúc GPIO:
typedef struct {
  GPIO_TypeDef* Instance;  // Biến chỉ định instance cụ thể của GPIO
  GPIO_InitTypeDef Init;  // Cấu trúc chứa các thông số cấu hình
  uint32_t Lock;  // Biến thể hiện trạng thái khóa của GPIO
  uint32_t State;  // Biến thể hiện trạng thái hiện tại của GPIO
  uint32_t ErrorCode;  // Biến lưu trữ mã lỗi nếu xảy ra lỗi
  
  *Giải thích các thành phần:
	Instance: Biến này là con trỏ đến instance cụ thể của GPIO (ví dụ: GPIOC).
	Init: Cấu trúc này chứa các thông số cấu hình ban đầu của GPIO (ví dụ: chế độ hoạt động, tốc độ, pull-up/pull-down,
   chức năng thay thế).
	Lock: Biến này thể hiện trạng thái khóa của GPIO.
	State: Biến này thể hiện trạng thái hiện tại của GPIO.
	ErrorCode: Biến này lưu trữ mã lỗi nếu xảy ra lỗi khi sử dụng GPIO. 
} GPIO_HandleTypeDef;
int main{
	GPIO_HandleTypeDef hgpioc; // Khai báo cấu trúc xử lý GPIO
	// Khởi tạo cấu trúc GPIO
	hgpioc.Instance = GPIOC; 
	hgpioc.Init.Pin = GPIO_PIN_13;
	hgpioc.Init.Mode = GPIO_MODE_OUTPUT_PP;
	hgpioc.Init.Speed = GPIO_SPEED_FREQ_LOW;
	hgpioc.Init.Pull = GPIO_NOPULL;
	// Khởi tạo GPIOC
	if (HAL_GPIO_Init(&hgpioc) != HAL_OK) {
	  Error_Handler();
	}
	// Bật LED trên GPIOC.13
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	// Tắt LED trên GPIOC.13
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	// Đọc trạng thái của chân GPIOC.13
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
}
*/
/* STM32 HAL Header File Hierarchy - Hệ thống phân cấp thư viện
^Hệ thống phân cấp file header này giúp bạn dễ dàng tìm kiếm và sử dụng các hàm API cần thiết để điều khiển các
thiết bị ngoại vi và các chức năng của vi điều khiển.
^Sơ đồ hoạt động: "startup_stm32f446xx.s" -> "stm32f4xx.h" -> "stm32f446xx.h" -> "stm32f4xx_hal.h"
Với: 
 "startup_stm32f4xx.s" : File khởi động "Reset Hanler"
 #include "stm32f4xx.h" : File thư viện dành cho MCU family
 #include "stm32f446xx.h" : File thư viện dành cho con chip MCU đang đc sử dụng
 #include "stm32f4xx_hal.h" : File thư viện của CUBE_HAL

^Cách cấu hình hoặc xác định chip muốn hoặc đang sử dụng trong file "stm32f4xx.h". Ví dụ sử dụng chip STM32F446re
Tab "Project" -> Properties -> C/C++ Build -> Setting -> MCU GCC Compiler -> Mục "Preprocessor" -> Xác nhận tên chip
*/

/**-III-- STM32 HAL Program Flow with UART Exercise(S7) ---*/
/* System Control Block(SCB) trong Chip Cortex M4 (Datasheet CortexM4_UserGuide->4.3)
1. Khái niệm: SCB (System Control Block) là một khối điều khiển hệ thống được tích hợp trong bộ xử lý ARM Cortex-M4. 
Nó chứa các thanh ghi đặc biệt dùng để kiểm soát các chức năng cơ bản của bộ xử lý, bao gồm:
	+Quản lý ngắt (Interrupt Management)
	+Xử lý ngoại lệ (Exception Handling)
	+Kiểm soát năng lượng (Power Management)
	+Cấu hình bộ nhớ (Memory Configuration)

2. Chức năng chính của SCB:
2.1 Quản lý ngắt:
 +Thiết lập nhóm ưu tiên (Priority Grouping): Xác định số lượng nhóm ưu tiên cho ngắt, ảnh hưởng đến cách thức xử 
 lý ngắt có cùng mức ưu tiên.
 + Cấu hình mức ưu tiên (Priority): Thiết lập mức ưu tiên cho mỗi ngắt, cho phép bạn ưu tiên xử lý các ngắt quan 
 trọng hơn.
 +Kích hoạt và vô hiệu hóa ngắt: Bật hoặc tắt các ngắt cụ thể.
 +Xử lý ngắt: Xử lý các ngắt khi chúng xảy ra.
 
2.2 Xử lý ngoại lệ:
 +Kích hoạt và vô hiệu hóa ngoại lệ: Bật hoặc tắt các ngoại lệ cụ thể.
 +Cấu hình mức ưu tiên: Thiết lập mức ưu tiên cho các ngoại lệ.
 +Xử lý ngoại lệ: Xử lý các ngoại lệ khi chúng xảy ra (ví dụ: lỗi bus, lỗi bộ nhớ, lỗi sử dụng lệnh sai).
 
2.3 Kiểm soát năng lượng:
 +Thiết lập trạng thái ngủ đông (Sleep mode): Cấu hình bộ xử lý để chuyển sang chế độ ngủ đông (Sleep mode) nhằm 
 tiết kiệm năng lượng.
 +Thiết lập trạng thái dừng (Stop mode): Cấu hình bộ xử lý để chuyển sang chế độ dừng (Stop mode) nhằm giảm mức 
 tiêu thụ năng lượng xuống mức thấp nhất.
 
2.4 Cấu hình bộ nhớ: Thiết lập vùng bộ nhớ: Xác định vùng bộ nhớ cho stack, heap, các dữ liệu toàn cục (global) 
và các phần khác của chương trình.

3. Các thanh ghi chính trong SCB:
 +AIRCR (Application Interrupt and Reset Control Register): Điều khiển các chức năng liên quan đến ngắt và reset.
 +SHCSR (System Handler Control and State Register): Kiểm soát các ngoại lệ hệ thống (System Exception).
 +NVIC (Nested Vector Interrupt Controller): Kiểm soát việc xử lý ngắt.
 +MPU (Memory Protection Unit): Bảo vệ bộ nhớ khỏi truy cập trái phép.
 +FPU (Floating-Point Unit): Kiểm soát đơn vị xử lý số dấu phẩy động (floating-point).
 
4. Sử dụng SCB trong lập trình nhúng:
 +Thiết lập cấu hình ngắt: Sử dụng các thanh ghi trong SCB để thiết lập mức ưu tiên, nhóm ưu tiên và trạng thái 
 kích hoạt/vô hiệu hóa của ngắt.
 +Xử lý ngoại lệ: Xử lý các ngoại lệ hệ thống bằng cách sử dụng các hàm xử lý ngoại lệ (exception handler) được 
 xác định trong startup_stm32f4xx.s.
 +Kiểm soát năng lượng: Sử dụng SCB để chuyển bộ xử lý sang các chế độ ngủ đông hoặc dừng để tiết kiệm năng lượng.
*/
/* Project Exercise: Cấu hình UART Peripheral
^Chương trình sẽ viết các file "main.c" and "stm32f4xx_hal_msp.c" and "stm32f4xx_it.c" theo từng bước
cho việc cấu hình UART Peripheral theo thư viên HAL_Cube. 
 Các bước thực hiện: 
 +Khởi tạo phần cứng cấp thấp(Low Level Hardware Initializations)
	• Cụ thể cho bộ xử lý - Processor Specific
	– Cấu hình nhóm Ưu tiên của bộ xử lý
	– Kích hoạt Ngoại lệ hệ thống bộ xử lý bắt buộc
	• Lỗi bus, quản lý bộ nhớ, lỗi sử dụng, Systick, v.v.
	– Cấu hình mức độ ưu tiên của ngoại lệ hệ thống
	– Các khởi tạo khác liên quan đến MPU, Floating point unit, Sleep mode, v.v.

 +Khởi tạo ngoại vi(Peripheral Initializations): UART2
	• Xác định một biến xử lý để Xử lý thiết bị ngoại vi bắt buộc dưới dạng biến toàn cục trong	chương trình
	• Liên kết cũng như khởi tạo biến xử lý
	• Sử dụng API init. của Thiết bị ngoại vi để khởi tạo	thiết bị ngoại vi

^Cách thiết lập Tera Term để gửi data từ PC đến Board
Setup -> Terminal -> New-line (Receive: CR+LF) và Local-echo(check)
Sau đó gõ chữ trên terminal và enter

^Quy trình chi tiết:
 +B1: Xóa các file do thư viên HAL_Cube tạo ra -> xóa cả file .c và .h
 +B2: Tạo thành các file .c và .h mới 
 +B3: Khai báo các thư viện HAL cần thiết trong file main.c -> HAL_Init(); 
 +B4: Tạo hàm "SystemClockConfig" -> Chưa code -> Buide Project chạy thử
 +B5: Copy hàm "HAL_MspInit()" trong file "stm32f4xx_hal.c" vào trong file "msp.h"
  -Tiến hành Code trong hàm
  -Tìm hiểu file "tm32f4xx_hal_cortex.h"(1)
  -Tìm hiểu về thanh ghi SHCSR(CortexM4_UserGuide->4.3.9)-> Enable system exception
 +B6: Tiến hành code hàm "UART2_Init()" 
  -Khai báo biến toàn cục "UART_HandleTypeDef huart2" 
  -Tham khảo các define lựa chọn USARTx ở file "stm32f4xx_hal_uart.h"
 +B7: Tiến hành code hàm "void HAL_UART_MspInit()"
  -define "__HAL_RCC_USART2_CLK_ENABLE()" đã đc thiết lập sẵn trong file "stm32f4xx_hal_rcc.h"
  -Xác định 2 chân GPIO thiết lập Alternate là PA2(UART_Tx_AF7) và PA3(UART_Rx_AF7) -> file "stm32f4xx_hal_gpio.c"
  -Thiêt lập USARTx Interrupt -> hàm HAL_NVIC_EnableIRQ() of file "stm32f4xx_hal_cortex.c"
 +B8: Thiết lập hàm "HAL_UART_Transmit()" để gửi
 +B9: Thiết lập hàm "SysTick_Handler()" trong file it.c đển cấu hình thời gian chuẩn xác cho ứng dụng
 +B10: Thiết lập hàm "HAL_UART_Receive()" để nhận
 
 ~Tạo một project mới để code UART Receive Interrupt
 +B1: Sử dụng Interrupt để nhận data(có xóa nội dung code cũ)
  -Thiết lập hàm "HAL_UART_Receive_IT()" -> hàm Init interrupt nếu có việc nhận data
 +B2: Tiến hành code hàm "USART2_IRQHandler" trong file it.c
  -Call hàm "HAL_UART_IRQHandler" nằm trong file "stm32f4xx_hal_uart.c"
  -Thêm thư viện "stm32f4xx_hal.h" vào trong file "main.h"
 +B3: Thiết lập hàm "HAL_UART_RxCpltCallback" trong main.c
*/
/* Từ khóa "extern" trong lập trình C/C++
^Được sử dụng để khai báo một biến hoặc hàm được định nghĩa ở một file khác. Nó giúp bạn chia nhỏ code thành nhiều file 
và quản lý code dễ dàng hơn.
^Cách hoạt động:
 +extern là một "tín hiệu" cho trình biên dịch biết rằng biến hoặc hàm được khai báo sẽ được định nghĩa ở một file khác, 
 không phải trong file hiện tại.
 +Trình biên dịch sẽ không cố gắng cấp phát bộ nhớ cho biến extern trong file hiện tại mà sẽ tìm kiếm định nghĩa của biến 
 đó trong các file khác.
 +Định nghĩa của biến hoặc hàm extern phải có sẵn trong lúc liên kết (linking), để trình biên dịch có thể tìm thấy địa chỉ 
 của biến hoặc hàm đó.
^Sử dụng extern:
 +Khai báo biến toàn cục:
	// file1.c
	int global_variable = 10; // Định nghĩa biến global_variable
	// file2.c
	extern int global_variable; // Khai báo biến global_variable
	
 +Khai báo hàm
	// file1.c
	int add(int a, int b) {
		return a + b;
	} 
	// file2.c
	extern int add(int a, int b); // Khai báo hàm add()
 
 +Lập trình STM32F4
	//file1.c (main.c)
	#include "stm32f4xx_hal.h"
	extern uint8_t rcvd_data; // Khai báo biến rcvd_data được định nghĩa ở file khác
	int main(void) {
	  // ... (Khởi tạo HAL và UART)
	  // ... (Nhận dữ liệu từ UART)
	  HAL_UART_Receive(&huart2, &rcvd_data, 1, HAL_MAX_DELAY); 
	}

	// file2.c (stm32f4xx_it.c)
	uint8_t rcvd_data; // Định nghĩa biến rcvd_data 

	void UART2_IRQHandler(void) {
	  // ... (Xử lý ngắt UART2)
	  rcvd_data = // Giá trị dữ liệu nhận được ;
	}
 
^Lưu ý:
 +Bạn có thể sử dụng extern để khai báo biến toàn cục hoặc hàm được định nghĩa ở các file khác.
 +Tên của biến hoặc hàm được khai báo bằng extern phải trùng với tên của biến hoặc hàm được định nghĩa trong file khác.
 +Bạn chỉ cần khai báo một biến hoặc hàm bằng extern một lần trong một dự án.
 +Bạn cần chắc chắn rằng file chứa định nghĩa của biến hoặc hàm được liên kết vào dự án trong lúc biên dịch.
*/

/**-IV-- Clock and PLL Programing(S8) ---*/
/* System Clock
1. Khái niệm: "System Clock" (Đồng hồ Hệ thống) là nguồn cung cấp xung nhịp cơ bản cho vi điều khiển STM32F4. 
Nó là "trái tim" của hệ thống, điều khiển tốc độ hoạt động của CPU và các thiết bị ngoại vi. Tần số của System
Clock ảnh hưởng trực tiếp đến hiệu năng của chương trình, nghĩa là CLK càng cao thì điện năng tiêu thụ càng lớn.

2. Nguồn gốc: 
 ^Mạch dao động (Oscillator): System Clock thường được tạo ra từ một mạch dao động thạch anh (crystal oscillator) 
 hoặc một nguồn dao động bên trong (internal oscillator).
 ^Bộ nhân tần số (PLL): Để tăng tần số System Clock, thường sử dụng bộ nhân tần số (PLL - Phase-Locked Loop) 
 để nhân tần số từ mạch dao động.
 
3. Các loại đồng hồ:
 HSI(High-speed Internal): Đồng hồ - bộ dao động nội tốc độ cao, thường có tần số 16MHz.
 HSE(High-speed External): Bộ dao động ngoại tốc độ cao, thường là mạch dao động thạch anh.
 LSE(Low-speed External): Bộ dao động ngoại(thạch anh) tốc độ thấp(32.768kHz), thường sử dụng cho RTC (Real-Time Clock).
 LSI(Low-speed Internal): Bộ dao động nội bộ tốc độ thấp(32kHz), thường sử dụng cho các chức năng tiêu thụ năng lượng thấp.

4. Cấu hình System Clock:
 ^Bộ điều khiển đồng hồ (RCC): Trong STM32F4, bộ điều khiển đồng hồ (RCC - Reset and Clock Control) được sử dụng để 
 cấu hình System Clock.
 ^Thư viện HAL: STM32 HAL cung cấp các hàm API để dễ dàng cấu hình System Clock.

5. Các bước cấu hình System Clock:
 ^Chọn nguồn đồng hồ: Chọn nguồn đồng hồ chính (HSI hoặc HSE) cho System Clock.
 ^Cấu hình PLL (nếu cần sử dụng): Nếu sử dụng PLL, cần cấu hình PLL để nhân tần số.
 ^Cấu hình các thiết bị ngoại vi: Cấu hình đồng hồ cho các CPU,AHB,APB bus đối với thiết bị ngoại vi muốn sử dụng, 
 sử dụng các phân chia tần số từ System Clock. 
 ^Kiểm tra và gỡ lỗi: Kiểm tra xem System Clock đã được cấu hình chính xác chưa và gỡ lỗi nếu cần.
  +Cấu hình và kiểm tra độ trễ của flash để phòng flash và bus không giao tiếp với nhau.
  +Enable Clock đã lựa chọn làm SYSCLK 

6. Nucleo-F446RE Board
 ^HSI -> 16MHz (Internal to MCU)
 ^HSE -> 8MHz (External to MCU)
 ^PLL -> Can generate clock up to 180MHz(Internal to MCU)
 ^LSI -> 32kHz (Internal to MCU)
 ^LSE -> 32.768kHz (External to MCU)
 
 ^Ở trạng thái mặc định sau khi reset MCU thì HSI_ON và các clock còn lại sẽ bị OFF. Nghĩa là lúc này SYSCLK của hệ 
 thống là HSI=16MHz. 
 ^Các clock khác nhau có thể ON_OFF độc lập lẫn nhau nếu ko sử dụng. Nghĩa là nếu chương trình sử dụng
 hai clock vd HSI và HSE nhưng trong một thời điểm tính năng mà sử dụng clock HSE ko sử dụng thì chương trình
 sẽ tắt clock HSE để tiết kiệm năng lượng.
 
7. STM32 Cube Clock Handling APIs: STM32 HAL RCC Driver
 ^Header: "stm32f4xx_hal_rcc.h" và "stm32f4xx_hal_rcc_ex.h" (ex = Extension)
 ^Source: "stm32f4xx_hal_rcc.c" và "stm32f4xx_hal_rcc_ex.c"
	Hàm: HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitTruct)
		 HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitTruct)
*/
/* Exercise: Using HSE Configure the SYSCLK as 8MHz
+ AHB clock as 4MHz(HCLK)
+ APB1 clock as 2MHz(PCLK1)
+ APB2 clock as 2MHz(PCLK2)
 =>Lưu ý: lập trình viên có thể sử dụng phần mềm STM32CubeMx-> file ".ioc" để cấu hình xung CLK theo đề bài

^Chuẩn bị tài liệu
+Datasheet : RM0390->6.2->Figure14-Sơ đồ khối các nguồn xung clock
+File "stm32f4xx_hal_rcc_ex.c" ->  Call "HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitTruct)"
 -"RCC_OscInitTypeDef" là một biến Struct đc khai báo ở file "stm32f4xx_hal_rcc_ex.h"
+File "stm32f4xx_hal_rcc.c" ->  Call "HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitTruct)"
 -"RCC_ClkInitTypeDef" là một biến Struct đc khai báo ở file "stm32f4xx_hal_rcc.h"

^Các bước thực hiện: Thực hiện cấu hình xung CLK bằng code
 +B1: Trước tiên, hãy "Enable Clock" cần thiết và đợi cho đến khi đồng hồ sẵn sàng. Nếu ứng dụng của bạn cần PLL, 
 hãy cấu hình PLL và enable nó.
  -Khai báo hai biến cấu trúc "osc_init" ở file main.c
  -Cấu hình các thông số cho các thành phần của biến struct "osc_init"
   ~Lưu ý 1: các lựa chọn cho thành phần cũng đã đc "define" ở file thư viện với biến cấu trúc "osc_init"
   ~Call hàm "HAL_RCC_OscConfig()" và cấu hình 
   ~Lưu ý 2: Phải cấu hình biến osc_init vì nếu ko sử dụng file ".ioc" để cấu hình xung clk trước sau đó mới generate
   mà dùng code để cấu hình clk thì mặc định ban đầu chương trình sẽ chọn clk là HSI=16MHz
   
 +B2: Khởi tạo bộ chia xung nhịp CPU, AHB và APB bus theo yêu cầu ứng dụng của bạn. Không vượt quá giới hạn tối đa.
  -Khai báo hai biến cấu trúc "clk_init" ở file main.c
  -Cấu hình các thông số cho các thành phần của biến struct "clk_init"
   ~Lưu ý: các lựa chọn cho thành phần cũng đã đc "define" ở file thư viện với biến cấu trúc "clk_init"
   ~Call hàm "HAL_RCC_ClockConfig()" và cấu hình
    Lưu ý: Khi cấu hình Clk thì ta cũng sẽ cấu hình độ trễ flash của bước 3 luôn
	Thêm: Đây chính là bước cấu hình thanh ghi RCC_CFGR
   
 +B3: Cấu hình độ trễ flash đúng cách bằng cách tham chiếu đến MCU RM
  -Tham khảo: Datasheet RM0390 -> 3.4.1 và Table 5
  -Giải thích: Theo bảng Table_5 thì clk muốn sử dụng nằm trong khoảng 0MHz <clk <30MHz nên LATENCY = 0WS
  
 +B4: Chọn Clock mới được chọn là SYSCLK
  -Sử dụng một "define" để tắt HSI clk để chuyển đổi sang clk khác và tiết kiệm năng lượng
  -Cấu hình Stick với hàm "HAL_SYSTICK_Config()" trong file "stm32f4xx_hal_cortex.c"
  -Cấu hình clksource cho bộ SysTick
  -Call UART
  -Thiết lập các hàm test 

^Chế độ "HSE By-Pass" trên Nucleo-F446RE
 +Khi lựa chọn HSE clock thì ta có 3 lựa chọn là On,Off và ByPass
 +Lựa chon HSE "On" hay "Off" là lựa chọn dùng hay không dùng bộ dao động ngoại (ví dụ thạch anh). Tuy nhiên khi
 quan sát board F446RE thì chân X3 dùng chứa bộ dao động ngoại ko đc sử dụng -> Nếu ko có phần cứng phù hợp thì
 ko thể lựa chọn HSE dùng bộ dao động ngoại -> Chỉ có 1 lựa chọn HSE clock duy nhất là "By-Pass"
 +Lựa chọn HSE "By-Pass" là lựa chọn dùng xung của một MCU khác cấp cho MCU chính. MCU này chính là ST-link MCU.
 ST-link MCU là MCU dùng để nạp và debug chương trình. Nghĩa là sử dụng bộ dao động thạch anh(X1)8MHz của ST-link MCU
 truyền qua chân output PA8 và nối vào chân OSC_IN của Main MCU STM32F446RE 
 ->Nếu lựa chọn "By-Pass" thì HSE clock có giá trị gốc là 8MHz

^Giải thích dòng code: 
 +Khởi tạo cấu trúc bằng hàm memset:
  memset(&osc_init,0,sizeof(osc_init)); Dòng này gọi hàm memset để khởi tạo tất cả các thành viên của cấu trúc 
  osc_init bằng giá trị 0. Hàm memset có ba tham số:
  &osc_init: Địa chỉ bắt đầu của vùng nhớ cần khởi tạo (ở đây là địa chỉ của cấu trúc osc_init).
  0: Giá trị cần gán cho từng byte trong vùng nhớ (trong trường hợp này là 0).
  sizeof(osc_init): Kích thước của vùng nhớ cần khởi tạo (tính bằng byte), tương ứng với kích thước của cấu trúc 
  osc_init.
  
 + Code: clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
							  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  -Dòng code này sử dụng toán tử bitwise OR (|) để kết hợp các hằng số đại diện cho các loại đồng hồ. Điều này 
 có nghĩa là dòng code này sẽ cấu hình đồng hồ cho tất cả các loại đồng hồ được liệt kê, bao gồm System Clock, 
 AHB clock, APB1 clock và APB2 clock.
  -Ký tự '\' trong lập trình C được sử dụng để chia nhỏ dòng code dài thành nhiều dòng ngắn hơn, giúp code dễ 
  đọc hơn. Nó giống như việc bạn nhấn phím Enter để xuống dòng, nhưng trình biên dịch sẽ coi đó là một dòng code 
  duy nhất.
  
 + Hàm sprintf(): Đây là hàm tiêu chuẩn trong C dùng để định dạng chuỗi văn bản (string formatting). Hàm này sẽ
 định dạng các giá trị được truyền vào và lưu trữ kết quả vào một chuỗi đã được chỉ định.
 
 + "Tick" thường được hiểu là một đơn vị thời gian nhỏ nhất mà hệ thống có thể đo lường. Trong trường hợp của 
 STM32F4, "tick" thường được sử dụng để tính toán thời gian trễ hoặc thời gian thực hiện của các tác vụ.
  -Tính toán "tick" từ HCLK: Ví dụ sử dụng HCLK = 4MHz <=> 1 giây có 4*10^6 xung = 4.000.000 xung 
  -> Cần (1/4.000.000) = 0.25 * 10^-6 giây = 0.25 micro giây để chạy 1 xung
  -> Số "tick" đếm đc trong 1 mili giây(số giây cần để ngắt lặp lại) là (1ms0.25 micro giây) <=> 4000000/1000
  => Cách tính đơn giản số "tick" của hàm "HAL_SYSTICK_Config()" là lấy số HCLK đc cấu hình và chia cho 1000
  
  -Hàm "HAL_SYSTICK_Config" là: Khởi tạo Bộ đếm thời gian hệ thống và ngắt của nó và khởi động Bộ đếm thời gian 
  tích tắc hệ thống(Bộ đếm đang ở chế độ chạy tự do để tạo ra các ngắt định kỳ).

 + Giải thích: Phải gọi lại hàm UARTx_Init() vì khi gọi lần 1 ta cấu hình cho APB bus dựa trên HSI=16MHz. 
 sau đó ta lại tiến hành cấu hình HSE cùng các bộ chia dẫn đến theo đề bài tần số APBx Bus là khác nhau thì dẫn
 đến sẽ có sự khác nhau về lấy mẫu baud rate của các UARTx. 
 ->Việc gọi lại để cấu hình 1 lần nữa thông số UARTx mà mình muốn sử dụng.
 ->Nghĩa là: ở lần gọi 1 thì baud rate UART2 sẽ dựa trên HSI=16MHz. Tuy nhiên, sau khi cấu hình clk mới thì xung 
 clk trên UART2 treo chỉ còn 2MHz dẫn đến sẽ có sự sai khác lấy mẫu baud rate. Vì vậy việc gọi lại hàm khởi
 tạo UART2_Init() dùng để cấu hình lại baud rate dựa trên xung clk mới của APB1 là 2MHz
*/