/*
 * tutorial.c
 *
 *  Created on: Oct 14, 2023
 *      Author: Minh
 */
/**
**=MCU Clock And Details - STM32F407VGT6(S8-V34)
+ Clock là một thành phần quan trọng của bất kỳ bộ vi điều khiển nào, vì ko có "Clock" thì ko có gì có thể hoạt động.
Nguyên nhân là vi điều khiển là một mạch kỹ thuật số đồng bộ(synchoronous), nó sẽ đồng bộ với Clock.
+ Clock: là một số tín hiệu điện sóng vuông có tần số nhất định .
+ Với một ứng dụng nhất định, việc chọn "Clock" nào phù hợp sẽ rất quan trọng bởi vì giả sử ứng dụng của bạn là một
ứng dụng công suất thấp(low power application) thì nghĩa là bạn nhắm đến mục tiêu mức tiêu thụ năng lượng thấp, thì
tần số "Clock" nên sử dụng phải thấp, vì có mối quan hệ giữa tần số clock và mức tiêu thụ năng lượng.
+ Trong bài này ta sẽ sử dụng "Clock" của STM32F407VGT6 làm ví dụ minh họa:
 ^ Tài liệu: DM00031020 -> 7.2 Clocks
 ^ Trong STM32F407VGT6 sẽ có 3 "Clock Source" khác nhau có thể đc sử dụng điều khiển "System Clock-SYSCLK"

  ~"HSE" oscillator clock(Crystal Oscillator - Bộ dao động thạch anh) là bộ tạo dao động thạch anh sẽ đc kết nối
  với bộ vi điều khiển nhằm cung cấp nguồn xung clock từ bên ngoài. (8MHz) (cùng nằm trong board mạch STM32F4_Discovery)

  ~"HSI" oscillator clock(RC Oscillator): là bộ tạo nguồn xung clock từ bên trong bộ vi điều khiển(tần số 16MHz)

  ~Main PLL(Phase Locked Loop - Vòng lặp khóa giai đoạn) Clock("PLLCLK"): là nguồn xung clock đc tạo từ bên trong bộ vi
  điều khiển, bộ này sẽ cung cấp đầu ra là các "higher frequency clock" với đầu vào là "lower frequency clock"

**=MCU Clock Tree(S10)
+ Tìm hiểu về "RCC Block" hoặc "RCC Periperal" của vi điều khiển
  Tài liệu: DM00031020 -> 7.2 Clocks -> Figure21 Clock Tree
  ^Đây là một sơ đồ Clocking engine of the microcontroller

+ Tiến hành tạo một Project với "Targeted Project Type" là chọn "STM32Cube" để có thể cấu hình clock(clock configuration)
  ^Xem Video để biét cách cài đặt cho Project(2p50s) -> (Vd: 04_MCU_Clock)
  ^Sau khi tạo ta có đc GUI mô tả sơ đồ nhiều thứ, Chú trọng đến "Clocking Configuaration"
  ^Dựa vào "Clocking Configuaration" ta có thể lựa chọn 1 trong 3 "Clock Source"
  ^Phần "Input frequency"(gần khối HSE): cho phép chúng ta cấu hình tần số input

+ Tìm hiểu "HSE Clock"(V35-5p10s)
  ^Tài liệu: DM00031020 -> 7.2 Clocks -> 7.2.1 HSE Clock
  ^Có hai cách để cung cấp xung clock sử dụng khối HSE là
   ~Dùng "Crytals/Ceramic resonators-bộ cộng hưởng" bên ngoài kết nối với vi điều khiển
   ~Dùng một "External Clock-Xung ngoại" đc cung câp từ một mạch khác sau đó chuyển đổi thành HSC
   ~Lưu ý: Trên các dòng "Nucleo" thì ko tích hợp các "Bộ cộng hưởng" nên thường dùng "External Clock"

  ^Quan sát tài liệu về board STM32F407DISC1
   ~Phần 6.9 -> 6.9.1 OSC clock supply
   ~Có 3 cách để tạo xung clock là:
    %MCO from ST-LINK
	%Oscillator on Board(Crytal- 2 Khối X2,X3 trong sơ đồ khối)
	%Oscillator from external port PH0
   ~Ta sẽ sử dụng sơ đồ khối để kiểm tra các cổng kết nối với clcok mong muốn mà Datasheet đã viết hướng dẫn

  ^HSE có thể đc cung cấp cho microcontroller thông qua một "Khối Crystal" hoặc "Khối External-đc cung câp bởi
  một MCU khác"
  ^Trên STM32-DISC board, thì HSE là 8MHz đc cung cấp bởi khối "Crystal" tích hợp trên Board
  ^Trên Nucleo board, HSE là 8MHz đc kéo từ mạch ST-LINK
  ^Lưu ý: ko nên thay đổi các cài đặt mạc định, trừ khi bạn đc yêu cầu trong khóa học

  ^Cách để kích hoạt khối HSE trong "Clock Configuaration"(V36-2p35)
   ~Chọn "Pinout & Configuaration" -> System Core -> RCC > High Speed Clock(HSE) -> Crystal/Ceramic Resonator

+ Tìm hiểu "HSI Clock"(V36)
  ^Tài liệu: DM00031020 -> 7.2 Clocks -> 7.2.2 HSI Clock
  ^Tần số cung cấp 16MHz
  ^HSI là clock thường đc cài đặt mặc định cho microcontroller. Khi reset, microcontroller sẽ thiết lập HSI làm
  main system clock. Sau đó, thông qua code, bạn có thể thay đổi thành một "Clock Source" khác.
  ^Khi quan sát "Clocking Configuaration" ta thấy là HSI sẽ đc nối với khối "HSI RC - 16MHz"
  ^Nếu bạn ko có bộ "Crystal Clock" thì bạn vẫn có thể chạy bộ vi điều khiển của mình thông qua "Internal RC Oscillator"
  Đó là bộ HSI hoặc PLLCLK

  ^Lưu ý: Sau khi chọn đc "Clock Source" thì ta sẽ xác định đc tần số cho bộ "SYSCLK". Sau đó ta có thể thiết lập thông số
  tần số với 2 khối "AHB Prescaler- bộ chia" và "HCLK(MHz)" từ đó thiết lập các tần số cho các System Bus kết nối
  (vd như: APB1 peripheral, APB1 Timer clock,...)

+ Tìm hiểu "PLLCLK"
  ^Khối "PLLCLK" Là một "clock multiplier engine-xung clock từ nhân" của microcontroller, đc sử dụng để tăng xung clock lên
  cao hơn HSI(16MHz) hoặc HSE(8MHz) trong trường hợp bạn muốn một tần số cao hơn tần số mặc định của 2 khối này
  ^Khi chọn khối "PLLCLK" làm system clock thì đầu vào sẽ là các "HSI hoặc HSE", sau đó qua xử lý của khối "Main PLL - tích
  hợp các bộ chia thay đổi tần số" thì có thể đẩy tần số clock lên đến max 100MHz
  ^Lưu ý: Nếu bạn sử dụng một I2C Clock thì khối này có "Clock Source" là khối PLLCLK

+ Tìm hiểu về RCC Register
  ^Tài liệu: DM00031020 -> 7.3 RC Register
  ^Cung cấp các thanh ghi nhằm cấu hình cho các "Clock Source" cũng như các khối "AHB Prescaler" hay ""APBx Prescaler"


+ Tìm hiểu về Periperal Clock Configuaration - Cấu hình clock cho các ngoại vi(V37)
  ^Bài này sẽ tìm hiểu về cách kích hoạt(enable) một peripheral clock cho một ngoại vi cụ thể
  ^Trong hầu hết các bộ vi điều khiển, trước khi sử dụng bất kỳ peripheral nào, bạn phải kích hoạt "peripheral clock"
  đc điều khiển bởi "peripheral clock register" của peripheral đó
  ^Theo mặc định, peripheral clock của hầu hết peripheral sẽ bị vô hiệu hóa(disable) để tiết kiệm năng lượng(save power)
  ^Một Periperal sẽ không mất hoặc trả lời(repond) với các giá trị cấu hình(việc code) của bạn cho đến khi bạn kích
  hoạt(enable) peripheral clock của nó
  ^Trong STM32 microcontroller, thì các peripheral clock đc quản lý thông qua các thanh ghi "RCC register"
  ^Tài liệu tham khảo: DM00031020 -> 7.3 RC Register(chứa các thanh ghi RCC mà bạn muôn cấu hình)
   ~Bạn sẽ chọn thực hiện một Periperal. Sau đó tiến hành chọn đến thanh ghi RCC của Periperal để cấu hình các bit
   ~Vd: bạn chọn điều khiển Periperal ADC (Vd: 04_1_Peri_Clock_Enable)

+ Bài tập: HSI(16MHz) measurement(Phép đo)(V38)
 ^Viết một chương trình(program) để đầu ra HSI clock trên pin vi điều khiển(nghĩa là HSI clock là main system clock cho các pin
 của microcontroller) và đo nó bằng cách sử dụng osciloscope(máy dò sóng) hoặc logic analyzer
 ^Các bước thực hiện:
  1.Chọn(Select) clock source mong muốn cho MCOx signal(Microcontroller Clock Output)
  2.Thiết lập Output(xuất) tín hiệu(signal) MCOx trên MCU pin
 ^Hướng dẫn việc thực hiện trên STM32CubeMx(Xem Video 2p00) -> thực hiện trên file 04_MCU_Clock
  ~System Core -> RCC -> Tích vào 2 ô Master Clock Output 1 và 2 -> Kích hoạt Khối MCO1_2 source mux phần "Clock Configuaration"
  ~HSI hoạt động với tần số 16MHz và ta sử dụng bộ chia 4 -> MCO1 hoạt động với tần số là 4MHz
  ~Datasheet -> 7.3.3 RCC clock configuration register -> MCO1 thuộc 2 bit là 21 và 22 và ta có 4 lựa chọn cặp số bit
  và theo đề bài tạo HSI clock nên cặp số cần thiết là 00:HSI clock select
  ~Tài liệu: ds_stm32f407 -> phần 3 Pinout and pin desciption -> Table 9.Alternate function mapping
   %Table này mô tả một chức năng thay thế khác nhau của các chân pin vi điều khiển, ta có 16 chế độ từ AF0 đến AF15,
   Có nghĩa là bạn có thể ghép cách chức năng khác nhau trên một chân pin của vi điều khiển bằng cách thay đổi chế độ
   của chân pin đó, túc là một pin của vi điều khiển có thể thay đổi thành các chức năng khác nhau tùy vào lập trình viên
   %Theo bảng này thì với chan PA8(Port A pin 8) thì nó có các chế độ là MCO1(AF0),I2C3_SCL(AF4),USART1_CK(AF7),...
   ->Với yêu cầu MCO1 thì chân cần MCU pin cần là là PA8(mã pin 67)
  ~Tiến hành sử dụng phần cứng "logic analyzer" kết hợp phần mềm để đọc tín hiệu xung clock HSI trên máy tính (V39)
   %Cách kết nối chân: pin PA8(Board) nối vào pin CHO(logic analyzer USB), GND(Board) nối vào pi GND(logic analyzer USB)
   Đầu USB(logic analyzer USB) sau đó nối vào máy tính -> Sử dụng phần mềm để quan sát(V40-6p10s)

+ Tiến hành code bài tập HSI measurement(Ko sử dụng cấu hình Cube Mx)(V40)
 ^File thực hiện: 04_2_HSI_Measurement

+ Bài tập: HSE(8MHz) measurement(Phép đo)(V41)
 ^Viết một chương trình bật(switch) HSE thành system clock và measure nó
 ^Các bước thực hiện(STM32F407DISC1):
  1.Kích hoạt(Enable) HSE Clock bằng cách sử dụng HSEON bit (RCC_CR)
  2.Bạn phải chờ đợi cho đến khi(untill) HSE clock từ "external crytal" ổn định(stabilizes)(trường hợp nếu khối crytal
  đc kết nối với vxl để tạo xung clock)(cho biết bộ dao động ngoài tốc độ cao có ổn định hay không)
  3.Bật system clock thành HSE(RCC_CFGR register)
  4.Thực hiện MCO1 setting và measure nó

 ^Các bước thực hiện(Nucleo Board):
  1.Enable the HSEBYP bit (RCC_CR register) (bypass(bỏ qua) the oscillator(bộ dao động) with an external(ngoại) clock )
  2.Enable the HSE clock using HSEON bit(RCC_CR register)
  3.Switch the system clock to HSE
  4.DO MCO1 setting to measure it
*/

